#include "fram.h"

#include "control/spi.h"

#include <avr/io.h>
#include <util/crc16.h>
#include <util/delay.h>

// Pin assignments
#define PIN_CS PC0

enum OpCode {
    kWriteEnable = 0x06, // WREN
    kWriteDisable = 0x04, // WRDI
    kReadStatusRegister = 0x05, // RDSR
    kWriteStatusRegister = 0x01, // WRSR
    kReadMemoryData = 0x03, // READ
    kWriteMemoryData = 0x02, // WRITE
};

enum StatusRegisterBits {
    // The WPEN bit controls the effect of the hardware /WP pin. When WPEN is
    // low, the /WP pin is ignored. When WPEN is high, the /WP pin controls write
    // access to the status register. Thus the Status register is write protected
    // if WPEN=1 and /WP=0.
    SREG_WPEN = 0b10000000,

    // BP1 and BP0 are memory block write protection bits.
    // As a 2-bit number the modes are:
    //   0: No protection
    //   1: 0x600 to 0x7FF (upper quarter)
    //   2: 0x400 to 0x7FF (upper half)
    //   3: 0x000 to 0x7FF (all)
    SREG_BP1 = 0b00001000,
    SREG_BP0 = 0b00000100,

    // The WEL bit indicates the state of the Write Enable Latch.
    // This bit is read only. Set and cleared using the WREN and WRDI ops
    SREG_WEL = 0b00000010,

    // Bits 0 and 4-6 are fixed as 0 and cannot be modified
    // This mask allows checking if a read status register value is sane
    SREG_UNUSED_MASK = 0b01110001,
};

/**
 * Select chip for SPI communication, starting an operation
 * Only one opcode can be used per chip-enable
 */
static inline void startOp(void)
{
    PORTC &= ~_BV(PIN_CS);
}

/**
 * Select chip for SPI communication, starting an operation
 */
static inline void endOp(void)
{
    PORTC |= _BV(PIN_CS);
}

static void writeStatusReg(uint8_t value)
{
    // Enable write latch (valid for the next op)
    startOp();
    spi_write(kWriteEnable);
    endOp();

    startOp();
    spi_write(kWriteStatusRegister);
    spi_write(value);
    endOp();
}

static uint8_t readStatusReg(void)
{
    uint8_t value = 0;

    startOp();
    spi_write(kReadStatusRegister);
    value = spi_read();
    endOp();

    return value;
}

static uint16_t calculateChecksum(uint8_t* data, uint8_t size)
{
    uint16_t checksum = 0xFFFF;

    for (uint8_t i = 0; i < size; ++i) {
        checksum = _crc16_update(checksum, data[i]);
    }

    return checksum;
}

void fram_init(void)
{
    // Set CS as output, initially high (line is active low)
    DDRC |= _BV(PIN_CS);
    PORTC |= _BV(PIN_CS);

    // Enable the write protect pin (tied to CS in hardware) to avoid ISP programming affecting the F-RAM
    // (The CS and Write Protect lines are active low, which is the state of the shared pin after reset)

    // TODO: return error on failed initialisation instead of trying forever
    while (true) {
        uint8_t statusReg = SREG_WPEN;
        writeStatusReg(statusReg);

        // Verify written value
        if (readStatusReg() == statusReg) {
            break;
        }

        _delay_ms(100);
    }
}

FramOpStatus fram_write(uint16_t address, void* data, uint8_t size)
{
    // Hard code to version zero for now
    uint8_t version = 0;

    uint8_t* byteData = data;
    uint16_t checksum = calculateChecksum(data, size);

    // Include version number in CRC
    checksum = _crc16_update(checksum, version);

    // Enable writing to memory
    // This stays active until the end of the next write memory op
    startOp();
    spi_write(kWriteEnable);
    endOp();

    // Sanity check the F-RAM is present
    {
        uint8_t status = readStatusReg();

        if ((status & SREG_UNUSED_MASK) != 0) {
            // Something is wrong with communication: the F-RAM should never return any of these bits for the status register
            return kFram_Failed;
        }

        // Check if the write latch is set as expected
        if ((status & SREG_WEL) == 0) {
            // Status register did not reflect kWriteEnable being issued
            // This probably indicates a hardware communication problem
            return kFram_Failed;
        }
    }

    startOp();

    // Issue write op and address
    spi_write(kWriteMemoryData);
    spi_write(address >> 8);
    spi_write(address & 0xFF);

    // Write version
    spi_write(version);

    // Write two-byte checksum before data
    spi_write(checksum >> 8);
    spi_write(checksum & 0xFF);

    // Write payload
    for (uint8_t i = 0; i < size; ++i) {
        spi_write(byteData[i]);
    }

    endOp();

    return kFram_Success;
}

FramOpStatus fram_read(uint16_t address, uint8_t size, void* output)
{
    uint8_t* byteOutput = output;

    // Disable any outstanding write to memory
    startOp();
    spi_write(kWriteDisable);
    endOp();

    // Sanity check the F-RAM is present
    if ((readStatusReg() & SREG_UNUSED_MASK) != 0) {
        // Something is wrong with communication: the F-RAM should never return any of these bits for the status register
        return kFram_Failed;
    }

    startOp();

    spi_write(kReadMemoryData);
    spi_write(address >> 8);
    spi_write(address & 0xFF);

    // Read the data format version
    // This is a currently unused placeholder for future update handling
    uint8_t version = spi_read();

    // Read the two-byte checksum stored after the data
    uint16_t storedChecksum = (spi_read() << 8);
    storedChecksum |= spi_read();

    for (uint8_t i = 0; i < size; ++i) {
        byteOutput[i] = spi_read();
    }

    endOp();

    // Calculate checksum CRC
    uint16_t checksum = calculateChecksum(byteOutput, size);
    checksum = _crc16_update(checksum, version);

    // Check if checksum matches
    if (storedChecksum != checksum) {
        return kFram_BadChecksum;
    }

    return kFram_Success;
}