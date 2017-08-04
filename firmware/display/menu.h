#pragma once

#include <stdint.h>
#include <stddef.h>

#include "u8glib/src/u8g.h"

// Flags for how interactions with a menu item should be handled
typedef enum MenuItemType {
    // Do nothing when selected
    kNone = 0,

    // Call the on_click function pointer when selected
    kFunctionCall,

    // Display a value using get_display_value()
    // Enter value editing mode when clicked
    kValueEditable,

    // Display a value using get_display_value()
    // Call on_click when clicked to immediately cycle the value
    kValueEditableToggle,

    // Display a value using get_display_value()
    // Do nothing when clicked
    kValueReadOnly,

} MenuItemType;

/**
 * A selectable item in a menu
 */
typedef struct menu_item {
    const u8g_pgm_uint8_t *title;
    char* (*get_value)(void);
    void (*on_click)(void);
    MenuItemType type;
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
 * Keep the cursor position of a menu within bounds of 0 to menu->num_items
 */
void menu_wrap_cursor_pos(menu_screen* menu);

/**
 * Draw a menu to screen
 *
 * Note that the cursor position is expected to be kept in the bounds of
 * 0 ... [num_items] by the calling code (menu_wrap_cursor_pos() can help)
 */
void menu_draw(u8g_t* u8g, const menu_screen* menu);

/**
 * Return a human readable string for a boolean
 */
const u8g_pgm_uint8_t* menu_val_format_bool(uint8_t value);
