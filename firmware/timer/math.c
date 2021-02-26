#include "math.h"

#include <stdbool.h>
#include <stdlib.h>

static const __flash uint8_t month_days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

uint16_t rtc_daysSinceJan2000(const RtcTime* inTime)
{
    // Calculate the number of days since Jan 1 2000
    // (Leap years are simple thanks to the century we're in)
    uint16_t days = (inTime->year * ((uint16_t)365)) + (inTime->year/4) + 1;

    // On leap years, subtract the leap day if Feb 29 hasn't been yet
    if ((inTime->year % 4) == 0 && inTime->month < 3) {
        --days;
    }

    // Add days leading up to this month
    for (uint8_t i = 0; i < (inTime->month - 1); ++i) {
        days += month_days[i];
    }

    // Add days of the current month
    days += inTime->day - 1;

    return days;
}

int16_t rtc_minutesBetween(const RtcTime* timeA, const RtcTime* timeB)
{
    int16_t minutes = 0;

    // Add date difference to minutes
    {
        static const uint16_t kMinutesInDay = 1440;

        // Maximum number of calendar days to allow diff between
        // (Note the actual diff maxes out at this plus 23 hours 59 minutes)
        static const uint8_t kMaxDays = 20;

        // Get dates in a diff-able format
        uint16_t daysA = rtc_daysSinceJan2000(timeA);
        uint16_t daysB = rtc_daysSinceJan2000(timeB);

        // Ensure the date delta is in a range we can represent a signed minutes (16-bit)
        // This has to be checked using unsigned numbers as the number of days in 99 years
        // is more than half of a 16-bit value.
        uint16_t daysBetween = 0;
        bool daysNegative = false;

        if (daysA > daysB) {
            daysBetween = daysA - daysB;
        } else {
            daysBetween = daysB - daysA;
            daysNegative = true;
        }

        if (daysBetween > kMaxDays) {
            return kMinutesBetween_DiffTooLarge;
        }

        minutes = daysBetween * kMinutesInDay;
        if (daysNegative) {
            minutes *= -1;
        }
    }

    // Account for time difference
    minutes += (timeA->hour - timeB->hour) * 60;
    minutes += (timeA->minute - timeB->minute);

    return minutes;
}