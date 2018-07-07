#include "fuel_loading.h"

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

static void render(u8g_t* u8g)
{
    display_draw_title(u8g, pstr_fuelload_title, &icon_fuel);
}

ViewStackFrame view_fuel_loading = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = NULL,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
