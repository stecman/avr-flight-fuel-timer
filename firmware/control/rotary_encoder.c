#include "rotary_encoder.h"

#include <avr/interrupt.h>

#include "system.h"

EventHandler _incrementHandler;
EventHandler _decrementHandler;
EventHandler _shortPressHandler;

// Set up rotary encoder pins and interrupts
void rtenc_setup(void)
{
    DDRD &= ~(_BV(PD2) | _BV(PD4) | _BV(PD5)); // Encoder and push-button as inputs
    PORTD |= _BV(PD2); // Pull-up for push-button

    // INT0 is used for the push button so it can wake the device up.
    EIMSK |= _BV(INT0); // Enable INT0 for push-button
    EICRA |= _BV(ISC01); // Trigger on falling edge

    // Enable pin change interrupt for both rotary encoder outputs (PD4/PD5)
    // Turning the rotary encoder uses a plain pin change interrupt to save pins.
    PCMSK2 |= _BV(PCINT20) | _BV(PCINT21);
    PCICR |= _BV(PCIE2);
}

void rtenc_bind_incr(EventHandler handler)
{
    _incrementHandler = handler;
}

void rtenc_bind_decr(EventHandler handler)
{
    _decrementHandler = handler;
}

void rtenc_bind_short_press(EventHandler handler)
{
    _shortPressHandler = handler;
}

// Interrupt for rotary encoder button presses
// INT0 is used here so this switch can be used to wake the device up
ISR(INT0_vect)
{
    if (_shortPressHandler) {
        global_eventloop_queue(_shortPressHandler);
    }
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

    encoder_seq <<= 2; // Shift last state
    encoder_seq |= 0b11 & (PIND >> 4); // Copy PIND state (bits 4 and 5) to bits 0 and 1

    // Check if the sequence of the the last four updates indicates a turn
    // The bits in each sequence indicate the state of PD5 and PD4 over time (LSBs are newer)
    switch (encoder_seq) {
        case 0b10000111:
            global_eventloop_queue(_decrementHandler);
            break;

        case 0b01001011:
            global_eventloop_queue(_incrementHandler);
            break;
    }
}
