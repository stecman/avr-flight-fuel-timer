#pragma once

#include "eventloop.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct RtcTime {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} RtcTime;

/**
 * Configure the DS3231 RTC i/o
 * If the oscillator is flagged as stopped, the time will be reset.
 */
void rtc_setup(void);

/**
 * Enable the DS3231's 1Hz square wave output
 * This forces a refresh of the flight timers and calculations every second
 */
void rtc_update_on_tick(bool enable, EventHandler handler);

/**
 * Get the current HH:MM:SS in seconds
 * Returns true if the output parameter was successfully updated
 */
bool rtc_read_time(RtcTime* output);

/**
 * Set the current date and time on the RTC to the specified time
 * Returns true if the time was successfully set
 */
bool rtc_set_time(RtcTime* newTime);