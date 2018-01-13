#include "menu_aircraft_config.h"

#include "beeper.h"
#include "display/display.h"
#include "display/icons.h"
#include "display/menu.h"
#include "macros.h"
#include "system.h"
#include "text.h"
#include "views/menu_shared.h"

static menu_screen _menu;

// Populate shared menu item memory
static inline void _populate_menu(void)
{
    const menu_item default_items[] = {
        {
            .title = pstr_generic_back,
            .type  = kFunctionCall,
            .onClick = &global_viewstack_pop_silent,
        },
        {
            .title = pstr_aircraftcfg_save,
            .type = kFunctionCall,
        },
        {
            .title = pstr_aircraftcfg_burn_cruise,
            .type = kFunctionCall,
        },
        {
            .title = pstr_aircraftcfg_burn_taxi,
            .type = kValueEditable,
        },
        {
            .title = pstr_aircraftcfg_xfeed,
            .type = kValueEditable,
        },
        {
            .title = pstr_aircraftcfg_reserve,
            .type = kValueEditable,
        },
        {
            .title = pstr_aircraftcfg_tankrotation,
            .type = kValueEditable,
        },
        {
            .title = pstr_aircraftcfg_tankbalance,
            .type = kValueEditableToggle,
        },
    };

    _menu.items = populate_shared_menu(default_items, COUNT_OF(default_items));
    _menu.num_items = COUNT_OF(default_items);
}

static void viewWillMount(void)
{
    // Set up menu memory
    menu_init(&_menu);
    _menu.title = pstr_aircraftcfg_title;
    _menu.icon_xbm = icon_settings_3x5;
    _menu.icon_dimensions = icon_dimension_settings;
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

    global_handle_menu_select(item);
}

static void render(u8g_t* u8g)
{
    menu_draw(u8g, &_menu);
}

ViewStackFrame view_menu_aircraft_config = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = &viewWillFocus,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
