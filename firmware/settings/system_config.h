#pragma once

#include <stdint.h>
#include <stdbool.h>

enum SystemConfigFlags {
    // If the tank rotation time should be adjusted dynamically to balance the fuel in each tank
    kSystemConfig_autoBalanceTanks = (1 << 0),
};

typedef struct SystemConfig {
    struct device {
        // Display-specific contrast value (not normalised)
        uint8_t displayContrast;
    } device;

    // Preferences that apply to all aircraft configs
    struct flight {
        // Number of minutes of required reserve time in tanks
        // This is time that is available, but is not planned for use. In New Zealand, the legal
        // minimum reserve time for day flight is "30 minutes past your intended destination"
        uint8_t reserveTimeMinutes;

        // Number of minutes before SARTIME the SARTIME reminder alarm will fire
        uint8_t sartimeAlarmDistanceMinutes;

        // Minutes between SARTIME alarm
        uint8_t sartimeAlarmPeriodMinutes;

        // Bitmask of SystemConfigFlags values
        uint8_t flags;
    } flight;
} SystemConfig;

SystemConfig* config_get_sysconf(void);

bool config_save_sysconf(void);