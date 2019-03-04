#include "settings_date_time.h"

#include <stdlib.h>

#include "beeper.h"
#include "control/rtc.h"
#include "display/common.h"
#include "display/display.h"
#include "display/icons.h"
#include "macros.h"
#include "system.h"
#include "text.h"

// Temporary buffer for modifying time value
static RtcTime _time;

// Cursor index
static uint8_t _cursor;

static void viewWillMount(void)
{
    // Read current time and move cursor to first value
    rtc_read_time(&_time);
    _cursor = 0;
}

static uint8_t _clamp(uint8_t value, uint8_t min, uint8_t max)
{
    if (value == 255) {
        return max;
    } else if (value > max) {
        return min;
    } else if (value < min) {
        return max;
    }

    return value;
}

static void changeValue(int8_t delta)
{
    switch (_cursor) {
        case 0: // Hours
            _time.hour = _clamp(_time.hour + delta, 0, 23);
            break;

        case 1: // Minutes
            _time.minute = _clamp(_time.minute + delta, 0, 59);
            break;

        case 2: // Seconds
            _time.second = _clamp(_time.second + delta, 0, 59);
            break;

        case 3: // Day
            _time.day = _clamp(_time.day + delta, 1, 31);
            break;

        case 4: // Month
            _time.month = _clamp(_time.month + delta, 1, 12);
            break;

        case 5: // Year
            _time.year = _clamp(_time.year + delta, 0, 99);
            break;
    }

    display_mark_dirty();
}

static void handleIncrement(void)
{
    changeValue(1);
}

static void handleDecrement(void)
{
    changeValue(-1);
}


static void handleShortPress(void)
{
    if (_cursor < 5) {
        ++_cursor;
        display_mark_dirty();
    } else {
        rtc_set_time(&_time);
        global_viewstack_pop_silent();
    }
}

inline void drawCenterStrP(u8g_t* u8g,
                           uint8_t yPos,
                           uint8_t charWidth,
                           uint8_t strLen,
                           const u8g_pgm_uint8_t *pstr)
{
    u8g_DrawStrP(u8g, 64 - ((strLen * charWidth)/2), yPos, pstr);
}

static char* zeroPad(uint8_t value, char* output)
{
    uint8_t offset = 0;

    if (value < 10) {
        output[0] = '0';
    }

    utoa(value, output + offset, 10);

    return output + 2;
}

static void render(u8g_t* u8g)
{
    display_draw_title(u8g, pstr_date_time_title, &icon_corner_clock);

    char buf[3];

    for (uint8_t i = 0; i < 6; ++i) {

        u8g_SetColorIndex(u8g, 1);

        const uint8_t x = i * 20;
        const uint8_t y = 36;

        if (i == _cursor) {
            u8g_DrawBox(
                u8g,
                x - 1,
                y - 8,
                11,
                8
            );
            u8g_SetColorIndex(u8g, 0);
        }

        zeroPad(((const uint8_t*)&_time)[i], buf);
        u8g_DrawStr(u8g, x, y, buf);
    }
}

ViewStackFrame view_settings_date_time = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = NULL,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
