#include "settings_firmware.h"

#include "display/defs.h"
#include "display/icons.h"
#include "system.h"
#include "text.h"

static const uint8_t kEdgePadding = 1;

static void handleShortPress(void)
{
    global_viewstack_pop_silent();
}

static void render(u8g_t* u8g)
{
    u8g_SetColorIndex(u8g, 1);

    u8g_DrawStrP(u8g, kEdgePadding, 9, U8G_PSTR("Fuel Timer by Stecman"));
    u8g_DrawStrP(u8g, kEdgePadding, 9 * 3, pstr_build_version);
    u8g_DrawStrP(u8g, kEdgePadding, 9 * 4, pstr_build_date);
    u8g_DrawStrP(u8g, kEdgePadding, kScreenHeight, U8G_PSTR("(Press to exit)"));
}

ViewStackFrame view_settings_firmware = {
    .frameWillMount = NULL,
    .frameWillGetFocus = NULL,
    .handleIncrement = NULL,
    .handleDecrement = NULL,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
