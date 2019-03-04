#include "st7565.h"

#include "u8glib/src/u8g.h"

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// Commands from ST7565 datasheet
#define CMD_DISPLAY_OFF 0b10101110
#define CMD_DISPLAY_ON  0b10101111
#define CMD_RESET 0b11100010
#define CMD_SET_CONTRAST 0b10000001

#define CMD_SET_COLUMN 0b00010000
#define CMD_SET_PAGE 0b10110000

#define CMD_SET_VOLTAGE_REGULATOR_RESISTOR_RATIO 0b00100000
#define CMD_COMMON_OUTPUT_ORDER_NORMAL   0b11001001
#define CMD_COMMON_OUTPUT_ORDER_REVERSED 0b11001000

// Pin assignments
#define PIN_CS PB0
#define PIN_DATA_CMD PB1
#define PIN_RESET PB2
#define PIN_MOSI PB3
#define PIN_SCLK PB5

static uint8_t u8g_dev_pb_full_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/**
 * Select chip for SPI communication
 */
static inline void selectChip(bool enable)
{
    if (enable) {
        // Active low
        PORTB &= ~_BV(PIN_CS);
    } else {
        PORTB |= _BV(PIN_CS);
    }
}

static inline void _init_pins(void)
{
    // Pull reset line low initially to reset LCD controller
    PORTB &= ~_BV(PIN_RESET);

    // Chip select not selected (active low)
    PORTB |= _BV(PIN_CS);

    // All lines as outputs
    DDRB |= _BV(PIN_CS) | _BV(PIN_DATA_CMD) | _BV(PIN_RESET) | _BV(PIN_MOSI) | _BV(PIN_SCLK);

    // Wait for reset
    _delay_us(10);

    // Pull reset line high to end the reset
    // This needs to be done before SPI init as reset uses the SS pin (prevents SPI going into slave mode)
	// See http://www.avrfreaks.net/forum/avr-spi-problem
	PORTB |= _BV(PIN_RESET);

    // Enable SPI as master, set clock rate fck/2
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPOL);
	SPSR = (1<<SPI2X);
}

static inline void send_spi(const uint8_t data)
{
	// Send the data
	SPDR = data;

	// Wait for the transmission to complete
	while (!(SPSR & (1<<SPIF)));
}

static inline void send_data(const uint8_t* data, const uint8_t size)
{
	// Data/Command line goes high for sending data
	PORTB |= _BV(PIN_DATA_CMD);

    for (uint8_t i = 0; i < size; ++i) {
    	send_spi(data[i]);
    }
}

/**
 * Send single byte command
 */
static void send_command(const uint8_t command)
{
    // Data/Command line goes low for sending command bytes
	PORTB &= ~_BV(PIN_DATA_CMD);
	send_spi(command);
}

/**
 * Send a command with n arguments
 */
static void send_command_seq(const __flash uint8_t* commands, const uint8_t size)
{
    // Data/Command line goes low for sending command bytes
	PORTB &= ~_BV(PIN_DATA_CMD);

    for (uint8_t i = 0; i < size; ++i) {
        send_spi(commands[i]);
    }
}

static const __flash uint8_t init_sequence[] = {

    // Booster, voltage regulator and follower circuit ON
    0b00101111,

    // Set voltage regulator internal resistor ratio
    // At 3-5V, this makes the most of the range of 64 contrast steps sane
    (CMD_SET_VOLTAGE_REGULATOR_RESISTOR_RATIO | 2),

    // Set contrast
    CMD_SET_CONTRAST, 34,

    // Set common output mode = reverse to correct horizontally mirrored output
    // (This LCD has columns wired in reverse to our display buffer)
    CMD_COMMON_OUTPUT_ORDER_REVERSED,

    // Start the display
    CMD_DISPLAY_ON,
};

static uint8_t _st7565_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
    switch(msg)
    {
        case U8G_DEV_MSG_INIT:
            _init_pins();

            selectChip(true);

            send_command_seq(init_sequence, sizeof(init_sequence));

            selectChip(false);
            break;

        case U8G_DEV_MSG_STOP:
            break;

        case U8G_DEV_MSG_PAGE_NEXT:
        {
            selectChip(true);

            const u8g_pb_t* pb = (u8g_pb_t*)(dev->dev_mem);
            const uint8_t* buf = pb->buf;

            for (uint8_t page = 0; page < 8; ++page) {
                // Reset column address to 0
                send_command(CMD_SET_COLUMN);
                send_command(0);

                // Set page address
                send_command(CMD_SET_PAGE | page);

                // Send 16 lines (1 page)
                send_data(buf, 128);
                buf += 128;
            }

            selectChip(false);
        }
        break;

        case U8G_DEV_MSG_CONTRAST:
        {
            selectChip(true);

            const uint8_t contrast = *((uint8_t*) arg);

            if (contrast < 64) {
                send_command(CMD_SET_CONTRAST);
                send_command(contrast);
            }

            selectChip(false);
        }
        break;

        case U8G_DEV_MSG_SLEEP_ON:
            //u8g_WriteEscSeqP(u8g, dev, _ssd13xx_sleep_on);
            return 1;

        case U8G_DEV_MSG_SLEEP_OFF:
            //u8g_WriteEscSeqP(u8g, dev, _ssd13xx_sleep_off);
            return 1;
    }
    return u8g_dev_pb_full_base_fn(u8g, dev, msg, arg);
}

// Device definition
static uint8_t _st7565_buf[(128*64)/8] U8G_NOCOMMON;
static u8g_pb_t _st7565_pb = {
    .p = {
        .page_height = 64,
        .total_height = 64,
    },
    .width = 128,
    .buf = _st7565_buf
};

u8g_dev_t onepage_st7565 = { _st7565_fn, &_st7565_pb, U8G_COM_HW_SPI };


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

static inline void u8g_pb_full_set_pixel(u8g_pb_t *b, u8g_uint_t x, u8g_uint_t y, uint8_t color_index)
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
