#include "menu.h"

#include <stdlib.h>
#include <stdbool.h>

static const uint8_t kScreenWidth = 128;
static const uint8_t kScreenHeight = 64;
static const uint8_t kMaxItemsOnScreen = 5;
static const uint8_t kEdgePadding = 1;

static const uint8_t kFontHeight = 8;

/**
 * Draw a menu title at the top of the screen
 */
static inline void _draw_title(u8g_t* u8g, const menu_screen* menu)
{
    // Draw title with a separator line underneath
    u8g_DrawStrP(u8g, 0, 8, menu->title);
    u8g_DrawLine(u8g, 0, 11, kScreenWidth, 11);
}

/**
 * Draw a [current page]/[total pages] indicator at the top right of the screen
 *
 * Nothing is drawn there is only one page, or if page count exceeds 9. This
 * is an intentional limiation for now to keep this code simple. It's unlikely
 * that menus with more than 45 options will need to be used.
 */
static inline void _draw_page_indicator(u8g_t* u8g, uint8_t currentPage, uint8_t numPages)
{
    if (numPages > 1 && numPages < 10) {
        char pageText[2];
        itoa(currentPage + 1, pageText, 10);
        char numPagesText[2];
        itoa(numPages, numPagesText, 10);

        // Construct a "[num]/[num]" string
        char pageDisplay[] = {
            pageText[0],
            '/',
            numPagesText[0],
            NULL
        };

        // Using the 5x8 font, this string has a consistent width of 15
        // (assuming the number of pages is always a single digit)
        u8g_DrawStr(u8g, kScreenWidth - 15, 8, pageDisplay);
    }
}

/**
 * Check if a menu item is a type that has an associated value
 */
static bool _item_has_value(const menu_item* item)
{
    switch (item->type) {
        case kValueReadOnly:
        case kValueEditable:
        case kValueEditableToggle:
            return true;
    }

    return false;
}

/**
 * Draw a menu item to screen with its title and any associated value
 *
 * This assumes the caller has configured font and colour settings
 */
static void _draw_row(u8g_t* u8g, const menu_item* item, uint8_t drawOffset)
{
    // Draw title
    u8g_DrawStrP(u8g, kEdgePadding, drawOffset + kFontHeight, item->title);

    // Draw value if there is one
    if (_item_has_value(item)) {
        u8g_DrawStrP(u8g, kScreenWidth - kEdgePadding - 15, drawOffset + kFontHeight, PSTR("64L"));
    }
}

/**
 * Keep the cursor position of a menu within bounds of 0 to menu->num_items
 */
static void _wrap_cursor_pos(menu_screen* menu)
{
    // Bail out there are no items (we can't do anything useful)
    if (menu->num_items == 0) {
        return;
    }

    // Treat 255 as a special case of "value below lower limit"
    if (menu->cursor_pos == 0xFF) {
        menu->cursor_pos = menu->num_items - 1;
    }

    // Wrap upper bound to zero
    else if (menu->cursor_pos > menu->num_items - 1) {
        menu->cursor_pos = 0;
    }
}

void menu_move_cursor_relative(menu_screen* menu, int8_t delta)
{
    menu->cursor_pos += delta;
    _wrap_cursor_pos(menu);
}

void menu_init(menu_screen* menu)
{
    menu->cursor_pos = 0;
}

const menu_item* menu_get_current_item(menu_screen* menu)
{
    if (menu->num_items == 0) {
        return NULL;
    }

    return &((menu->items)[menu->cursor_pos]);
}

void menu_draw(u8g_t* u8g, const menu_screen* menu)
{
    // Keep track of where we're drawing on screen
    uint8_t drawOffset = 0;

    // Local, zero-based index (offset from a point in menu->items)
    uint8_t drawIndex = 0;

    // Figure out what items should show on the current page
    uint8_t cursorPos = menu->cursor_pos;
    uint8_t page = cursorPos / kMaxItemsOnScreen;
    uint8_t idxStart = page * kMaxItemsOnScreen;
    uint8_t idxEnd = idxStart + kMaxItemsOnScreen;

    // Find the number of pages needed to fit num_items (effectively rounded-up integer division)
    uint8_t numPages = ((menu->num_items + (kMaxItemsOnScreen - 1)) / kMaxItemsOnScreen);

    // Keep menu selection within the number of available items
    if (idxEnd >= menu->num_items) {
        idxEnd = menu->num_items;
    }

    // Ensure the menu font is in use
    u8g_SetFont(u8g, u8g_font_5x8r);

    _draw_title(u8g, menu);
    _draw_page_indicator(u8g, page, numPages);

    // Render each menu item
    for (uint8_t i = idxStart; i < idxEnd; i++) {
        const menu_item* item = &(menu->items[i]);

        // Position based on font height, offset by the header height
        drawOffset = (drawIndex * 9) + 17;
        drawIndex++;

        // Draw selected state if this item is selected
        if (i == cursorPos) {
            // Draw box
            u8g_DrawRBox(u8g, 0, drawOffset + kEdgePadding, kScreenWidth, kFontHeight, 0);

            // Draw inverted
            u8g_SetColorIndex(u8g, 0);
            _draw_row(u8g, item, drawOffset);
            u8g_SetColorIndex(u8g, 1);
        } else {
            // Draw regular text
            _draw_row(u8g, item, drawOffset);
        }
    }
}

const u8g_pgm_uint8_t* menu_val_format_bool(uint8_t value)
{
    if (value == 0) {
        return PSTR("No");
    } else {
        return PSTR("Yes");
    }
}
