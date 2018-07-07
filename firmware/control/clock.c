#include "clock.h"

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

// Default I2C device addresses as stated on page 17 of the datasheet
static const uint8_t DS3231_ADDR = 0b11010000;

EventHandler _tickHandler;

uint16_t rtc_read_time(void)
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

    // Send repeat start to switch to read mode
    i2c_send_start();

    // Check if I2C has send a repeat start condition
    if (!i2c_has_status(TW_REP_START)) {
        return false;
    }

    i2c_transmit(DS3231_ADDR | TW_READ);

    if (!i2c_has_status(TW_MR_SLA_ACK)) {
        return false;
    }

    uint8_t buffer[3] = {};

    // Read seconds, minutes and hours in BCD form
    for (uint8_t i = 0; i < 3; ++i) {
        buffer[i] = i2c_receive();
    }

    // Done
    i2c_send_stop();

    // Calculate seconds from BCD
    uint16_t seconds = 0;
    seconds += (buffer[0] & 0x0F); // Seconds
    seconds += (buffer[0] & 0xF0) * 10; // 10s of seconds

    uint8_t minutes = (buffer[1] & 0x0F); // Minutes
    minutes += (buffer[1] & 0xF0) * 10; // 10s of minutes

    uint8_t hours = (buffer[2] & 0x0F); // Hours
    hours += (buffer[2] & 0x10) * 10; // 10s of hours
    hours += (buffer[2] & 0x20) * 20; // 20s of hours

    seconds += (minutes * 60) + (hours * 60 * 60);

    return seconds;
}

bool rtc_set_control_register(uint8_t data)
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
    i2c_transmit(0x0E);
    i2c_transmit(data);

    bool status = i2c_has_status(TW_MT_SLA_ACK);

    // Done
    i2c_send_stop();

    return status;
}

void rtc_setup(void)
{
    // PD3 as input with pull-up (required by the DS3231's SQW/INT pin)
    DDRD &= ~_BV(PD3);
    PORTD |= _BV(PD3);
}

void rtc_update_on_tick(bool enable, EventHandler handler)
{
    // Disable interrupts immediately to avoid an extra tick happening during disabling
    cli();

    if (enable) {
        // Zero the control register to get a 1Hz clock from the SQW pin
        rtc_set_control_register(0x0);

        // Trigger INT1 on falling edge
        EICRA |= _BV(ISC11);

        // Enable INT1 interrupt
        EIMSK |= _BV(INT1);

        _tickHandler = handler;

    } else {

        // Enable alarm interrupts to disable SQW output
        rtc_set_control_register(_BV(CTRL_INTCN));

        // Disable INT1 interrupt
        EIMSK &= ~_BV(INT1);

        // Clear any pending interrupt from INT1
        EIFR |= _BV(INTF1);

        _tickHandler = NULL;
    }

    sei();
}



// Interrupt for 1Hz signal from RTC
ISR(INT1_vect)
{
    if (_tickHandler != NULL) {
        global_eventloop_queue(_tickHandler);
    }
}
