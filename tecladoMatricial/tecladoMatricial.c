#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definições de pinos
#define LED_R_PIN 13
#define LED_B_PIN 12
#define LED_G_PIN 11
#define BUZZER_PIN 21

// Definição dos pinos do teclado
#define NUM_COLUMNS 4
#define NUM_ROWS 4

uint columns[NUM_COLUMNS] = {4, 3, 2, 28};
uint rows[NUM_ROWS] = {8, 7, 6, 5};

char KEY_MAP[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

// Máscaras de coluna e estado do teclado
uint column_mask[NUM_COLUMNS];
uint all_columns_mask = 0x0;
char _matrix_values[16];

// Inicializa o teclado matricial
void pico_keypad_init(uint columns[NUM_COLUMNS], uint rows[NUM_ROWS], char matrix_values[16]) {
    for (int i = 0; i < 16; i++) {
        _matrix_values[i] = matrix_values[i];
    }

    for (int K = 0; K < NUM_COLUMNS; K++) {
        gpio_init(columns[K]);
        gpio_init(rows[K]);
        gpio_set_dir(columns[K], GPIO_IN);
        gpio_set_dir(rows[K], GPIO_OUT);
        gpio_put(rows[K], 1); // Linhas ativas com nível lógico alto

        all_columns_mask |= (1 << columns[K]);
        column_mask[K] = 1 << columns[K];
    }
}

// Captura a tecla pressionada
char pico_keypad_get_key(void) {
    int row;
    uint32_t cols = gpio_get_all() & all_columns_mask;
    if (cols == 0x0) {
        return 0;
    }

    for (int j = 0; j < NUM_ROWS; j++) {
        gpio_put(rows[j], 0); // Desativa todas as linhas antes da varredura
    }

    for (row = 0; row < NUM_ROWS; row++) {
        gpio_put(rows[row], 1);  // Ativa linha
        busy_wait_us(10000); // Atraso para estabilizar
        cols = gpio_get_all() & all_columns_mask;
        gpio_put(rows[row], 0); // Desativa linha

        if (cols != 0x0) {
            break;
        }
    }

    for (int w = 0; w < NUM_ROWS; w++) {
        gpio_put(rows[w], 1); // Reativa todas as linhas
    }

    for (int col = 0; col < NUM_COLUMNS; col++) {
        if (cols == column_mask[col]) {
            return _matrix_values[row * NUM_COLUMNS + col];
        }
    }
    return 0;
}

// Funções para controlar LEDs
void led_on(uint pin) {
    gpio_put(pin, 1);
}

void led_off(uint pin) {
    gpio_put(pin, 0);
}

// Função para controlar o Buzzer
void buzzer_on(void) {
    gpio_put(BUZZER_PIN, 1);
}

void buzzer_off(void) {
    gpio_put(BUZZER_PIN, 0);
}

int main() {
    stdio_init_all();
    pico_keypad_init(columns, rows, KEY_MAP);

    // Configuração dos LEDs e do Buzzer
    gpio_init(LED_R_PIN);
    gpio_set_dir(LED_R_PIN, GPIO_OUT);
    gpio_put(LED_R_PIN, 0);

    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    gpio_put(LED_G_PIN, 0);

    gpio_init(LED_B_PIN);
    gpio_set_dir(LED_B_PIN, GPIO_OUT);
    gpio_put(LED_B_PIN, 0);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    char key_pressed;

    while (true) {
        key_pressed = pico_keypad_get_key();  // Obtém a tecla pressionada

        // Controle dos LEDs e do Buzzer
        switch (key_pressed) {
            case 'A':
                led_on(LED_R_PIN); // Liga o LED vermelho
                break;
            case 'B':
                led_on(LED_B_PIN); // Liga o LED azul
                break;
            case 'C':
                led_on(LED_G_PIN); // Liga o LED verde
                break;
            case 'D':
                led_on(LED_R_PIN); // Liga todos os LEDs
                led_on(LED_B_PIN);
                led_on(LED_G_PIN);
                break;
            case '#':
                buzzer_on(); // Liga o buzzer
                break;
            default:
                // Desliga os LEDs e o buzzer
                led_off(LED_R_PIN);
                led_off(LED_B_PIN);
                led_off(LED_G_PIN);
                buzzer_off();
                break;
        }

        sleep_ms(50);  // Delay para evitar leituras rápidas demais
    }

    return 0;
}