#pragma once

#include "timer/types.h"

static const int16_t kMinutesBetween_DiffTooLarge = 0x8000;

 /**
 * Convert an RTC time to the number of days since Jan 1 2000 (Y2K epoch)
 * The returned value can be used to diff RTC times without dealing with calendar math.
 *
 * Assuming a year of 0-99 (2000 - 2099), the return value will be no greater than 36,160.
 * This assumes the RtcTime struct does not include the century bit from the DS3231.
 */
uint16_t rtc_daysSinceJan2000(const RtcTime* inTime);

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
