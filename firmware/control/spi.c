#include "spi.h"

#include <avr/io.h>

// Pin assignments
#define PIN_SS PB2
#define PIN_MOSI PB3
#define PIN_MISO PB4
#define PIN_SCLK PB5

void spi_init(void)
{
    // Output lines
    // Note that the Slave Select (SS) pin is only set here to prevent the SPI
    // peripheral initialising in slave mode (see MSTR in the datasheet)
    DDRB |= _BV(PIN_MOSI) | _BV(PIN_SCLK) | _BV(PIN_SS);

    // Input line with pull-up
    DDRB &= ~_BV(PIN_MISO);
    PORTB |= _BV(PIN_MISO);

    // Enable SPI as master, set clock rate fck/2
    SPCR = (1<<SPE) | (1<<MSTR);
    // SPSR = (1<<SPI2X);
}

void spi_write(const uint8_t data)
{
    // Send the data
    SPDR = data;

    // Wait for the transmission to complete
    while (!(SPSR & (1<<SPIF)));
}

uint8_t spi_read(void)
{
    spi_init();
    // Send something to trigger an SPI transmission
    SPDR = 0x0;

    // Wait for the transmission to complete
    while (!(SPSR & (1<<SPIF)));

    // Read receieved byte
    return SPDR;
}