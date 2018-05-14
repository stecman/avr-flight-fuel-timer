#include <stdint.h>

enum AircraftConfigFlags {
	// If the aircraft has a crossfeed between fuel tanks
	// When enabled, the fuel tanks are handled as a single total volume.
	kAircraftConfig_hasCrossfeed = 1 << 0,

	// If tank switching times should be adjusted automatically to balance
	// the amount of fuel in each tank
	kAircraftConfig_autoBalanceTanks = 1 << 1,
};

typedef struct AircraftConfig {
	// Name of the config (used to identify configs when saving and loading)
	char name[5];

	// Integer number of litres burned per hour while flight mode is active
	uint8_t fuelBurnCruise;

	// Integer number of litres burned per hour while the engine is running on the ground
	uint8_t fuelBurnTaxi;

	// Integer number of minutes between fuel tank changes
	// If this value is zero, automatic tank rotation is disabled
	uint8_t tankRotationMinutes;

	// Integer number of minutes of required reserve time in tanks
	// This is time that is available, but is not planned for use. In New Zealand, the legal
	// minimum reserve time for day flight is "30 minutes past your intended destination"
	uint8_t reserveTimeMinutes;

	// Bitmask of AircraftConfigFlags values
	uint8_t flags;

} AircraftConfig;

/**
 * Return the aircraft config that is currently loaded
 */
AircraftConfig* config_get_current_aircraft(void);