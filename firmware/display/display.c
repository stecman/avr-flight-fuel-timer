#include "display.h"

#include "display/st7565.h"
#include "display/ssd1306.h"

// Display context
static u8g_t _u8g;

// The renderer to be used on the next draw
static Renderer _currentRenderer = NULL;

// If the display needs to be redrawn (ie. is dirty)
static bool _shouldDraw = false;


void display_setup(void)
{
    // OLED I2C screen
    // u8g_InitI2C(&_u8g, &onepage_ssd1306_128x64_2x_i2c, U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);

    // OLED SPI screen
    // u8g_InitHWSPI(&_u8g, &u8g_dev_ssd1306_128x64_2x_hw_spi, PN(1,2), PN(2,1), PN(2,0));

    // Custom driver for ST7565 COG LCD
    u8g_Init(&_u8g, &onepage_st7565);
}

void display_set_contrast(uint8_t contrast)
{
    u8g_SetContrast(&_u8g, contrast);
}

void display_set_renderer(Renderer renderer)
{
    _currentRenderer = renderer;
    _shouldDraw = true;
}

void display_mark_dirty(void)
{
    _shouldDraw = true;
}

void display_draw(void)
{
    // Bail out if we've got no renderer
    if (!_currentRenderer || !_shouldDraw) {
        return;
    }

    u8g_FirstPage(&_u8g);
    do
    {
        _currentRenderer(&_u8g);
    } while ( u8g_NextPage(&_u8g) );

    _shouldDraw = false;
}
