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

// Project
#include "main.h"
#include "beeper.h"
#include "control/rotary_encoder.h"
#include "display/menu.h"
#include "macros.h"

// External
#include "u8glib/src/u8g.h"

// C
#include <stdbool.h>
#include <stdlib.h>


static const __flash char mnu_generic_back[] = "Back";

static const __flash char mnu_aircraftcfg_title[] = "Aircraft config";
static const __flash char mnu_aircraftcfg_save[] = "Load / store";
static const __flash char mnu_aircraftcfg_burn_cruise[] = "Fuel burn cruise";
static const __flash char mnu_aircraftcfg_burn_taxi[] = "Fuel burn taxi";
static const __flash char mnu_aircraftcfg_xfeed[] = "Crossfeed avail.";
static const __flash char mnu_aircraftcfg_reserve[] = "Reserve time";
static const __flash char mnu_aircraftcfg_tankrotation[] = "Tank rotation";
static const __flash char mnu_aircraftcfg_tankbalance[] = "Tank auto-balance";

// Display memory
static u8g_t u8g;

// Current draw target
static menu_screen* activeMenu;

void u8g_setup(void)
{
    // u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
    u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(1,2), PN(2,1), PN(2,0));
}

void draw_active_menu(void)
{
    u8g_FirstPage(&u8g);
    do
    {
        menu_draw(&u8g, activeMenu);
    } while ( u8g_NextPage(&u8g) );
}

void incrementCursor(void)
{
    if (activeMenu) {
        activeMenu->cursor_pos++;
        eventloop_queue(&globalEventQueue, &draw_active_menu);
    }
}

void decrementCursor(void)
{
    if (activeMenu) {
        activeMenu->cursor_pos--;
        eventloop_queue(&globalEventQueue, &draw_active_menu);
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

    eventloop_init(&globalEventQueue);

    beeper_setup();
    rtenc_setup();
    u8g_setup();

    menu_item items[] = {
        { .title = mnu_generic_back },
        { .title = mnu_aircraftcfg_save },
        { .title = mnu_aircraftcfg_burn_cruise },
        { .title = mnu_aircraftcfg_burn_taxi },
        { .title = mnu_aircraftcfg_xfeed },
        { .title = mnu_aircraftcfg_reserve },
        { .title = mnu_aircraftcfg_tankrotation },
        { .title = mnu_aircraftcfg_tankbalance },
    };

    menu_screen menu = {
        .title = mnu_aircraftcfg_title,
        .items = &items,
        .num_items = COUNT_OF(items)
    };

    activeMenu = &menu;
    eventloop_queue(&globalEventQueue, &draw_active_menu);

    rtenc_bind_incr(&incrementCursor);
    rtenc_bind_decr(&decrementCursor);

    for (;;) {
        eventloop_run(&globalEventQueue);
    }

    return 0;
}
