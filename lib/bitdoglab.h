#ifndef _BITDOGLAB_H
#define _BITDOGLAB_H

// Blibiotecas utilizadas
#include "lib/led_matrix.pio.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"

#include <stdio.h>
#include <stdlib.h>

// Níveis de tensão
#define LOW 0
#define HIGH 1

// Total de LEDS na matriz
#define LED_MATRIX_ITEM_TOTAL 25

// Constantes de cores
#define NO_COLOR 0x00000000
#define DIM_WHITE_COLOR 0x00040404

// Pinos utilizados
#define A_BUTTON_PIN 5
#define B_BUTTON_PIN 6
#define LED_MATRIX_PIN 7
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12
#define RED_LED_PIN 13
#define OLED_DISPLAY_SDA_PIN 14
#define OLED_DISPLAY_SCL_PIN 15

// Constantes para PIO
#define STATE_MACHINE 0
#define LED_MATRIX_PIO pio0

// Constantes para Display OLED
#define OLED_I2C_HARDWARE_BLOCK i2c1
#define OLED_DISPLAY_WIDTH 128
#define OLED_DISPLAY_HEIGHT 64
#define OLED_DISPLAY_BAUD_RATE 115200
#define OLED_DISPLAY_I2C_ADDRESS 0x3C

// Constantes de tempo
#define DELAY_TIME 100
#define DEBOUNCE_TIME 250
#define CURRENT_TIME to_ms_since_boot(get_absolute_time())

// Comandos do Display OLED
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

// Estrutura de dados do Display OLED
typedef struct ssd1306
{
    unsigned char width, height, pages, address;
    i2c_inst_t *i2c_port;
    bool external_vcc;
    unsigned char *ram_buffer;
    unsigned int bufsize;
    unsigned char port_buffer[2];
} ssd1306_t;

// Variáveis globais
static unsigned long last_event_time;
static unsigned char character;

static ssd1306_t ssd1306;

// Matriz sem design
static const unsigned char base_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0};

// Matrizes com design de dígitos de 0 a 9
static const unsigned char zero_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0};

static const unsigned char one_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0};

static const unsigned char two_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0};

static const unsigned char three_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0};

static const unsigned char four_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0};

static const unsigned char five_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0};

static const unsigned char six_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0};

static const unsigned char seven_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 0, 0, 0,
    0, 0, 0, 1, 0};

static const unsigned char eight_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0};

static const unsigned char nine_matrix[LED_MATRIX_ITEM_TOTAL] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0};

// Fonte utilizada
static unsigned char font_set[] = {
    0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, // NULL
    0X3E, 0X41, 0X41, 0X49, 0X41, 0X41, 0X3E, 0X00, // 0
    0X00, 0X00, 0X42, 0X7F, 0X40, 0X00, 0X00, 0X00, // 1
    0X30, 0X49, 0X49, 0X49, 0X49, 0X46, 0X00, 0X00, // 2
    0X49, 0X49, 0X49, 0X49, 0X49, 0X49, 0X36, 0X00, // 3
    0X3F, 0X20, 0X20, 0X78, 0X20, 0X20, 0X00, 0X00, // 4
    0X4F, 0X49, 0X49, 0X49, 0X49, 0X30, 0X00, 0X00, // 5
    0X3F, 0X48, 0X48, 0X48, 0X48, 0X48, 0X30, 0X00, // 6
    0X01, 0X01, 0X01, 0X61, 0X31, 0X0D, 0X03, 0X00, // 7
    0X36, 0X49, 0X49, 0X49, 0X49, 0X49, 0X36, 0X00, // 8
    0X06, 0X09, 0X09, 0X09, 0X09, 0X09, 0X7F, 0X00, // 9
    0X78, 0X14, 0X12, 0X11, 0X12, 0X14, 0X78, 0X00, // A
    0X7F, 0X49, 0X49, 0X49, 0X49, 0X49, 0X7F, 0X00, // B
    0X7E, 0X41, 0X41, 0X41, 0X41, 0X41, 0X41, 0X00, // C
    0X7F, 0X41, 0X41, 0X41, 0X41, 0X41, 0X7E, 0X00, // D
    0X7F, 0X49, 0X49, 0X49, 0X49, 0X49, 0X49, 0X00, // E
    0X7F, 0X09, 0X09, 0X09, 0X09, 0X01, 0X01, 0X00, // F
    0X7F, 0X41, 0X41, 0X41, 0X51, 0X51, 0X73, 0X00, // G
    0X7F, 0X08, 0X08, 0X08, 0X08, 0X08, 0X7F, 0X00, // H
    0X00, 0X00, 0X00, 0X7F, 0X00, 0X00, 0X00, 0X00, // I
    0X21, 0X41, 0X41, 0X3F, 0X01, 0X01, 0X01, 0X00, // J
    0X00, 0X7F, 0X08, 0X08, 0X14, 0X22, 0X41, 0X00, // K
    0X7F, 0X40, 0X40, 0X40, 0X40, 0X40, 0X40, 0X00, // L
    0X7F, 0X02, 0X04, 0X08, 0X04, 0X02, 0X7F, 0X00, // M
    0X7F, 0X02, 0X04, 0X08, 0X10, 0X20, 0X7F, 0X00, // N
    0X3E, 0X41, 0X41, 0X41, 0X41, 0X41, 0X3E, 0X00, // O
    0X7F, 0X11, 0X11, 0X11, 0X11, 0X11, 0X0E, 0X00, // P
    0X3E, 0X41, 0X41, 0X49, 0X51, 0X61, 0X7E, 0X00, // Q
    0X7F, 0X11, 0X11, 0X11, 0X31, 0X51, 0X0E, 0X00, // R
    0X46, 0X49, 0X49, 0X49, 0X49, 0X30, 0X00, 0X00, // S
    0X01, 0X01, 0X01, 0X7F, 0X01, 0X01, 0X01, 0X00, // T
    0X3F, 0X40, 0X40, 0X40, 0X40, 0X40, 0X3F, 0X00, // U
    0X0F, 0X10, 0X20, 0X40, 0X20, 0X10, 0X0F, 0X00, // V
    0X7F, 0X20, 0X10, 0X08, 0X10, 0X20, 0X7F, 0X00, // W
    0X00, 0X41, 0X22, 0X14, 0X14, 0X22, 0X41, 0X00, // X
    0X01, 0X02, 0X04, 0X78, 0X04, 0X02, 0X01, 0X00, // Y
    0X41, 0X61, 0X59, 0X45, 0X43, 0X41, 0X00, 0X00, // Z
};

// Declaração das funções básicas

extern unsigned long get_color_data(unsigned long color);
extern void pio_write_data(PIO pio, unsigned int state_machine, unsigned long color_data);
extern void set_led_matrix_data(unsigned long color, const unsigned char *matrix_design);
extern void set_matrix_design(unsigned char character);
extern void gpio_irq_callback(unsigned int gpio, unsigned long events);

// Declaração das funções do Display OLED

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

#endif /* _BITDOGLAB_H */
