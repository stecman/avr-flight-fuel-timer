#pragma once

#include <stdbool.h>

typedef struct rtenc_state {
	volatile uint8_t position;
	volatile bool button_pressed;
} rtenc_state;

void rtenc_setup(void);
void rtenc_bind(rtenc_state *state);