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

