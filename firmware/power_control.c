#include <util/delay.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#include "power_control.h"

void low_clock_delay(short seconds)
{
    clock_prescale_set(clock_div_256); // 62KHz on 328p
    while (seconds > 0) {
        seconds--;
        _delay_ms(1000/256);
    }
    clock_prescale_set(clock_div_1); // return to 16MHz
}
