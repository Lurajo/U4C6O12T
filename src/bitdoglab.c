#include "lib/bitdoglab.h"

// Transforma a cor desejada em dados para o PIO utilizado
inline unsigned long get_color_data(unsigned long color)
{
    unsigned long red_color = color & 0x00FF0000;
    unsigned long green_color = color & 0x0000FF00;
    unsigned long blue_color = color & 0x000000FF;

    return ((red_color >> 8) | (green_color << 8) | blue_color) << 8;
}

// Escreve dados no PIO utilizado
inline void pio_write_data(PIO pio, unsigned int state_machine, unsigned long color_data)
{
    pio_sm_put_blocking(pio, state_machine, color_data);
}

// Adiciona o design desejado na matriz de LEDs
inline void set_led_matrix_data(unsigned long color, const unsigned char *matrix_design)
{
    unsigned long color_data;

    for (unsigned char index = LED_MATRIX_ITEM_TOTAL - 1; index > 0; index--)
    {
        color_data = matrix_design[index] ? get_color_data(color) : NO_COLOR;
        pio_write_data(LED_MATRIX_PIO, STATE_MACHINE, color_data);
    }
}

// Identifica o design a ser utilizado com base no caractere recebido
inline void set_matrix_design(unsigned char character)
{
    static const unsigned char *matrix_design;

    switch (character)
    {
    case '0':
        matrix_design = zero_matrix;
        break;

    case '1':
        matrix_design = one_matrix;
        break;

    case '2':
        matrix_design = two_matrix;
        break;
    case '3':
        matrix_design = three_matrix;
        break;

    case '4':
        matrix_design = four_matrix;
        break;

    case '5':
        matrix_design = five_matrix;
        break;

    case '6':
        matrix_design = six_matrix;
        break;

    case '7':
        matrix_design = seven_matrix;
        break;

    case '8':
        matrix_design = eight_matrix;
        break;

    case '9':
        matrix_design = nine_matrix;
        break;

    default:
        matrix_design = base_matrix;
        break;
    }

    set_led_matrix_data(DIM_WHITE_COLOR, matrix_design);
}

// Função callback para a interrupção gerada pelos botões A ou B
inline void gpio_irq_callback(unsigned int gpio, unsigned long events)
{
    unsigned long current_event_time = CURRENT_TIME;

    if (current_event_time - last_event_time > DEBOUNCE_TIME)
    {
        switch (gpio)
        {
        case A_BUTTON_PIN:
            rgb_led_pin = GREEN_LED_PIN;
            rgb_led_pin_state = !gpio_get(rgb_led_pin);

            if (rgb_led_pin_state)
            {
                green_string[11] = '1';
            }
            else
            {
                green_string[11] = '0';
            }

            printf("%s\n", green_string);

            break;

        case B_BUTTON_PIN:
            rgb_led_pin = BLUE_LED_PIN;
            rgb_led_pin_state = !gpio_get(rgb_led_pin);

            if (rgb_led_pin_state)
            {
                blue_string[11] = '1';
            }
            else
            {
                blue_string[11] = '0';
            }

            printf("%s\n", blue_string);

            break;

        default:
            rgb_led_pin = RED_LED_PIN;
            rgb_led_pin_state = !gpio_get(rgb_led_pin);

            break;
        }

        gpio_put(rgb_led_pin, rgb_led_pin_state);

        last_event_time = CURRENT_TIME;
    }
}

// Funções do Display OLED

void ssd1306_init(ssd1306_t *ssd, unsigned char width, unsigned char height, bool external_vcc, unsigned char address, i2c_inst_t *i2c)
{
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8U;
    ssd->address = address;
    ssd->i2c_port = i2c;
    ssd->bufsize = ssd->pages * ssd->width + 1;
    ssd->ram_buffer = calloc(ssd->bufsize, sizeof(unsigned char));
    ssd->ram_buffer[0] = 0x40;
    ssd->port_buffer[0] = 0x80;
}

void ssd1306_config(ssd1306_t *ssd)
{
    ssd1306_command(ssd, SET_DISP | 0x00);
    ssd1306_command(ssd, SET_MEM_ADDR);
    ssd1306_command(ssd, 0x01);
    ssd1306_command(ssd, SET_DISP_START_LINE | 0x00);
    ssd1306_command(ssd, SET_SEG_REMAP | 0x01);
    ssd1306_command(ssd, SET_MUX_RATIO);
    ssd1306_command(ssd, OLED_DISPLAY_HEIGHT - 1);
    ssd1306_command(ssd, SET_COM_OUT_DIR | 0x08);
    ssd1306_command(ssd, SET_DISP_OFFSET);
    ssd1306_command(ssd, 0x00);
    ssd1306_command(ssd, SET_COM_PIN_CFG);
    ssd1306_command(ssd, 0x12);
    ssd1306_command(ssd, SET_DISP_CLK_DIV);
    ssd1306_command(ssd, 0x80);
    ssd1306_command(ssd, SET_PRECHARGE);
    ssd1306_command(ssd, 0xF1);
    ssd1306_command(ssd, SET_VCOM_DESEL);
    ssd1306_command(ssd, 0x30);
    ssd1306_command(ssd, SET_CONTRAST);
    ssd1306_command(ssd, 0xFF);
    ssd1306_command(ssd, SET_ENTIRE_ON);
    ssd1306_command(ssd, SET_NORM_INV);
    ssd1306_command(ssd, SET_CHARGE_PUMP);
    ssd1306_command(ssd, 0x14);
    ssd1306_command(ssd, SET_DISP | 0x01);
}

void ssd1306_command(ssd1306_t *ssd, unsigned char command)
{
    ssd->port_buffer[1] = command;
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->port_buffer,
        2,
        false);
}

void ssd1306_send_data(ssd1306_t *ssd)
{
    ssd1306_command(ssd, SET_COL_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->width - 1);
    ssd1306_command(ssd, SET_PAGE_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->pages - 1);
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->ram_buffer,
        ssd->bufsize,
        false);
}

void ssd1306_pixel(ssd1306_t *ssd, unsigned char x, unsigned char y, bool value)
{
    unsigned short index = (y >> 3) + (x << 3) + 1;
    unsigned char pixel = (y & 0b111);
    if (value)
        ssd->ram_buffer[index] |= (1 << pixel);
    else
        ssd->ram_buffer[index] &= ~(1 << pixel);
}

void ssd1306_fill(ssd1306_t *ssd, bool value)
{

    for (unsigned char y = 0; y < ssd->height; ++y)
    {
        for (unsigned char x = 0; x < ssd->width; ++x)
        {
            ssd1306_pixel(ssd, x, y, value);
        }
    }
}

void ssd1306_rect(ssd1306_t *ssd, unsigned char top, unsigned char left, unsigned char width, unsigned char height, bool value, bool fill)
{
    for (unsigned char x = left; x < left + width; ++x)
    {
        ssd1306_pixel(ssd, x, top, value);
        ssd1306_pixel(ssd, x, top + height - 1, value);
    }
    for (unsigned char y = top; y < top + height; ++y)
    {
        ssd1306_pixel(ssd, left, y, value);
        ssd1306_pixel(ssd, left + width - 1, y, value);
    }

    if (fill)
    {
        for (unsigned char x = left + 1; x < left + width - 1; ++x)
        {
            for (unsigned char y = top + 1; y < top + height - 1; ++y)
            {
                ssd1306_pixel(ssd, x, y, value);
            }
        }
    }
}

void ssd1306_line(ssd1306_t *ssd, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bool value)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        ssd1306_pixel(ssd, x0, y0, value);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = err * 2;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void ssd1306_hline(ssd1306_t *ssd, unsigned char x0, unsigned char x1, unsigned char y, bool value)
{
    for (unsigned char x = x0; x <= x1; ++x)
        ssd1306_pixel(ssd, x, y, value);
}

void ssd1306_vline(ssd1306_t *ssd, unsigned char x, unsigned char y0, unsigned char y1, bool value)
{
    for (unsigned char y = y0; y <= y1; ++y)
        ssd1306_pixel(ssd, x, y, value);
}

void ssd1306_draw_char(ssd1306_t *ssd, char c, unsigned char x, unsigned char y)
{
    unsigned short index = 0;
    char ver = c;

    if (c >= ' ' && c <= '~')
    {
        index = (c - ' ') * 8;
    }
    else
    {
        index = 0;
    }

    for (unsigned char i = 0; i < 8; ++i)
    {
        unsigned char line = font_set[index + i];

        for (unsigned char j = 0; j < 8; ++j)
        {
            ssd1306_pixel(ssd, x + i, y + j, line & (1 << j));
        }
    }
}

void ssd1306_draw_string(ssd1306_t *ssd, const char *str, unsigned char x, unsigned char y)
{
    while (*str)
    {
        ssd1306_draw_char(ssd, *str++, x, y);
        x += 8;
        if (x + 8 >= ssd->width)
        {
            x = 0;
            y += 8;
        }
        if (y + 8 >= ssd->height)
        {
            break;
        }
    }
}
