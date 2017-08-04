/* Name: main.c
 * Author: Stephen Holdaway <stephen@stecman.co.nz>
 * Date: 25 June, 2016
 * License: MIT
 */

// AVR
#include <avr/io.h>
#include <avr/wdt.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// Firmware
#include "global_eventloop.h"
#include "display/display.h"
#include "beeper.h"
#include "control/rotary_encoder.h"
#include "display/menu.h"
#include "macros.h"

// C
#include <stdbool.h>
#include <stdlib.h>


static const __flash char pstr_generic_back[] = "Back";

// Aircraft config menu
static const __flash char pstr_aircraftcfg_title[] = "Aircraft config";
static const __flash char pstr_aircraftcfg_save[] = "Load / store";
static const __flash char pstr_aircraftcfg_burn_cruise[] = "Fuel burn cruise";
static const __flash char pstr_aircraftcfg_burn_taxi[] = "Fuel burn taxi";
static const __flash char pstr_aircraftcfg_xfeed[] = "Crossfeed avail.";
static const __flash char pstr_aircraftcfg_reserve[] = "Reserve time";
static const __flash char pstr_aircraftcfg_tankrotation[] = "Tank rotation";
static const __flash char pstr_aircraftcfg_tankbalance[] = "Tank auto-balance";

// Current draw target
static menu_screen* activeMenu;

void draw_active_menu(u8g_t* u8g)
{
    menu_draw(u8g, activeMenu);
}

void incrementCursor(void)
{
    if (activeMenu) {
        activeMenu->cursor_pos++;
        display_mark_dirty();
    }
}

void decrementCursor(void)
{
    if (activeMenu) {
        activeMenu->cursor_pos--;
        display_mark_dirty();
    }
}

int main(void)
{
    // Recover from any watchdog timer reset mishaps
    wdt_reset();
    wdt_disable();

    // select minimal prescaler (max system speed)
    CLKPR = 0x80;
    CLKPR = 0x00;

    beeper_setup();
    display_setup();
    global_eventloop_init();
    rtenc_setup();

    menu_item items[] = {
        {
            .title = pstr_generic_back,
            .type  = kFunctionCall,
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

    menu_screen menu = {
        .title = pstr_aircraftcfg_title,
        .items = &items,
        .num_items = COUNT_OF(items)
    };

    activeMenu = &menu;
    display_set_renderer(&draw_active_menu);

    rtenc_bind_incr(&incrementCursor);
    rtenc_bind_decr(&decrementCursor);

    for (;;) {
        // Process any pending events
        global_eventloop_run();

        // Update the display if needed
        display_draw();
    }

    return 0;
}
