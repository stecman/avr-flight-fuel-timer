/* Name: main.c
 * Author: Stephen Holdaway <stephen@stecman.co.nz>
 * Date: 25 June, 2016
 * License: MIT
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/**
 * This code was written using an Arduino mini 5V (ATmega 328p)
 * The pin and port numbers used in this code reflect the AVR pinout,
 * not the Arduino pinout. A mapping between these can be found here:
 *
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */

/**
 * Go into a super low clock speed state and wait a number of seconds
 * Need to test if this is actually useful for saving power
 */
void low_clock_delay(char seconds)
{
    clock_prescale_set(clock_div_256); // 62KHz on 328p
    while (seconds > 0) {
        seconds--;
        _delay_ms(1000/256);
    }
    clock_prescale_set(clock_div_1); // return to 16MHz
}

/**
 * Watch dog timer interrupt handler
 */
ISR(WDT_vect) {
    wdt_reset();
    wdt_disable();
    
    PORTD ^= 1 << 6; // Toggle digital pin 6

    // Show we've been interrupted
    char i;
    for (i = 0; i < 2; i++) {
        PORTB ^= 1 << 5; // Toggle LED
        _delay_ms(50);
    }

    _delay_ms(3000);

    for (i = 0; i < 4; i++) {
        PORTB ^= 1 << 5; // Toggle LED
        _delay_ms(50);
    }
    
    low_clock_delay(3);

    for (i = 0; i < 6; i++) {
        PORTB ^= 1 << 5; // Toggle LED
        _delay_ms(50);
    }
}

/**
 * Put the microcontroller in low power sleep for a period of time, waking on any interrupt
 * 
 * This uses the watchdog timer to wake by default, but any interrupt will bring the controller
 * out of its heavy sleep state
 *
 * Pass one of the WDTO_* constants to specify a time
 * http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__watchdog.html
 */
void cpu_sleep_delay(char WDTO_value)
{
    cli(); // disable interrupts

    // Remove the safety on sleep
    sleep_enable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    wdt_reset();
    wdt_enable(WDTO_value);

    // Put the watchdog timer in interrupt mode
    WDTCSR = _BV(WDIF) | _BV(WDIE) | _BV(WDCE);

    sei(); // enable interrupts
    sleep_mode(); // actually sleep

    // Execution reaches here after wake up by watch dog timer
    sleep_disable();
    wdt_reset();
    wdt_disable();
    power_all_enable(); // re-enable peripherals
}

int main(void)
{
    // Recover from any watchdog timer reset mishaps
    wdt_reset();
    wdt_disable();

    DDRB = 1 << 5; // LED as output
    DDRD = 1 << 6; // Digital pin 6 as output
    
    char i = 5;
    do {
        // Brief boot-up time to allow for screwing up the watch dog timer
        // Watchdog isn't affected by reset pin, so manual timing of power-on is needed
        // to flash if bad code is flashed during development
        PORTB ^= 1 << 5; // Toggle LED
        low_clock_delay(1);
    } while(i--);

    _delay_ms(3000);

    for (;;) {
        cpu_sleep_delay(WDTO_4S);
    }

    return 0;
}

