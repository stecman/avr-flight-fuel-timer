#include "rtc.h"

#include "beeper.h"
#include "control/i2c.h"
#include "system.h"

#include <avr/interrupt.h>

// DS3231 Control Register bits
#define CTRL_A1IE 0
#define CTRL_A2IE 1
#define CTRL_INTCN 2
#define CTRL_RS1 3
#define CTRL_RS2 4
#define CTRL_CONV 5
#define CTRL_BBSQW 6
#define CTRL_EOSC 7

// DS3231 Status Register bits
#define STATUS_OSF 7
#define STATUS_EN32KHz 3
#define STATUS_BUSY 2
#define STATUS_A2F 1
#define STATUS_A1F 0

// Default I2C device addresses as stated on page 17 of the datasheet
static const uint8_t DS3231_ADDR = 0b11010000;

// Master value for control register
// This is written at start-up to put the RTC in a known state
static uint8_t _controlRegister = 0x0;

EventHandler _tickHandler;

static bool _set_register(const uint8_t address, const uint8_t* data, const uint8_t size)
{
    // Start and confirm success
    i2c_send_start();

    if (!i2c_has_status(TW_START)) {
        return false;
    }

    // Transmit address
    i2c_transmit(DS3231_ADDR | TW_WRITE);

    if (!i2c_has_status(TW_MT_SLA_ACK)) {
        return false;
    }

    // Send the Control Register address and the new value
    i2c_transmit(address);

    for (uint8_t i = 0; i < size; ++i) {
        i2c_transmit(data[i]);
    }

    bool status = i2c_has_status(TW_MT_SLA_ACK);

    // Done
    i2c_send_stop();

    return status;
}

static bool _get_register(uint8_t offset, uint8_t* buffer, uint8_t size)
{
    // Start and confirm success
    i2c_send_start();
    if (!i2c_has_status(TW_START)) {
        return false;
    }

    // Set memory offset to read from
    i2c_transmit(DS3231_ADDR | TW_WRITE);
    if (!i2c_has_status(TW_MT_SLA_ACK)) {
        return false;
    }

    // Transmit memory offset
    i2c_transmit(offset);

    // Restart in read mode
    i2c_send_start();
    if (!i2c_has_status(TW_REP_START)) {
        return false;
    }

    //  in read mode
    i2c_transmit(DS3231_ADDR | TW_READ);

    // Read requested data, flagging when the last byte is being requested
    for (uint8_t i = 0; size > 0; --size) {
        buffer[i] = i2c_receive(size == 1);
        ++i;
    }

    // Done
    i2c_send_stop();

    return true;
}

bool rtc_read_time(RtcTime* output)
{
    // Read seconds, minutes, hours and date in BCD form
    // See page 11 in the DS3231 datasheet for a full description of the encoding
    uint8_t buffer[7];

    if (_get_register(0x0, buffer, 7)) {
        output->second = (buffer[0] & 0x0F) + // Seconds
                          ((buffer[0] >> 4) * 10); // 10s of seconds

        output->minute = (buffer[1] & 0x0F) + // Minutes
                          ((buffer[1] >> 4) * 10); // 10s of minutes


        output->hour = (buffer[2] & 0x0F) + // Hours
                        ((buffer[2] & 0x10) >> 4) * 10 + // 10s of hours
                        ((buffer[2] & 0x20) >> 5) * 20; // 20s of hours

        output->day = (buffer[4] & 0x0F) + // Day
                      ((buffer[4] >> 4) * 10); // 10s of days

        output->month = (buffer[5] & 0x0F) + // Month
                        ((buffer[5] & 0x10) >> 4) * 10; // 10s of months

        output->year = (buffer[6] & 0x0F) + // Year
                       ((buffer[6] >> 4) * 10); // 10s of years

        return true;
    }

    return false;
}

bool rtc_set_time(RtcTime* input)
{
    // Buffer for new time registers in DS3231 (BCD)
    uint8_t settings[7];

    // Seconds
    settings[0] = ((input->second / 10) << 4) | (0x0F & (input->second % 10));

    // Minutes
    settings[1] = ((input->minute / 10) << 4) | (0x0F & (input->minute % 10));

    // Hours
    uint8_t hours = input->hour;
    settings[2] = 0x0;

    if (hours >= 20) {
        settings[2] |= (1<<5);
        hours -= 20;
    }

    if (hours >= 10) {
        settings[2] |= (1<<4);
        hours -= 10;
    }

    settings[2] |= (hours & 0x0F);

    // Day of the week (we don't care about this)
    settings[3] = 0;

    // Day
    settings[4] = ((input->day / 10) << 4) | (0x0F & (input->day % 10));

    // Month
    // Bit 7 of this address is the century, but we don't use this
    settings[5] = ((input->month / 10) << 4) | (0x0F & (input->month % 10));

    // Year
    settings[6] = ((input->year / 10) << 4) | (0x0F & (input->year % 10));

    return _set_register(0x0, settings, 7);
}

inline static bool _update_control_register(void)
{
    return _set_register(0x0E, &_controlRegister, 1);
}

inline static void _start_rtc(void)
{
    // Get status register
    uint8_t status;
    _get_register(0xF, &status, 1);

    // Check if Oscillator Stop Flag is set
    if ( (status & _BV(STATUS_OSF)) != 0) {

        // Write default time as the RTC is in an unknown state
        const uint8_t defaultTime[] = {
            0x0, // 0 seconds
            0x0, // 0 minutes
            (1<<4) | (1<<1), // 12 hours
            0x0, // Unknown weekday
            0x1, // day 1
            0x1, // month 1
            0x18, // 2018
        };
        _set_register(0x0, defaultTime, sizeof(defaultTime));

        // Clear OSF flag
        status &= ~_BV(STATUS_OSF);

        // Write back to allow RTC to run
        _set_register(0xF, &status, 1);
    }

}

void rtc_setup(void)
{
    // PD3 as input with pull-up (required by the DS3231's SQW/INT pin)
    DDRD &= ~_BV(PD3);
    PORTD |= _BV(PD3);

    // Set control register to initial value
    _update_control_register();

    // Reset RTC if power was lost
    _start_rtc();
}

void rtc_update_on_tick(bool enable, EventHandler handler)
{
    // Disable interrupts immediately to avoid an extra tick happening during disabling
    cli();

    if (enable) {
        // Zero the control register to get a 1Hz clock from the SQW pin
        _controlRegister &= ~_BV(CTRL_INTCN);

        // Trigger INT1 on falling edge
        EICRA |= _BV(ISC11);

        // Enable INT1 interrupt
        EIMSK |= _BV(INT1);

        _tickHandler = handler;

    } else {

        // Enable alarm interrupts to disable SQW output
        _controlRegister |= _BV(CTRL_INTCN);

        // Disable INT1 interrupt
        EIMSK &= ~_BV(INT1);

        // Clear any pending interrupt from INT1
        EIFR |= _BV(INTF1);

        _tickHandler = NULL;
    }

    _update_control_register();

    sei();
}



// Interrupt for 1Hz signal from RTC
ISR(INT1_vect)
{
    if (_tickHandler != NULL) {
        global_eventloop_queue(_tickHandler);
    }
}
