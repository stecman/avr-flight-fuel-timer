#pragma once

// Configure pins for beeper
void beeper_setup(void);

// Make a short click-like sound
void beeper_blip(void);

// Make a longer sound (warning/stop tone)
void beeper_beep_long(void);
