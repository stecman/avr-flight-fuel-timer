#include "aircraft.h"

#include "control/fram.h"

#include <string.h>

static AircraftConfig _currentConfig;

AircraftConfig* config_get_current_aircraft(void) {
	return &_currentConfig;
}

static void config_init(void)
{
    memset(_currentConfig.name, '\0', sizeof(((AircraftConfig*)0)->name));

    _currentConfig.fuelBurnTaxi = 0;
    _currentConfig.fuelBurnCruise = 0;
    _currentConfig.tankRotationMinutes = 0;
    _currentConfig.reserveTimeMinutes = 40;
    _currentConfig.flags = kAircraftConfig_autoBalanceTanks;
}

bool config_restore_aircraft(void)
{
    const FramOpStatus status = fram_read(0x80, sizeof(AircraftConfig), &_currentConfig);

    // If a valid config doesn't exist, initialise the config to defaults
    if (status != kFram_Success) {
        config_init();
        return false;
    }

    return true;
}

bool config_save_aircraft(void)
{
    return fram_write(0x80, &_currentConfig, sizeof(AircraftConfig)) == kFram_Success;
}