#include "menu_root.h"

#include "beeper.h"
#include "display/display.h"
#include "display/menu.h"
#include "macros.h"
#include "system.h"
#include "text.h"

#include "views/menu_aircraft_config.h"

#define NUM_ITEMS 4

static void openAircraftConfig(void)
{
    global_viewstack_push(&view_menu_aircraft_config);
}

static menu_item _items[NUM_ITEMS];
static menu_screen _menu;

// Set up menu memory
//
// Doing this with memcpy ends up smaller than assigning each index of the menu
// items array to a specific value. This is also slightly more readable.
static void _init(void)
{
    if (_menu.num_items != 0) {
        return;
    }

    const menu_item default_items[] = {
        {
            .title = pstr_generic_back,
            .onClick = &global_viewstack_pop_silent,
            .type = kFunctionCall,
        },
        {
            .title = pstr_fuelload_title,
            .type = kFunctionCall,
        },
        {
            .title = pstr_log_title,
            .type = kFunctionCall,
        },
        {
            .title = pstr_aircraftcfg_title,
            .onClick = &openAircraftConfig,
            .type = kFunctionCall,
        },
    };

    memcpy(_items, default_items, sizeof(default_items));

    _menu.title = pstr_root_menu_title;
    _menu.items = &_items;
    _menu.num_items = NUM_ITEMS;
}

static void viewWillMount(void)
{
    _init();

    menu_init(&_menu);
    beeper_beep_long();
}

static void handleIncrement(void)
{
    _menu.cursor_pos++;
    beeper_blip();
    menu_wrap_cursor_pos(&_menu);
    display_mark_dirty();
}

static void handleDecrement(void)
{
    _menu.cursor_pos--;
    beeper_blip();
    menu_wrap_cursor_pos(&_menu);
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

ViewStackFrame view_menu_root = {
    .frameWillMount = &viewWillMount,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
