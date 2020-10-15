#include "menu_device_config.h"

#include "display/display.h"
#include "display/formatters.h"
#include "display/icons.h"
#include "display/menu.h"
#include "macros.h"
#include "settings/system_config.h"
#include "system.h"
#include "text.h"
#include "views/menu_shared.h"
#include "views/settings_date_time.h"
#include "views/settings_firmware.h"

#include "stdlib.h"

static menu_screen _menu;

static uint8_t _contrast = 34;

static void text_display_contrast(char* buffer, uint8_t length)
{
    itoa(_contrast, buffer, 10);
}

static void text_display_reserveTime(char* buffer, uint8_t length)
{
    SystemConfig* config = config_get_sysconf();
    text_format_minutes(config->flight.reserveTimeMinutes, buffer, length);
}

static void text_display_tankAutoBalance(char* buffer, uint8_t length)
{
    SystemConfig* config = config_get_sysconf();
    text_format_bool(config->flight.flags & kSystemConfig_autoBalanceTanks, buffer, length);
}

static void handleSetDateTime(void)
{
    global_viewstack_push(&view_settings_date_time);
}

static void showFirmwareInfo(void)
{
    global_viewstack_push(&view_settings_firmware);
}

static void save_and_pop_view(void)
{
    // TODO: Display error if saving to persistent storage fails
    if (config_save_sysconf()) {
        global_viewstack_pop_silent();
    }
}

// Populate shared menu item memory
static inline void _populate_menu(void)
{
    const menu_item default_items[] = {
        {
            .title = pstr_generic_back,
            .type  = kFunctionCall,
            .onClick = &save_and_pop_view,
        },
        {
            .title = pstr_date_time_title,
            .onClick = &handleSetDateTime,
            .type = kFunctionCall,
        },
        {
            .title = pstr_display_contrast,
            .type = kValueEditable,
            .getValueAsText = &text_display_contrast,
        },
        {
            .title = pstr_flighttcfg_reserve,
            .type = kValueEditable,
            .getValueAsText = &text_display_reserveTime,
        },
        {
            .title = pstr_flightcfg_tankbalance,
            .type = kValueEditable,
            .getValueAsText = &text_display_tankAutoBalance,
        },
        {
            .title = pstr_firmware_title,
            .onClick = &showFirmwareInfo,
            .type = kFunctionCall,
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
    _menu.title = pstr_device_config_title;
    _menu.icon = &icon_settings;
}

static void viewWillFocus(void)
{
    _populate_menu();
}

static void stepSetting(int direction)
{
    SystemConfig* config = config_get_sysconf();

    if ( _menu.cursor_pos == 2 ) {
        _contrast += direction;

        if (_contrast == 255) {
            _contrast = 0;
        } else if (_contrast == 64) {
            _contrast = 63;
        }

        display_set_contrast(_contrast);

    } else if ( _menu.cursor_pos == 3 ) {
        config->flight.reserveTimeMinutes += direction;

    } else if ( _menu.cursor_pos == 4 ) {
        config->flight.flags ^= kSystemConfig_autoBalanceTanks;

    }
}

static void handleIncrement(void)
{
    if (_menu.flags & kEditingMenuItem) {
        stepSetting(1);
    } else {
        menu_move_cursor_relative(&_menu, +1);
    }

    display_mark_dirty();
}

static void handleDecrement(void)
{
    if (_menu.flags & kEditingMenuItem) {
        stepSetting(-1);
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

ViewStackFrame view_device_config = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = &viewWillFocus,
    .frameLostFocus = NULL,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
