#include "menu_settings_root.h"

#include "display/display.h"
#include "display/icons.h"
#include "display/menu.h"
#include "macros.h"
#include "system.h"
#include "text.h"
#include "views/menu_shared.h"
#include "views/settings_date_time.h"
#include "views/settings_firmware.h"

static void handleSetDateTime(void)
{
    global_viewstack_push(&view_settings_date_time);
}

static void showFirmwareInfo(void)
{
    global_viewstack_push(&view_settings_firmware);
}

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
            .title = pstr_date_time_title,
            .onClick = &handleSetDateTime,
            .type = kFunctionCall,
        },
        {
            .title = pstr_firmware_title,
            .onClick = &showFirmwareInfo,
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
    _menu.title = pstr_system_settings_title;
    _menu.icon = &icon_corner_clock;
}

static void viewWillFocus(void)
{
    _populate_menu();
}

static void handleIncrement(void)
{
    menu_move_cursor_relative(&_menu, +1);
    display_mark_dirty();
}

static void handleDecrement(void)
{
    menu_move_cursor_relative(&_menu, -1);
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

ViewStackFrame view_settings_root = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = &viewWillFocus,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
