#pragma once

#include "eventloop.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * Configure the DS3231 RTC i/o
 */
void rtc_setup(void);

/**
 * Enable the DS3231's 1Hz square wave output
 * This forces a refresh of the flight timers and calculations every second
 */
void rtc_update_on_tick(bool enable, EventHandler handler);

/**
 * Get the current HH:MM:SS in seconds
 */
uint16_t rtc_read_time(void);
