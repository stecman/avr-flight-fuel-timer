#include "common.h"

#include "display/defs.h"

void display_draw_title(u8g_t* u8g, const u8g_pgm_uint8_t* title, const XbmIcon* icon)
{
    // Drawing white on black
    u8g_SetColorIndex(u8g, 1);

    uint8_t titleOffsetX = 0;

    if (icon != NULL) {
        const uint8_t width = icon->dimensions >> 4;
        const uint8_t height = icon->dimensions & 0x0F;

        // Draw icon anchored to the slightly above the baseline of the text
        u8g_DrawXBMP(u8g, 5 - width, (kFontHeight - 1) - height, width, height, icon->data);

        // Enough spacing for the icon to not look like it's part of the title text
        titleOffsetX = 9;
    }

    // Draw title with a separator line for the header underneath
    u8g_DrawStrP(u8g, titleOffsetX, kFontHeight, title);
    u8g_DrawLine(u8g, 0, 11, kScreenWidth, 11);
}