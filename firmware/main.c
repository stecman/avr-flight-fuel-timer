// AVR
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>

// Firmware
#include "beeper.h"
#include "control/fram.h"
#include "control/i2c.h"
#include "control/rotary_encoder.h"
#include "control/rtc.h"
#include "control/spi.h"
#include "display/defs.h"
#include "display/display.h"
#include "display/fonts.h"
#include "display/menu.h"
#include "macros.h"
#include "settings/aircraft.h"
#include "system.h"
#include "views/menu_root.h"

// C
#include <stdbool.h>
#include <stdlib.h>

enum InitState {
    kInit_Display = 0,
    kInit_Controls,
    kInit_FRAM,
    kInit_RTC,
    kInit_Settings,
};

static enum InitState _initState = kInit_Display;

static void _renderInitScreen(u8g_t* u8g)
{
    const u8g_pgm_uint8_t* strings[] = {
        U8G_PSTR("Display"),
        U8G_PSTR("Controls"),
        U8G_PSTR("F-RAM"),
        U8G_PSTR("RTC"),
        U8G_PSTR("Settings"),
    };

    const uint8_t kFontHeight = 10;
    const uint8_t kInset = 3;

    u8g_DrawFrame(u8g, 0, 0, 128, 64);

    // Set a font
    u8g_SetFont(u8g, FONT_MAIN);

    for (uint8_t i = 0; i <= _initState; ++i) {
        const uint8_t yPos = ((i + 1) * kFontHeight);

        // Draw system name
        u8g_DrawStrP(u8g, kInset, yPos, strings[i]);

        // Draw status (passed/checking)
        if (i < _initState) {
            u8g_DrawStrP(u8g, 128 - 10 - kInset, yPos, U8G_PSTR("OK"));
        } else {
            u8g_DrawStrP(u8g, 128 - 5 - kInset, yPos, U8G_PSTR("?"));
        }
    }
}

static void _displayBootProgress(enum InitState currentTask)
{
    _initState = currentTask;
    display_set_renderer(_renderInitScreen);
    display_draw();
}

int main(void)
{
    // Recover from the watchdog timer left running
    wdt_reset();
    wdt_disable();

    // select minimal prescaler (max system speed)
    CLKPR = 0x80;
    CLKPR = 0x00;

    // Init event loop before anything can push to it
    global_eventloop_init();

    // Set up peripherals
    spi_init();
    i2c_init();

    // Configure display so the boot progress can be displayed
    display_setup();

    _displayBootProgress(kInit_Controls);
    beeper_setup();
    rtenc_setup();

    _displayBootProgress(kInit_FRAM);
    // fram_init();

    _displayBootProgress(kInit_RTC);
    // rtc_setup();

    _displayBootProgress(kInit_Settings);
    config_restore_aircraft();

    // Set the root view
    global_viewstack_init(&view_menu_root);

    // Start handling interrupts
    sei();

    for (;;) {
        // Process any pending events
        global_eventloop_run();

        // Update the display if needed
        display_draw();
    }

    return 0;
}
