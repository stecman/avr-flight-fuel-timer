#include "menu_aircraft_config.h"

#include "beeper.h"
#include "display/display.h"
#include "display/icons.h"
#include "display/menu.h"
#include "display/formatters.h"
#include "macros.h"
#include "settings/aircraft.h"
#include "system.h"
#include "text.h"
#include "views/menu_shared.h"

static menu_screen _menu;

static void text_display_cruiseBurn(char* buffer, uint8_t length)
{
    AircraftConfig* config = config_get_current_aircraft();
    text_format_litres_per_hour(config->fuelBurnCruise, buffer, length);
}

static void text_display_taxiBurn(char* buffer, uint8_t length)
{
    AircraftConfig* config = config_get_current_aircraft();
    text_format_litres_per_hour(config->fuelBurnTaxi, buffer, length);
}

static void text_display_crossfeed(char* buffer, uint8_t length)
{
    AircraftConfig* config = config_get_current_aircraft();
    text_format_bool(config->flags & kAircraftConfig_hasCrossfeed, buffer, length);
}

static void text_display_reserveTime(char* buffer, uint8_t length)
{
    AircraftConfig* config = config_get_current_aircraft();
    text_format_minutes(config->reserveTimeMinutes, buffer, length);
}

static void text_display_tankRotation(char* buffer, uint8_t length)
{
    AircraftConfig* config = config_get_current_aircraft();

    if (config->tankRotationMinutes == 0) {
        strcpy_P(buffer, (const __flash char*) pstr_generic_off);
    } else {
        text_format_minutes(config->tankRotationMinutes, buffer, length);
    }
}

static void text_display_tankAutoBalance(char* buffer, uint8_t length)
{
    AircraftConfig* config = config_get_current_aircraft();
    text_format_bool(config->flags & kAircraftConfig_autoBalanceTanks, buffer, length);
}

static void save_and_pop_view(void)
{
    // TODO: Display error if saving to persistent storage fails
    if (config_save_aircraft()) {
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
            .title = pstr_aircraftcfg_save,
            .type = kFunctionCall,
        },
        {
            .title = pstr_aircraftcfg_burn_cruise,
            .type = kValueEditable,
            .getValueAsText = &text_display_cruiseBurn,
        },
        {
            .title = pstr_aircraftcfg_burn_taxi,
            .type = kValueEditable,
            .getValueAsText = &text_display_taxiBurn,
        },
        {
            .title = pstr_aircraftcfg_xfeed,
            .type = kValueEditable,
            .getValueAsText = &text_display_crossfeed,
        },
        {
            .title = pstr_aircraftcfg_reserve,
            .type = kValueEditable,
            .getValueAsText = &text_display_reserveTime,
        },
        {
            .title = pstr_aircraftcfg_tankrotation,
            .type = kValueEditable,
            .getValueAsText = &text_display_tankRotation,
        },
        {
            .title = pstr_aircraftcfg_tankbalance,
            .type = kValueEditable,
            .getValueAsText = &text_display_tankAutoBalance,
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
    _menu.title = pstr_aircraftcfg_title;
    _menu.icon = &icon_settings;
}

static void viewWillFocus(void)
{
    _populate_menu();
}

static void stepSetting(int direction)
{
    AircraftConfig* config = config_get_current_aircraft();

    if ( _menu.cursor_pos == 2 ) {
        config->fuelBurnCruise += direction;

    } else if ( _menu.cursor_pos == 3 ) {
        config->fuelBurnTaxi += direction;

    } else if ( _menu.cursor_pos == 4 ) {
        config->flags ^= kAircraftConfig_hasCrossfeed;

    } else if ( _menu.cursor_pos == 5 ) {
        config->reserveTimeMinutes += direction;

    } else if ( _menu.cursor_pos == 6 ) {
        config->tankRotationMinutes += direction;

    } else if ( _menu.cursor_pos == 7 ) {
        config->flags ^= kAircraftConfig_autoBalanceTanks;
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

ViewStackFrame view_menu_aircraft_config = {
    .frameWillMount = &viewWillMount,
    .frameWillGetFocus = &viewWillFocus,
    .handleIncrement = &handleIncrement,
    .handleDecrement = &handleDecrement,
    .handleShortPress = &handleShortPress,
    .renderer = &render,
};
