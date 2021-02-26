#include "munit/munit.h"

#include "test_math.h"
#include "test_utils.h"

int main (int argc, char* const argv[])
{
    // Sub-suites
    MunitSuite suites[] = {
        suite_math,
        END_SUITES_ARRAY()
    };

    // Top-level suite
    MunitSuite top_level_suite = {
        .prefix = NULL,
        .tests = NULL,
        .suites = suites,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE,
    };

    return munit_suite_main(&top_level_suite, NULL, argc, argv);
}