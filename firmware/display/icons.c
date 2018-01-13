#include "icons.h"

// These small bitmaps are exported as XBM and copied here manually
// This is a little larger than raw 1bpp bitmap data, but u8glib has
// built in support for arbitrary dimensions when using XBM.
const __flash uint8_t icon_plane_5x5[] = {
	0x08, 0x09, 0x1f, 0x09, 0x08
};

const __flash uint8_t icon_settings_3x5[] = {
	0x05, 0x07, 0x02, 0x02, 0x02
};

// const __flash uint8_t icon_fuel_4x5[] = {
// 	0x04, 0x06, 0x0f, 0x09, 0x06
// };