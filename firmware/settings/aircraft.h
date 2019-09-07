#pragma once

#include <stdint.h>
#include <stdbool.h>

enum AircraftConfigFlags {
	// If the aircraft has a crossfeed between fuel tanks
	// When enabled, the fuel tanks are handled as a single total volume.
	kAircraftConfig_hasCrossfeed = 1 << 0,
};

typedef struct AircraftConfig {
	// Name of the config (used to identify configs when saving and loading)
	char name[5];

	// Integer number of litres burned per hour while flight mode is active
	uint8_t fuelBurnCruise;

	// Integer number of litres burned per hour while the engine is running on the ground
	uint8_t fuelBurnTaxi;

	// Number of minutes between fuel tank changes when not operating in cross-feed mode
    // If the value is zero, automatic tank rotation is disabled
    uint8_t tankRotationMinutes;

	// Bitmask of AircraftConfigFlags values
	uint8_t flags;

} AircraftConfig;

/**
 * Return the aircraft config that is currently loaded
 */
AircraftConfig* config_get_current_aircraft(void);

// Hacked in save/restore in F-RAM
bool config_restore_aircraft(void);
bool config_save_aircraft(void);