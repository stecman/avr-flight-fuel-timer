#include "beeper.h"

// AVR
#include <avr/io.h>
#include <util/delay.h>

void beeper_setup()
{
    // PC2 as beeper output
    DDRC |= _BV(PC2);
}

void beeper_blip()
{
    PORTC |= _BV(PC2);
    _delay_ms(2);
    PORTC &= ~_BV(PC2);
}

