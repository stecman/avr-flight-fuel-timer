#pragma once

#include <stdint.h>
#include <stddef.h>

#include "u8glib/src/u8g.h"

#define MENU_ACTION_NONE 0x0
#define MENU_ACTION_EDIT 0x1
#define MENU_ACTION_CALL 0x2

/**
 * A selectable item in a menu
 */
typedef struct menu_item {
	const u8g_pgm_uint8_t *title;
	const char* (*format_value)(void);
	const void (*handle_click)(void);
} menu_item;

/**
 * A collection of menu items, rendered as a single screen
 */
typedef struct menu_screen {
	const u8g_pgm_uint8_t *title;
	const menu_item* items;
	uint8_t num_items;
	uint8_t cursor_pos;
} menu_screen;

/**
 * Draw the given menu to screen
 */
void menu_draw(u8g_t* u8g, const menu_screen* menu);

/**
 * Return a human readable string for a boolean
 */
char* menu_val_format_bool(int value);
