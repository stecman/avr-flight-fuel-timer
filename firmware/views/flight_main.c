#include "flight_main.h"

#include <stdlib.h>

#include "beeper.h"
#include "control/rtc.h"
#include "display/display.h"
#include "macros.h"
#include "system.h"
#include "text.h"

static RtcTime _time = {0, 0, 0};

static void handle1HzTick(void)
{
    rtc_read_time(&_time);
    display_mark_dirty();
}

static void viewWillMount(void)
{
    rtc_read_time(&_time);
    rtc_update_on_tick(true, &handle1HzTick);
}

static void handleIncrement(void)
{
//    display_mark_dirty();
}

static void handleDecrement(void)
{
//    display_mark_dirty();
}


static void handleShortPress(void)
{
    global_viewstack_pop_silent();
}


inline void drawCenterStrP(u8g_t* u8g,
                           uint8_t yPos,
                           uint8_t charWidth,
                           uint8_t strLen,
                           const u8g_pgm_uint8_t *pstr)
{
    u8g_DrawStrP(u8g, 64 - ((strLen * charWidth)/2), yPos, pstr);
}

static char* appendValue(uint8_t value, char* output)
{
    if (value < 10) {
        output[0] = '0';
        utoa(value, output + 1, 10);
    } else {
        utoa(value, output, 10);
    }

    output[2] = ':';

    return output + 3;
}

static void render(u8g_t* u8g)
{
    u8g_SetFont(u8g, u8g_font_8x13Br);
    drawCenterStrP(u8g, 13, 8, len_pstr_flight_armed, pstr_flight_armed);

    u8g_SetFont(u8g, u8g_font_5x8r);
    drawCenterStrP(u8g, 15 + 10, 5, len_pstr_flight_armed_start, pstr_flight_armed_start);
    drawCenterStrP(u8g, 15 + 20, 5, len_pstr_flight_armed_cancel, pstr_flight_armed_cancel);

    // Print time
    char buf[9];
    char* offset = buf;
    offset = appendValue(_time.hours, offset);
    offset = appendValue(_time.minutes, offset);
    offset = appendValue(_time.seconds, offset);
    buf[8] = '\0';

    u8g_SetFont(u8g, u8g_font_blipfest_07n);
    u8g_DrawStr(u8g, 8, 50, buf);
}

ViewStackFrame view_flight_main = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = NULL,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
