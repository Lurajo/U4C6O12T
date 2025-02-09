#include "lib/bitdoglab.h"

int main()
{
    // Registro do tempo de incialização
    last_event_time = CURRENT_TIME;

    // Carregamento da programação do PIO e armazenagem do seu local na memória
    unsigned int memory_offset = pio_add_program(LED_MATRIX_PIO, &led_matrix_program);

    // Inicialização dos componentes padrão
    stdio_init_all();

    // Inicialização da Interface Serial I2C
    i2c_init(OLED_I2C_HARDWARE_BLOCK, OLED_DISPLAY_BAUD_RATE);

    gpio_set_function(OLED_DISPLAY_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_DISPLAY_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(OLED_DISPLAY_SDA_PIN);
    gpio_pull_up(OLED_DISPLAY_SCL_PIN);

    // Inicialização do Display OLED
    ssd1306_init(&ssd1306, OLED_DISPLAY_WIDTH, OLED_DISPLAY_HEIGHT, false, OLED_DISPLAY_I2C_ADDRESS, OLED_I2C_HARDWARE_BLOCK);
    ssd1306_config(&ssd1306);
    ssd1306_send_data(&ssd1306);

    // Inserção de dados no Display OLED
    ssd1306_fill(&ssd1306, false);

    ssd1306_draw_string(&ssd1306, green_string, 0, 10);
    ssd1306_draw_string(&ssd1306, blue_string, 0, 30);
    ssd1306_draw_string(&ssd1306, character_string, 0, 50);

    ssd1306_send_data(&ssd1306);

    // Inicialização da matriz de LEDs
    led_matrix_program_init(LED_MATRIX_PIO, STATE_MACHINE, memory_offset, LED_MATRIX_PIN);
    set_led_matrix_data(NO_COLOR, base_matrix);

    // Inicialização dos pinos do LED
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);

    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);

    // Inicialização dos botões A e B
    gpio_init(A_BUTTON_PIN);
    gpio_set_dir(A_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(A_BUTTON_PIN);

    gpio_init(B_BUTTON_PIN);
    gpio_set_dir(B_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(B_BUTTON_PIN);

    // Inicialização das interrupções dos botões A e B
    gpio_set_irq_enabled_with_callback(A_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_callback);
    gpio_set_irq_enabled_with_callback(B_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_callback);

    // Loop principal
    while (true)
    {
        // Caso conectado via USB, atualize o Display OLED
        if (stdio_usb_connected() && scanf("%c", &character) == 1)
        {
            character_string[11] = character;

            ssd1306_draw_string(&ssd1306, character_string, 0, 50);
            set_matrix_design(character);

            ssd1306_send_data(&ssd1306);
        }
    }

    return 0;
}
