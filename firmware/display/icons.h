#pragma once

#include <stdint.h>

typedef __flash struct _XbmIcon {
	// Dimensions as a pair of 4-bit numbers (read the individual hex digits as X and Y)
	const uint8_t dimensions;

	// 2-colour bitmap data exported as XBM and copied here manually
	// This is a little larger than raw 1bpp bitmap data, but u8glib has
	// built in support for arbitrary dimensions when using XBM.
	const uint8_t data[];
} XbmIcon;

extern const __flash XbmIcon icon_plane;
extern const __flash XbmIcon icon_settings;
extern const __flash XbmIcon icon_fuel;
extern const __flash XbmIcon icon_corner_clock;