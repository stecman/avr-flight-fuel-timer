#include <avr/interrupt.h>

#include "rotary_encoder.h"
#include "../beeper.h"

// Global state where rotary encoder input should go
// This is changed using rtenc_bind()
static rtenc_state* _current_state;

// Set up rotary encoder pins and interrupts
void rtenc_setup(void)
{
    DDRD &= ~(_BV(PD2) | _BV(PD3) | _BV(PD4)); // Encoder and push-button as inputs
    PORTD |= _BV(PD2); // Pull-up for push-button

    EIMSK |= _BV(INT0); // Enable INT0 for push-button
    EICRA |= _BV(ISC11); // Trigger on falling edge

    // Enable pin change interrupt for both rotary encoder outputs (PD3/PD4)
    PCMSK2 |= _BV(PCINT19) | _BV(PCINT20);
    PCICR |= _BV(PCIE2);

    sei(); // enable interrupts
}

// Bind all rotary encoder movements to a particular state reference
void rtenc_bind(rtenc_state *state)
{
    _current_state = state;
}

// Interrupt for rotary encoder button presses
// INT0 is used here so this switch can be used to wake the device up
ISR(INT0_vect)
{
    // Bail out if there's no target to recieve the action
    if (!_current_state) return;

    _current_state->button_pressed = true;
    beeper_blip();
}

// Interrupt for both outputs from the encoder
//
// We're using a pin change interrupt, so there are four interrupts for each
// detent on the rotary encoder. This can be done with a single interrupt, but
// the results aren't as reliable.
ISR(PCINT2_vect)
{
    // State to keep track of which way the encoder is turning
    static uint8_t encoder_seq = 0x0;

    // There should always be a target for encoder input, but bail out
    // if it's not present to avoid clobbering anything accidentally
    if (!_current_state) return;

    encoder_seq <<= 2; // Shift last state
    encoder_seq |= 0b11 & (PIND >> 3); // Copy PIND state (bits 3 and 4) to bits 0 and 1

    // Check if the sequence of the the last four updates indicates turn
    // The bits in each sequence indicate the state of PD4 and PD3 over time (LSBs are newer)
    switch (encoder_seq) {
        case 0b10000111:
            _current_state->position -= 1;
            beeper_blip();
            break;

        case 0b01001011:
            _current_state->position += 1;
            beeper_blip();
            break;
    }
}
