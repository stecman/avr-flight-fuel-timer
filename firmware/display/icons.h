// C
#include <stdint.h>

// Icon XBM format data
extern const __flash uint8_t icon_plane_5x5[];
extern const __flash uint8_t icon_settings_3x5[];

// Dimensions as a pair of 4-bit numbers (read the individual hex digits as X and Y)
static const uint8_t icon_dimensions_plane = 0x55;
static const uint8_t icon_dimension_settings = 0x35;
// static const uint8_t icon_fuel_dimension = 0x45;