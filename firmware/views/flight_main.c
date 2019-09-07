#include "flight_main.h"

#include <stdlib.h>

#include "beeper.h"
#include "control/rtc.h"
#include "display/display.h"
#include "display/fonts.h"
#include "macros.h"
#include "system.h"
#include "text.h"

static const uint8_t kScreenWidth = 128;
static const uint8_t kScreenHeight = 64;

static const uint8_t kMargin = 10;

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

static char* appendValue(uint8_t value, char* output, char divider)
{
    if (value < 10) {
        output[0] = '0';
        utoa(value, output + 1, 10);
    } else {
        utoa(value, output, 10);
    }

    output[2] = divider;

    return output + 3;
}

static void renderModeTag(u8g_t* u8g, const u8g_pgm_uint8_t* str, const uint8_t length)
{
    static const uint8_t kTagWidth = 9;
    static const uint8_t kFontHeight = 8;

    u8g_DrawBox(u8g,
        kScreenWidth - 1 - kTagWidth,
        kMargin,
        kTagWidth,
        kScreenHeight - kMargin * 2
    );

    u8g_SetColorIndex(u8g, 0);

    for (uint8_t i = 0; i < length; ++i) {
        u8g_draw_glyph(
            u8g,
            kScreenWidth - kTagWidth + 2,
            (kMargin + kFontHeight + 4) + i * (kFontHeight + 1),
            u8g_pgm_read(str + i)
        );
    }

    u8g_SetColorIndex(u8g, 1);
}

const __flash u8g_pgm_uint8_t* MONTHS[] = {
    pstr_month_jan,
    pstr_month_feb,
    pstr_month_mar,
    pstr_month_apr,
    pstr_month_may,
    pstr_month_jun,
    pstr_month_jul,
    pstr_month_aug,
    pstr_month_sep,
    pstr_month_oct,
    pstr_month_nov,
    pstr_month_dec,
};

static void render(u8g_t* u8g)
{
    u8g_SetFont(u8g, font_scientifica);

    // Print mode in top left corner
    u8g_DrawStrP(u8g, 0, 8, pstr_flight_remaining);

    // Draw main display border
    u8g_DrawHLine(u8g, 0, kMargin, kScreenWidth - 1);
    u8g_DrawHLine(u8g, 0, kScreenHeight - kMargin, kScreenWidth - 1);
    u8g_DrawVLine(u8g, 0, kMargin, kScreenHeight - kMargin * 2);
    u8g_DrawVLine(u8g, kScreenWidth - 1, kMargin, kScreenHeight - kMargin * 2);

    // Draw state tag
    renderModeTag(u8g, pstr_flight_tag_safe, len_pstr_flight_tag_safe);

    // Print time
    char timeStr[9];
    {
        char* offset = timeStr;
        offset = appendValue(_time.hour, offset, ':');
        offset = appendValue(_time.minute, offset, ':');
        offset = appendValue(_time.second, offset, '\0');
    }

    uint8_t xpos = 3;
    uint8_t ypos = kScreenHeight - kMargin * 2 - 2;

    static const uint8_t scale = 2;

    u8g_SetFont(u8g, font_ctrld);
    xpos += u8g_draw_glyph_scaled(u8g, xpos, ypos, timeStr[0], scale);
    xpos += u8g_draw_glyph_scaled(u8g, xpos, ypos, timeStr[1], scale);

    u8g_SetFont(u8g, font_scientifica);
    xpos += u8g_draw_glyph(u8g, xpos, ypos, 'H');
    xpos += 3;

    u8g_SetFont(u8g, font_ctrld);
    xpos += u8g_draw_glyph_scaled(u8g, xpos, ypos, timeStr[3], scale);
    xpos += u8g_draw_glyph_scaled(u8g, xpos, ypos, timeStr[4], scale);

    u8g_SetFont(u8g, font_scientifica);
    xpos += u8g_draw_glyph(u8g, xpos, ypos, 'M');
    xpos += 3;

    u8g_SetFont(u8g, font_ctrld);
    xpos += u8g_draw_glyph_scaled(u8g, xpos, ypos, timeStr[6], scale);
    xpos += u8g_draw_glyph_scaled(u8g, xpos, ypos, timeStr[7], scale);

    // u8g_SetFont(u8g, font_scientifica);
    // u8g_draw_glyph(u8g, xpos, ypos, 'S');

    // Draw date in corner
    {
        u8g_SetFont(u8g, font_scientifica);

        char dateStr[5];
        uint8_t xpos = 0;

        // Draw day
        appendValue(_time.day, dateStr, '\0');
        xpos += u8g_DrawStr(u8g, xpos, 64, dateStr);
        xpos += 5;

        // Draw month name
        xpos += u8g_DrawStrP(u8g, xpos, 64, MONTHS[_time.month - 1]);
        xpos += 5;

        // Draw year
        dateStr[0] = '2';
        dateStr[1] = '0';
        appendValue(_time.year, dateStr + 2, '\0');
        u8g_DrawStr(u8g, xpos, 64, dateStr);
    }
}

ViewStackFrame view_flight_main = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = NULL,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
