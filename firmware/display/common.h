#pragma once

#include "display/icons.h"

#include "u8glib/src/u8g.h"

/**
 * Draw a page title with icon at the top of the screen
 *
 * @param icon can be set to NULL to skip drawing an icon
 */
void display_draw_title(u8g_t* u8g, const u8g_pgm_uint8_t* title, const __flash XbmIcon* icon);