// AVR
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>

// Firmware
#include "beeper.h"
#include "control/fram.h"
#include "control/rotary_encoder.h"
#include "control/rtc.h"
#include "control/spi.h"
#include "display/display.h"
#include "display/menu.h"
#include "macros.h"
#include "system.h"
#include "views/menu_root.h"

// C
#include <stdbool.h>
#include <stdlib.h>

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

    // Set I2C frequency to 400KHz
    TWBR = F_CPU/(2*400000)-8;
    spi_init();

    // Initialise pins and state
    display_setup();
    rtc_setup();

    beeper_setup();
    rtenc_setup();

    fram_init();
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
