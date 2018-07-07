#pragma once

// External
#include "u8glib/src/u8g.h"

// C
#include <stdbool.h>

typedef void (*Renderer)(u8g_t* u8g);

/**
 * Configure the internal u8g instance for use
 */
void display_setup(void);

/**
 * Set a flag indicating the display should be drawn next call to display_draw()
 */
void display_mark_dirty(void);

/**
 * Draw the screen with the passed function next time display_draw() is called
 * This implicitly marks the screen as dirty
 */
void display_set_renderer(Renderer renderer);

/**
 * Draw the display now using the current renderer
 *
 * This is intended to be called after all events are processed, rather than at the
 * time the screen is being decided. This does nothing if the display is not marked
 * as dirty.
 *
 * @see display_set_renderer()
 */
void display_draw(void);

/**
 * Set the brightness of the display
 */
void display_set_brightness(uint8_t brightness);
