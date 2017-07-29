#pragma once

// Firmware
#include "global_eventloop.h"

/**
 * Configure interrupts and pins necessary for the encoder to work
 */
void rtenc_setup(void);

/**
 * Set an EventHandler to push to the event loop when the encoder is incremented
 */
void rtenc_bind_incr(EventHandler handler);

/**
 * Set an EventHandler to push to the event loop when the encoder is decremented
 */
void rtenc_bind_decr(EventHandler handler);
