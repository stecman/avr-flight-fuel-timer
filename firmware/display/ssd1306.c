/*

    u8g_dev_ssd1306_128x64.c

    Universal 8bit Graphics Library

    Copyright (c) 2011, olikraus@gmail.com
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list
        of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice, this
        list of conditions and the following disclaimer in the documentation and/or other
        materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "u8glib/src/u8g.h"

#define WIDTH 128
#define HEIGHT 64

static uint8_t u8g_dev_pb_full_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

static const uint8_t init_sequence[] PROGMEM = {
    U8G_ESC_CS(0),      /* disable chip */
    U8G_ESC_ADR(0),     /* instruction mode */
    U8G_ESC_RST(1),     /* do reset low pulse with (1*16)+2 milliseconds */
    U8G_ESC_CS(1),      /* enable chip */

    0x0ae,              /* display off, sleep mode */
    0x0d5, 0x080,       /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
    0x0a8, 0x03f,       /* */

    0x0d3, 0x000,       /*  */

    0x040,              /* start line */

    0x08d, 0x014,       /* [2] charge pump setting (p62): 0x014 enable, 0x010 disable */

    0x020, 0x000,       // Horizontal addressing mode (so entire buffer can be dumped sequentially)
    0x0a1,              /* segment remap a0/a1*/
    0x0c8,              /* c0: scan dir normal, c8: reverse */
    0x0da, 0x012,       /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
    0x081, 0x0cf,       /* [2] set contrast control */
    0x0d9, 0x0f1,       /* [2] pre-charge period 0x022/f1*/
    0x0db, 0x040,       /* vcomh deselect level */

    0x02e,              /* 2012-05-27: Deactivate scroll */
    0x0a4,              /* output ram to display */
    0x0a6,              /* none inverted normal display mode */
    0x0af,              /* display on */

    U8G_ESC_CS(0),      /* disable chip */
    U8G_ESC_END         /* end of sequence */
};

static const uint8_t _ssd1306_128x64_data_start[] PROGMEM = {
    U8G_ESC_ADR(0),    /* instruction mode */
    U8G_ESC_CS(1),     /* enable chip */
    0x10,              /* set upper 4 bit of the col adr to 0 */
    0x00,              /* set lower 4 bit of the col adr to 0  */
    0xb0,              /* set to first page */
    U8G_ESC_END        /* end of sequence */
};

static const uint8_t _ssd13xx_sleep_on[] PROGMEM = {
    U8G_ESC_ADR(0),     /* instruction mode */
    U8G_ESC_CS(1),      /* enable chip */
    0x0ae,              /* display off */
    U8G_ESC_CS(0),      /* disable chip, bugfix 12 nov 2014 */
    U8G_ESC_END         /* end of sequence */
};

static const uint8_t _ssd13xx_sleep_off[] PROGMEM = {
    U8G_ESC_ADR(0),     /* instruction mode */
    U8G_ESC_CS(1),      /* enable chip */
    0x0af,              /* display on */
    U8G_ESC_DLY(50),    /* delay 50 ms */
    U8G_ESC_CS(0),      /* disable chip, bugfix 12 nov 2014 */
    U8G_ESC_END         /* end of sequence */
};

static uint8_t _ssd1306_128x64_2x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
    switch(msg)
    {
        case U8G_DEV_MSG_INIT:
            u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
            u8g_WriteEscSeqP(u8g, dev, init_sequence);
            break;

        case U8G_DEV_MSG_STOP:
            break;

        case U8G_DEV_MSG_PAGE_NEXT:
        {
            u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

            uint8_t* buf = pb->buf;

            // Reset column
            u8g_WriteEscSeqP(u8g, dev, _ssd1306_128x64_data_start);

            // Enter data mode
            u8g_SetAddress(u8g, dev, 1);

            // Write out 8x 128 byte chunks (limited by uint8_t length parameter)
            // 4x 256 byte chunks doesn't work resulted in artifacts on the screen for some reason
            for (uint8_t i = 0; i < 8; ++i) {
                u8g_WriteSequence(u8g, dev, 128, buf);
                buf += 128;
            }

            u8g_SetChipSelect(u8g, dev, 0);

        }
        break;

        case U8G_DEV_MSG_CONTRAST:
        {
            uint8_t command[] = {
                U8G_ESC_ADR(0),
                0x81, *((uint8_t*)arg), // Contrast
                U8G_ESC_END             // End of sequence
            };
            u8g_WriteSequence(u8g, dev, 4, command);
        }
        break;

        case U8G_DEV_MSG_SLEEP_ON:
            u8g_WriteEscSeqP(u8g, dev, _ssd13xx_sleep_on);
            return 1;

        case U8G_DEV_MSG_SLEEP_OFF:
            u8g_WriteEscSeqP(u8g, dev, _ssd13xx_sleep_off);
            return 1;
    }
    return u8g_dev_pb_full_base_fn(u8g, dev, msg, arg);
}

// Device definitions
static uint8_t _ssd1306_128x64_2x_buf[WIDTH*8] U8G_NOCOMMON ;
static u8g_pb_t _ssd1306_128x64_2x_pb = { {64, HEIGHT, 0, 0, 0},  WIDTH, _ssd1306_128x64_2x_buf};

u8g_dev_t onepage_ssd1306_128x64_2x_sw_spi = { _ssd1306_128x64_2x_fn, &_ssd1306_128x64_2x_pb, U8G_COM_SW_SPI };
u8g_dev_t onepage_ssd1306_128x64_2x_hw_spi = { _ssd1306_128x64_2x_fn, &_ssd1306_128x64_2x_pb, U8G_COM_HW_SPI };
u8g_dev_t onepage_ssd1306_128x64_2x_i2c = { _ssd1306_128x64_2x_fn, &_ssd1306_128x64_2x_pb, U8G_COM_SSD_I2C };


// Page buffer handler
// This is a modified version of u8g_pb16v1.c that works with a single page
// instead of a 128 x 32 pixel buffer that requires multiple draw runs.

static void u8g_pb_full_Clear(u8g_pb_t *b)
{
    uint8_t *ptr = b->buf;
    uint8_t *end_ptr = (ptr + 1024);

    do {
        *ptr++ = 0;
    } while( ptr != end_ptr );
}

static void u8g_pb_full_set_pixel(u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y, uint8_t color_index)
{
    register uint8_t mask;
    uint8_t *ptr = b->buf;

    while (y >= 8) {
        ptr += b->width;
        y -= 8;
    }

    mask = 1;
    mask <<= y;
    ptr += x;

    if ( color_index ) {
        *ptr |= mask;
    } else {
        mask ^=0xff;
        *ptr &= mask;
    }
}


static void u8g_pb_full_SetPixel(u8g_pb_t *b, const u8g_dev_arg_pixel_t * const arg_pixel)
{
    if ( arg_pixel->y < b->p.page_y0 ) {
        return;
    }
    if ( arg_pixel->y > b->p.page_y1 ) {
        return;
    }
    if ( arg_pixel->x >= b->width ) {
        return;
    }
    u8g_pb_full_set_pixel(b, arg_pixel->x, arg_pixel->y, arg_pixel->color);
}

static void u8g_pb_full_Set8PixelOpt2(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel)
{
    register uint8_t pixel = arg_pixel->pixel;
    u8g_uint_t dx = 0;
    u8g_uint_t dy = 0;

    switch( arg_pixel->dir )
    {
        case 0: dx++; break;
        case 1: dy++; break;
        case 2: dx--; break;
        case 3: dy--; break;
    }

    do {
        if ( pixel & 128 ) {
            u8g_pb_full_SetPixel(b, arg_pixel);
        }
        arg_pixel->x += dx;
        arg_pixel->y += dy;
        pixel <<= 1;
    } while( pixel != 0  );

}

uint8_t u8g_dev_pb_full_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
    u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
    switch(msg) {
        case U8G_DEV_MSG_SET_8PIXEL:
            if ( u8g_pb_Is8PixelVisible(pb, (u8g_dev_arg_pixel_t *)arg) ) {
                u8g_pb_full_Set8PixelOpt2(pb, (u8g_dev_arg_pixel_t *)arg);
            }
            break;
        case U8G_DEV_MSG_SET_PIXEL:
            u8g_pb_full_SetPixel(pb, (u8g_dev_arg_pixel_t *)arg);
            break;
        case U8G_DEV_MSG_INIT:
            break;
        case U8G_DEV_MSG_STOP:
            break;
        case U8G_DEV_MSG_PAGE_FIRST:
            u8g_pb_full_Clear(pb);
            u8g_page_First(&(pb->p));
            break;
        case U8G_DEV_MSG_PAGE_NEXT:
            if ( u8g_page_Next(&(pb->p)) == 0 ) {
                return 0;
            }
            u8g_pb_full_Clear(pb);
            break;
#ifdef U8G_DEV_MSG_IS_BBX_INTERSECTION
        case U8G_DEV_MSG_IS_BBX_INTERSECTION:
            return u8g_pb_IsIntersection(pb, (u8g_dev_arg_bbx_t *)arg);
#endif
        case U8G_DEV_MSG_GET_PAGE_BOX:
            u8g_pb_GetPageBox(pb, (u8g_box_t *)arg);
            break;
        case U8G_DEV_MSG_GET_WIDTH:
            *((u8g_uint_t *)arg) = pb->width;
            break;
        case U8G_DEV_MSG_GET_HEIGHT:
            *((u8g_uint_t *)arg) = pb->p.total_height;
            break;
        case U8G_DEV_MSG_SET_COLOR_ENTRY:
            break;
        case U8G_DEV_MSG_SET_XY_CB:
            break;
        case U8G_DEV_MSG_GET_MODE:
            return U8G_MODE_BW;
    }

    return 1;
}