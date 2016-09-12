#pragma once

struct rtenc_state {
	uint8_t position;
	uint8_t factor;
};

void rtenc_setup();
void rtenc_bind(struct rtenc_state *state);