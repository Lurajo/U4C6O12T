#include "lib/bitdoglab.h"

int main()
{
    // Registro do tempo de incialização
    last_event_time = CURRENT_TIME;

    // Carregamento da programação do PIO e armazenagem do seu local na memória
    unsigned int memory_offset = pio_add_program(LED_MATRIX_PIO, &led_matrix_program);

    // Inicialização dos componentes padrão
    stdio_init_all();

    // Inicialização da matriz de LEDs
    led_matrix_program_init(LED_MATRIX_PIO, STATE_MACHINE, memory_offset, LED_MATRIX_PIN);
    set_led_matrix_data(NO_COLOR, base_matrix);

    // Inicialização dos pinos verde e zauldo LED RGB
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
        tight_loop_contents();
    }

    return 0;
}
