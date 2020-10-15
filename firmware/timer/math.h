#pragma once

#include "timer/types.h"

static const int16_t kMinutesBetween_DiffTooLarge = 0x8000;

/**
 * Return the number of minutes between two times (timeA - timeB)
 *
 * The result is floored (ignoring seconds). Use rtc_remainderSeconds() to calculate
 * the difference in the seconds column if needed.
 *
 * Returns kMinutesBetween_DiffTooLarge if the difference between the two times exceeds 20 days, 23 hours, 59 minutes.
 */
int16_t rtc_minutesBetween(const RtcTime* timeA, const RtcTime* timeB);

/**
 * Calculate the number of minutes remaining of fuel at a given burn rate
 *
 * @param  fuelQuantity - scaled fixed-point value
 * @param  burnRate - litres per hour
 * @return              [description]
 */
uint16_t ft_minutesRemaining(const uint32_t* fuelQuantity, const uint8_t burnRate);
