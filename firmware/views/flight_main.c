#include "flight_main.h"

#include <stdlib.h>

#include "beeper.h"
#include "control/clock.h"
#include "display/display.h"
#include "macros.h"
#include "system.h"
#include "text.h"

static uint16_t _time = 1234;

static void handle1HzTick(void)
{
    beeper_blip();
    //_time = rtc_read_time();
    _time ++;
    display_mark_dirty();
}

static void viewWillMount(void)
{
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

static void render(u8g_t* u8g)
{
    u8g_SetFont(u8g, u8g_font_8x13Br);

    drawCenterStrP(u8g, 13, 8, len_pstr_flight_armed, pstr_flight_armed);

    u8g_SetFont(u8g, u8g_font_5x8r);
    drawCenterStrP(u8g, 15 + 10, 5, len_pstr_flight_armed_start, pstr_flight_armed_start);
    drawCenterStrP(u8g, 15 + 20, 5, len_pstr_flight_armed_cancel, pstr_flight_armed_cancel);

    char timeBuf[6];
    utoa(_time, timeBuf, 10);
    u8g_DrawStr(u8g, 0, 15 + 40, timeBuf);
}

ViewStackFrame view_flight_main = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = NULL,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
