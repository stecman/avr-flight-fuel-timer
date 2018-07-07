#include "settings_date_time.h"

#include <stdlib.h>

#include "beeper.h"
#include "control/clock.h"
#include "display/common.h"
#include "display/display.h"
#include "display/icons.h"
#include "macros.h"
#include "system.h"
#include "text.h"

static void viewWillMount(void)
{

}

static void handleIncrement(void)
{

}

static void handleDecrement(void)
{

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
    display_draw_title(u8g, pstr_date_time_title, &icon_corner_clock);
}

ViewStackFrame view_settings_date_time = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = NULL,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
