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

#include "stdlib.h"

static menu_screen _menu;

static uint8_t _contrast = 34;

static void handleSetDateTime(void)
{
    global_viewstack_push(&view_settings_date_time);
}

static void showFirmwareInfo(void)
{
    global_viewstack_push(&view_settings_firmware);
}

static void text_display_contrast(char* buffer, uint8_t length)
{
    itoa(_contrast, buffer, 10);
}

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
        {
            .title = pstr_contrast,
            .type = kValueEditable,
            .getValueAsText = &text_display_contrast,
        },
    };

    ASSERT_VALID_MENU_SIZE(default_items);

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
    if (_menu.flags & kEditingMenuItem) {
        _contrast++;

        if (_contrast > 63) {
            _contrast = 0;
        }

        display_set_contrast(_contrast);

    } else {
        menu_move_cursor_relative(&_menu, +1);
    }

    display_mark_dirty();
}

static void handleDecrement(void)
{
    if (_menu.flags & kEditingMenuItem) {
        _contrast--;

        if (_contrast > 63) {
            _contrast = 63;
        }

        display_set_contrast(_contrast);

    } else {
        menu_move_cursor_relative(&_menu, -1);
    }

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
