#ifndef _BITDOGLAB_H
#define _BITDOGLAB_H

// Blibiotecas utilizadas
#include "lib/led_matrix.pio.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include <stdio.h>

// Níveis de tensão
#define LOW 0
#define HIGH 1

// Total de LEDS na matriz
#define LED_MATRIX_ITEM_TOTAL 25

// Constantes de cores
#define NO_COLOR 0x00000000
#define DIM_WHITE_COLOR 0x00080808

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

// Constantes de tempo
#define DELAY_TIME 100
#define DEBOUNCE_TIME 250
#define CURRENT_TIME to_ms_since_boot(get_absolute_time())

// Variáveis globais
static unsigned long last_event_time;
static unsigned char counter = 0;

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

// Declaração das funções utilizadas
extern unsigned long get_color_data(unsigned long color);
extern void pio_write_data(PIO pio, unsigned int state_machine, unsigned long color_data);
extern void set_led_matrix_data(unsigned long color, const unsigned char *matrix_design);
extern void set_matrix_design(unsigned char counter);
extern void gpio_irq_callback(unsigned int gpio, unsigned long events);

#endif /* _BITDOGLAB_H */
