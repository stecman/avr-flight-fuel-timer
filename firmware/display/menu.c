#include "menu.h"

#include <stdlib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MAX_ITEMS_ON_SCREEN 5

// Scrolling

// Draw title

// Draw menu

menu_screen* menu_init(
    const __flash char* title,
    menu_item* items[],
    uint8_t num_items
) {
    menu_screen* screen = malloc(sizeof(menu_screen));

    screen->title = title;
    screen->items = items;
    screen->num_items = num_items;
    screen->cursor_pos = 0;

    return screen;
}

menu_item* menu_item_action_init(
    const __flash char* title,
    void (*handle_click)(void)
) {
    menu_item* item = malloc(sizeof(menu_item));

    item->title = title;
    item->handle_click = handle_click;

    return item;
}

void menu_draw(u8g_t* u8g, menu_screen* menu)
{
    // Keep track of where we're drawing on screen
    uint8_t drawOffset = 0;
    uint8_t drawIndex = 0;

    // Calculate pagination
    uint8_t cursorPos = (menu->cursor_pos % menu->num_items);
    uint8_t page = cursorPos / MAX_ITEMS_ON_SCREEN;
    uint8_t numPages = (menu->num_items / MAX_ITEMS_ON_SCREEN) + 1;
    uint8_t idxStart = page * MAX_ITEMS_ON_SCREEN;
    uint8_t idxEnd = idxStart + MAX_ITEMS_ON_SCREEN;

    // Limit menu item loop to available items
    if (idxEnd >= menu->num_items) {
        idxEnd = menu->num_items;
    }

    // Render menu header
    u8g_SetFont(u8g, u8g_font_5x8);

    u8g_DrawStrP(u8g, 0, 8, menu->title);
    u8g_DrawLine(u8g, 0, 11, SCREEN_WIDTH, 11);

    if (numPages > 1) {
        // The page numbers are assumed to never be more than one digit
        char pageText[2];
        itoa(page + 1, &pageText, 10);
        char numPagesText[2];
        itoa(numPages, &numPagesText, 10);

        char pageDisplay[] = {
            pageText[0],
            '/',
            numPagesText[0],
            NULL
        };

        // Using the 5x8 font, this string has a consistent width
        u8g_DrawStr(u8g, SCREEN_WIDTH - 15, 8, pageDisplay);
    }

    // Render menu items
    for (uint8_t i = idxStart; i < idxEnd; i++) {
        menu_item* item = menu->items[i];

        drawOffset = (drawIndex * 9) + 17;
        drawIndex++;

        if (i == cursorPos) {
            // Draw selected item
            u8g_DrawRBox(u8g, 0, drawOffset + 1, SCREEN_WIDTH, 8, 0);
            u8g_SetColorIndex(u8g, 0);
            u8g_DrawStrP(u8g, 1, drawOffset + 8, item->title);
            u8g_SetColorIndex(u8g, 1);
        } else {
            u8g_DrawStrP(u8g, 1, drawOffset + 8, item->title);
        }
    }
}

char* menu_val_format_bool(int value)
{
    if (value == 0) {
        return "No";
    } else {
        return "Yes";
    }
}