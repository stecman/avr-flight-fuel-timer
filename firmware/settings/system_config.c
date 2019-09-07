#include "system_config.h"

static SystemConfig _config;
static uint8_t _initialised = 0;

static void _set_defaults(SystemConfig* config)
{
    // For the ST7565 LCD at 3.3V, this is a tested good default contrast (range is 0-63)
    config->device.displayContrast = 34;

    config->flight.reserveTimeMinutes = 40;
    config->flight.flags = kSystemConfig_autoBalanceTanks;
}

SystemConfig* config_get_sysconf(void)
{
    if (!_initialised) {
        _initialised = 1;
        _set_defaults(&_config);
    }
    return &_config;
}

bool config_save_sysconf(void)
{
    // TODO: Actually persist
    return true;
}