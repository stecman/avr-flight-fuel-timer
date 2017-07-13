/* Name: main.c
 * Author: Stephen Holdaway <stephen@stecman.co.nz>
 * Date: 25 June, 2016
 * License: MIT
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// Project
#include "control/rotary_encoder.h"
#include "display/menu.h"
#include "macros.h"

// External
#include "u8glib/src/u8g.h"

/**
 * This code was written using an Arduino mini 5V (ATmega 328p)
 * The pin and port numbers used in this code reflect the AVR pinout,
 * not the Arduino pinout. A mapping between these can be found here:
 *
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */

static u8g_t u8g;

static const __flash char mnu_generic_back[] = "Back";

static const __flash char mnu_aircraftcfg_title[] = "Aircraft config";
static const __flash char mnu_aircraftcfg_save[] = "Load / store";
static const __flash char mnu_aircraftcfg_burn_cruise[] = "Fuel burn cruise";
static const __flash char mnu_aircraftcfg_burn_taxi[] = "Fuel burn taxi";
static const __flash char mnu_aircraftcfg_xfeed[] = "Crossfeed avail.";
static const __flash char mnu_aircraftcfg_reserve[] = "Reserve time";
static const __flash char mnu_aircraftcfg_tankrotation[] = "Tank rotation";
static const __flash char mnu_aircraftcfg_tankbalance[] = "Tank auto-balance";

void u8g_setup(void)
{
    // u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
    u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(1,2), PN(2,1), PN(2,0));
}

void onclick(void)
{

}

int main(void)
{
    // Recover from any watchdog timer reset mishaps
    wdt_reset();
    wdt_disable();

    // select minimal prescaler (max system speed)
    CLKPR = 0x80;
    CLKPR = 0x00;

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

    rtenc_state cursor;
    cursor.position = 0;
    rtenc_bind(&cursor);

    uint8_t lastPos;

    for (;;) {
        if (cursor.position == lastPos) {
            continue;
        }

        menu.cursor_pos = cursor.position;
        cursor.button_pressed = false;

        u8g_FirstPage(&u8g);
        do
        {
            menu_draw(&u8g, &menu);
        } while ( u8g_NextPage(&u8g) );

        lastPos = cursor.position;
    }

    return 0;
}
