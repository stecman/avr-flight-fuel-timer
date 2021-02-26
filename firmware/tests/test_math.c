#include "test_math.h"

#include "timer/math.h"

// Helper to get hours as minutes
#define HOURS_AS_MINS(x) (x*60)

MunitResult test_time_to_days_conversion(const MunitParameter params[], void* user_data_or_fixture)
{
    // Sanity check
    {
        RtcTime t = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 1, .month = 1, .year = 0,
        };

        munit_assert_uint16(rtc_daysSinceJan2000(&t), ==, 0);
    }

    // Random time
    {
        RtcTime t = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 14, .month = 7, .year = 14,
        };

        munit_assert_uint16(rtc_daysSinceJan2000(&t), ==, 5308);
    }

    // Roll-over between regular months
    {
        RtcTime t = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 31, .month = 7, .year = 19,
        };

        munit_assert_uint16(rtc_daysSinceJan2000(&t), ==, 7151);

        t.day = 1;
        t.month = 8;
        munit_assert_uint16(rtc_daysSinceJan2000(&t), ==, 7152);
    }

    // Confirm leap days are accounted for
    {
        RtcTime t = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 28, .month = 2, .year = 24,
        };

        // Before leap day
        munit_assert_uint16(rtc_daysSinceJan2000(&t), ==, 8824);

        // On leap day
        t.day = 29;
        munit_assert_uint16(rtc_daysSinceJan2000(&t), ==, 8825);

        // After leap day
        t.day = 1;
        t.month = 3;
        munit_assert_uint16(rtc_daysSinceJan2000(&t), ==, 8826);
    }

    return MUNIT_OK;
}

MunitResult test_time_diff_limits(const MunitParameter params[], void* user_data_or_fixture)
{
    RtcTime start = {
        .hour = 0, .minute = 0, .second = 0,
        .day = 22, .month = 5, .year = 20,
    };

    // +19 days
    {
        RtcTime end = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 10, .month = 6, .year = 20,
        };

        munit_assert_int16(rtc_minutesBetween(&end, &start), ==, (19 * HOURS_AS_MINS(24)) /* Minutes in 19 days */ );
    }

    // -19 days
    {
        RtcTime end = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 3, .month = 5, .year = 20,
        };

        munit_assert_int16(rtc_minutesBetween(&end, &start), ==, -(19 * HOURS_AS_MINS(24)) /* Minutes in 19 days */ );
    }

    // +20 days, 23 hours, 59 minutes
    {
        RtcTime end = {
            .hour = 23, .minute = 59, .second = 59,
            .day = 11, .month = 6, .year = 20,
        };

        int16_t expectedMinutes = (20 * HOURS_AS_MINS(24)) // Minutes in 20 days
                                + HOURS_AS_MINS(23)       // Minutes in 23 hours
                                + 59;                     // Minutes column

        munit_assert_int16(rtc_minutesBetween(&end, &start), ==, expectedMinutes);
    }

    // +21 days
    {
        RtcTime end = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 12, .month = 6, .year = 20,
        };

        munit_assert_int16(rtc_minutesBetween(&end, &start), ==, kMinutesBetween_DiffTooLarge);
    }

    // -21 days
    {
        RtcTime end = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 1, .month = 5, .year = 20,
        };

        munit_assert_int16(rtc_minutesBetween(&end, &start), ==, kMinutesBetween_DiffTooLarge);
    }

    // +457 days
    {
        RtcTime end = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 22, .month = 8, .year = 21,
        };

        munit_assert_int16(rtc_minutesBetween(&end, &start), ==, kMinutesBetween_DiffTooLarge);
    }

    // -200 days
    {
        RtcTime end = {
            .hour = 0, .minute = 0, .second = 0,
            .day = 4, .month = 11, .year = 19,
        };

        munit_assert_int16(rtc_minutesBetween(&end, &start), ==, kMinutesBetween_DiffTooLarge);
    }

    return MUNIT_OK;
}

// Time delta
MunitResult test_time_diff_same_day(const MunitParameter params[], void* user_data_or_fixture)
{
    RtcTime utc = {
        .hour = 0, .minute = 12, .second = 54,
        .day = 22, .month = 2, .year = 19,
    };

    RtcTime sartime = {
        .hour = 2, .minute = 30, .second = 0,
        .day = 22, .month = 2, .year = 19,
    };

    munit_assert_int16(rtc_minutesBetween(&sartime, &utc), ==, (HOURS_AS_MINS(2) + 18) );

    return MUNIT_OK;
}

MunitResult test_time_diff_next_day(const MunitParameter params[], void* user_data_or_fixture)
{
    RtcTime utc = {
        .hour = 22, .minute = 57, .second = 12,
        .day = 31, .month = 12, .year = 19,
    };

    RtcTime sartime = {
        .hour = 5, .minute = 16, .second = 54,
        .day = 1, .month = 1, .year = 20,
    };

    munit_assert_int16(rtc_minutesBetween(&sartime, &utc), ==, (HOURS_AS_MINS(6) + 19) );

    return MUNIT_OK;
}

MunitResult test_time_diff_expired(const MunitParameter params[], void* user_data_or_fixture)
{
    RtcTime utc = {
        .hour = 22, .minute = 57, .second = 17,
        .day = 9, .month = 10, .year = 18,
    };

    RtcTime sartime = {
        .hour = 20, .minute = 5, .second = 1,
        .day = 9, .month = 10, .year = 18,
    };

    munit_assert_int16(rtc_minutesBetween(&sartime, &utc), ==, -(HOURS_AS_MINS(2) + 52) );

    return MUNIT_OK;
}

MunitResult test_time_diff_same_time(const MunitParameter params[], void* user_data_or_fixture)
{
    RtcTime t = {
        .hour = 22, .minute = 57, .second = 17,
        .day = 9, .month = 10, .year = 18,
    };

    munit_assert_int16(rtc_minutesBetween(&t, &t), ==, 0);

    return MUNIT_OK;
}

MunitTest tests[] = {
    TEST_CASE(test_time_to_days_conversion),
    TEST_CASE(test_time_diff_limits),
    TEST_CASE(test_time_diff_same_day),
    TEST_CASE(test_time_diff_next_day),
    TEST_CASE(test_time_diff_expired),
    TEST_CASE(test_time_diff_same_time),

    END_TESTS_ARRAY()
};

const MunitSuite suite_math = {
    .prefix = "/math",
    .tests = tests,
    .suites = NULL,
    .iterations = 1,
    .options = MUNIT_SUITE_OPTION_NONE,
};