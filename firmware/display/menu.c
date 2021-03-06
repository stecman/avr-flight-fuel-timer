#include "menu.h"

#include <stdlib.h>
#include <stdbool.h>

#include "display/common.h"
#include "display/defs.h"
#include "display/fonts.h"

static const uint8_t kMaxItemsOnScreen = 5;
static const uint8_t kEdgePadding = 1;

static const uint8_t kMenuDrawOffset = 14;

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
            '\0'
        };

        // Using the 5x8 font, this string has a consistent width of 15
        // (assuming the number of pages is always a single digit)
        u8g_DrawStr(u8g, kScreenWidth - 15, kFontHeight, pageDisplay);
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
            return item->getValueAsText != NULL;
        default:
            break;
    }

    return false;
}

/**
 * Draw a menu item to screen with its title and any associated value
 *
 * This assumes the caller has configured font and colour settings
 */
static void _draw_row(u8g_t* u8g, const menu_item* item, bool focused, bool editing, uint8_t drawOffset)
{
    // Draw selection box across whole line if we're focused
    if (focused && !editing) {
        u8g_DrawBox(u8g, 0, drawOffset + kEdgePadding, kScreenWidth, kFontHeight);
        u8g_SetColorIndex(u8g, 0);
    } else {
        u8g_SetColorIndex(u8g, 1);
    }

    // Draw title
    u8g_DrawStrP(u8g, kEdgePadding, drawOffset + kFontHeight, item->title);

    // Remove any invert
    u8g_SetColorIndex(u8g, 1);

    // Draw value if there is one
    if (_item_has_value(item)) {
        char buffer[7];
        item->getValueAsText(buffer, 7);

        uint8_t width = u8g_GetStrWidth(u8g, buffer);

        if (focused) {
            // Draw selection around value if editing
            if (editing) {
                u8g_DrawBox(
                    u8g,
                    kScreenWidth - width - 2, // Right edge of screen to left edge of value
                    drawOffset + kEdgePadding, // Current menu item's Y position
                    kScreenWidth,
                    kFontHeight
                );
            }

            // Invert the value colour when focused
            u8g_SetColorIndex(u8g, 0);
        }

        u8g_DrawStr(u8g, kScreenWidth - kEdgePadding - width, drawOffset + kFontHeight, buffer);
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
    // Set menu to initial state
    menu->cursor_pos = 0;
    menu->flags = 0;
    menu->icon = NULL;
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
    u8g_SetFont(u8g, font_scientifica);

    display_draw_title(u8g, menu->title, menu->icon);
    _draw_page_indicator(u8g, page, numPages);

    // Render each menu item
    for (uint8_t i = idxStart; i < idxEnd; i++) {
        const menu_item* item = &(menu->items[i]);

        // Position based on font height, offset by the header height
        drawOffset = (drawIndex * (kFontHeight + 1)) + kMenuDrawOffset;
        drawIndex++;

        _draw_row(
            u8g,
            item,
            i == cursorPos /* is focused */,
            (menu->flags & kEditingMenuItem) != 0 /* is editing */,
            drawOffset
        );
    }
}
