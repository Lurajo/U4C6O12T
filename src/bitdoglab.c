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

// Identifica o design a ser utilizado com base no contador global
inline void set_matrix_design(unsigned char counter)
{
    static const unsigned char *matrix_design;

    switch (counter)
    {
    case 0:
        matrix_design = zero_matrix;
        break;

    case 1:
        matrix_design = one_matrix;
        break;

    case 2:
        matrix_design = two_matrix;
        break;
    case 3:
        matrix_design = three_matrix;
        break;

    case 4:
        matrix_design = four_matrix;
        break;

    case 5:
        matrix_design = five_matrix;
        break;

    case 6:
        matrix_design = six_matrix;
        break;

    case 7:
        matrix_design = seven_matrix;
        break;

    case 8:
        matrix_design = eight_matrix;
        break;

    case 9:
        matrix_design = nine_matrix;
        break;

    default:
        break;
    }

    set_led_matrix_data(DIM_WHITE_COLOR, matrix_design);
}

// Função callback para a interrupção gerado pelos botões A ou B
inline void gpio_irq_callback(unsigned int gpio, unsigned long events)
{
    unsigned long current_event_time = CURRENT_TIME;

    if (current_event_time - last_event_time > DEBOUNCE_TIME)
    {
        switch (gpio)
        {
        case A_BUTTON_PIN:
            counter++;

            if (counter > 9)
            {
                counter = 9;
            }

            break;

        case B_BUTTON_PIN:
            counter--;

            if (counter > 9)
            {
                counter = 0;
            }

            break;

        default:
            break;
        }
    }

    set_matrix_design(counter);

    last_event_time = CURRENT_TIME;
}
