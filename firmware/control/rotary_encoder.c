#include <avr/interrupt.h>

#include "rotary_encoder.h"

static rtenc_state* __rtenc_state;

/**
 * Set up rotary encoder pins
 * This assumes pin 2 & 3 are CLK and DAT, and pin 10 is the press switch
 */
void rtenc_setup(void)
{
	DDRD &= ~(1 << 2 | 1 << 3); // Pin 2 & 3 as inputs
    DDRD &= ~_BV(4); // Pin 10 as input

    PORTD |= _BV(4); // Pull-up for switch

    EIMSK |= (1 << INT0) | (1 << INT1); // Enable pin change interrupts for PCINT 1 and 0
    EICRA |= (1 << ISC01) | (1 << ISC00); // Falling edge on INT0 generates an interrupt
    EICRA |= (1 << ISC11) | (1 << ISC10); // Rising edge on INT1 generates an interrupt
    sei(); // enable interrupts
}

/**
 * Bind all rotary encoder movements to a particular state referemce
 */
void rtenc_bind(rtenc_state *state)
{
	__rtenc_state = state;
}

/**
 * Handle either interrupt triggering
 */
void _handleMove(void)
{
    static volatile short encoderState = 0;

    cli();

    encoderState <<= 2; // Shift previous state
    encoderState |= 0x03 & (PIND >> 2); // Add bits 2 and 3 to the state

    // Use the lower 4 state bits to select a value base on the last two updates
    switch (encoderState & 0xf) {
        case 0b1011:
            __rtenc_state->position -= 1;
            break;

        case 0b0111:
            __rtenc_state->position += 1;
            break;
    }

    sei();
}

/**
 * Interrupt for pin 2
 */
ISR(INT0_vect)
{
    _handleMove();
}

/**
 * Interrupt for pin 3
 */
ISR(INT1_vect)
{
    _handleMove();
}
