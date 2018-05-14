#include "aircraft.h"

static AircraftConfig _currentConfig = {
	.fuelBurnTaxi = 15,
	.fuelBurnCruise = 35,
	.tankRotationMinutes = 0,
	.reserveTimeMinutes = 40,
	.flags = kAircraftConfig_autoBalanceTanks,
};

AircraftConfig* config_get_current_aircraft(void) {
	return &_currentConfig;
}