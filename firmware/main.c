// AVR
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <string.h>
#include <util/delay.h>

// Firmware
#include "beeper.h"
#include "control/rotary_encoder.h"
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

    // Initialise pins and state
    beeper_setup();
    display_setup();
    rtenc_setup();
    global_eventloop_init();

    // Set the root view
    global_viewstack_init(&view_menu_root);

    for (;;) {
        // Process any pending events
        global_eventloop_run();

        // Update the display if needed
        display_draw();
    }

    return 0;
}
