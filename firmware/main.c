/* Name: main.c
 * Author: Stephen Holdaway <stephen@stecman.co.nz>
 * Date: 25 June, 2016
 * License: MIT
 */

#include "u8glib/src/u8g.h"
#include "power_control.h"
#include "rotary_encoder.h"

#include <avr/io.h>
#include <avr/wdt.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define TWI_SCREEN_ADDR 0x3C
#define TWI_RTC_ADDR 0x50
#define TWI_EERPOM_ADDR 0x68

/**
 * This code was written using an Arduino mini 5V (ATmega 328p)
 * The pin and port numbers used in this code reflect the AVR pinout,
 * not the Arduino pinout. A mapping between these can be found here:
 *
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */

static u8g_t u8g;

volatile struct rtenc_state cursor;

const __flash uint8_t menu_0[] = "I am a horse";
const __flash uint8_t menu_1[] = "I am a dog";
const __flash uint8_t menu_2[] = "I am a frog";
const __flash uint8_t menu_3[] = "I am a log";
const __flash uint8_t menu_4[] = "I don't really know";

const __flash uint8_t* const __flash string_table[] = {
    menu_0,
    menu_1,
    menu_2,
    menu_3,
    menu_4
};

void u8g_setup(void)
{
    u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
}

void draw(uint8_t position)
{
    char str[] = "\"What is you?\" 1.0";
    u8g_DrawStr(&u8g, 1, 8, str);
    u8g_DrawLine(&u8g, 0, 11, 128, 11);

    uint8_t offset = 0;
    for (uint8_t i = 0; i < 5; i++) {
        offset = (i * 9) + 17;

        if (i == (position % 5)) {
            u8g_DrawRBox(&u8g, 0, offset + 1, 128, 8, 0);
            u8g_SetColorIndex(&u8g, 0);
            u8g_DrawStrP(&u8g, 1, offset + 8, string_table[i]);
            u8g_SetColorIndex(&u8g, 1);
        } else {
            u8g_DrawStrP(&u8g, 1, offset + 8, string_table[i]);
        }
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

    cursor.position = 0;
    cursor.factor = 1;

    rtenc_setup();
    rtenc_bind(&cursor);
    u8g_setup();

    uint8_t pos;

    u8g_SetFont(&u8g, u8g_font_5x8);

    for (;;) {
        if (cursor.position == pos) {
            continue;
        }

        pos = cursor.position;

        u8g_FirstPage(&u8g);
        do
        {
            draw(pos);
        } while ( u8g_NextPage(&u8g) );
    }

    return 0;
}
