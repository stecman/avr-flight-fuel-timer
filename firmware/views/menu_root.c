#include "menu_root.h"

#include "beeper.h"
#include "display/display.h"
#include "display/icons.h"
#include "display/menu.h"
#include "macros.h"
#include "system.h"
#include "text.h"
#include "views/menu_shared.h"

#include "views/menu_aircraft_config.h"
#include "views/flight_main.h"

static void openAircraftConfigView(void)
{
    global_viewstack_push(&view_menu_aircraft_config);
}

static void handleStartFlight(void)
{
    global_viewstack_push(&view_flight_main);
}

static menu_screen _menu;

// Populate shared menu item memory
static inline void _populate_menu(void)
{
    const menu_item default_items[] = {
        {
            .title = pstr_fuelload_title,
            .type = kFunctionCall,
        },
        {
            .title = pstr_aircraftcfg_title,
            .onClick = &openAircraftConfigView,
            .type = kFunctionCall,
        },
        {
            .title = pstr_generic_start_flight,
            .onClick = &handleStartFlight,
            .type = kFunctionCall,
        },
    };

    _menu.items = populate_shared_menu(default_items, COUNT_OF(default_items));
    _menu.num_items = COUNT_OF(default_items);
}

static void viewWillMount(void)
{
    // Set up menu memory
    menu_init(&_menu);
    _menu.title = pstr_root_menu_title;
    _menu.icon = &icon_plane;
}

static void viewWillFocus(void)
{
    _populate_menu();
}

static void handleIncrement(void)
{
    menu_move_cursor_relative(&_menu, +1);
    beeper_blip();
    display_mark_dirty();
}

static void handleDecrement(void)
{
    menu_move_cursor_relative(&_menu, -1);
    beeper_blip();
    display_mark_dirty();
}


static void handleShortPress(void)
{
    const menu_item* item = menu_get_current_item(&_menu);

    global_handle_menu_select(&_menu, item);
}

static void render(u8g_t* u8g)
{
    menu_draw(u8g, &_menu);
}

ViewStackFrame view_menu_root = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = &viewWillFocus,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
