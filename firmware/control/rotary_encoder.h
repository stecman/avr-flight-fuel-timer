#pragma once

typedef struct rtenc_state {
	volatile uint8_t position;
} rtenc_state;

void rtenc_setup(void);
void rtenc_bind(rtenc_state *state);