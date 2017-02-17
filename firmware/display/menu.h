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
	char* (*format_value)(void);
	void (*handle_click)(void);
} menu_item;

/**
 * A collection of menu items, rendered as a single screen
 */
typedef struct menu_screen {
	const u8g_pgm_uint8_t *title;
	menu_item** items;
	uint8_t num_items;
	uint8_t cursor_pos;
} menu_screen;

/**
 * Allocate and return new menu screen instance
 */
menu_screen* menu_init(
	const __flash char* title,
	menu_item* items[],
	uint8_t num_items
);

menu_item* menu_item_action_init(
	const __flash char* title,
	void (*handle_click)(void)
);

/**
 * Draw the given menu to screen
 */
void menu_draw(u8g_t* u8g, menu_screen* menu);

/**
 * Given a boolean value, return a human readable string
 */
char* menu_val_format_bool(int value);