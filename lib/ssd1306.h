#ifndef _SSD1306_H
#define _SSD1306_H

#include "hardware/i2c.h"
#include <stdlib.h>

#define OLED_DISPLAY_WIDTH 128
#define OLED_DISPLAY_HEIGHT 64

typedef enum ssd1306_command
{
    SET_CONTRAST = 0x81,
    SET_ENTIRE_ON = 0xA4,
    SET_NORM_INV = 0xA6,
    SET_DISP = 0xAE,
    SET_MEM_ADDR = 0x20,
    SET_COL_ADDR = 0x21,
    SET_PAGE_ADDR = 0x22,
    SET_DISP_START_LINE = 0x40,
    SET_SEG_REMAP = 0xA0,
    SET_MUX_RATIO = 0xA8,
    SET_COM_OUT_DIR = 0xC0,
    SET_DISP_OFFSET = 0xD3,
    SET_COM_PIN_CFG = 0xDA,
    SET_DISP_CLK_DIV = 0xD5,
    SET_PRECHARGE = 0xD9,
    SET_VCOM_DESEL = 0xDB,
    SET_CHARGE_PUMP = 0x8D
} ssd1306_command_t;

typedef struct ssd1306
{
    unsigned char width, height, pages, address;
    i2c_inst_t *i2c_port;
    bool external_vcc;
    unsigned char *ram_buffer;
    unsigned int bufsize;
    unsigned char port_buffer[2];
} ssd1306_t;

void ssd1306_init(ssd1306_t *ssd, unsigned char width, unsigned char height, bool external_vcc, unsigned char address, i2c_inst_t *i2c);
void ssd1306_config(ssd1306_t *ssd);
void ssd1306_command(ssd1306_t *ssd, unsigned char command);
void ssd1306_send_data(ssd1306_t *ssd);
void ssd1306_pixel(ssd1306_t *ssd, unsigned char x, unsigned char y, bool value);
void ssd1306_fill(ssd1306_t *ssd, bool value);
void ssd1306_rect(ssd1306_t *ssd, unsigned char top, unsigned char left, unsigned char width, unsigned char height, bool value, bool fill);
void ssd1306_line(ssd1306_t *ssd, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bool value);
void ssd1306_hline(ssd1306_t *ssd, unsigned char x0, unsigned char x1, unsigned char y, bool value);
void ssd1306_vline(ssd1306_t *ssd, unsigned char x, unsigned char y0, unsigned char y1, bool value);
void ssd1306_draw_char(ssd1306_t *ssd, char c, unsigned char x, unsigned char y);
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, unsigned char x, unsigned char y);

#endif /* _SSD1306_H */
