#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos
const int led_red = 15, led_green = 14, led_blue = 13, buzzer = 21;
const int row_pins[] = {10, 9, 8, 7};
const int col_pins[] = {6, 5, 4, 3};
char keys[4][4] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};

// Função para escanear o teclado matricial
char scanKeypad() {
    for (int i = 0; i < 4; i++) {
        gpio_put(row_pins[i], 1);
        for (int j = 0; j < 4; j++) {
            if (gpio_get(col_pins[j])) {
                gpio_put(row_pins[i], 0);
                return keys[i][j];
            }
        }
        gpio_put(row_pins[i], 0);
    }
    return '\0';
}

// Função para controlar LEDs e buzzer
void controlLights(char key) {
    if (key == 'A') { gpio_put(led_red, 1); sleep_ms(1000); gpio_put(led_red, 0); }
    if (key == 'B') { gpio_put(led_green, 1); sleep_ms(1000); gpio_put(led_green, 0); }
    if (key == 'C') { gpio_put(led_blue, 1); sleep_ms(1000); gpio_put(led_blue, 0); }
    if (key == 'D') { 
        gpio_put(led_red, 1); gpio_put(led_green, 1); gpio_put(led_blue, 1);
        sleep_ms(1000);
        gpio_put(led_red, 0); gpio_put(led_green, 0); gpio_put(led_blue, 0);
    }
    if (key == '#') { gpio_put(buzzer, 1); sleep_ms(1000); gpio_put(buzzer, 0); }
}

// Função principal para Raspberry Pi Pico
int main() {
    stdio_init_all();

    // Configuração dos pinos
    gpio_init(led_red); gpio_set_dir(led_red, GPIO_OUT);
    gpio_init(led_green); gpio_set_dir(led_green, GPIO_OUT);
    gpio_init(led_blue); gpio_set_dir(led_blue, GPIO_OUT);
    gpio_init(buzzer); gpio_set_dir(buzzer, GPIO_OUT);

    for (int i = 0; i < 4; i++) {
        gpio_init(row_pins[i]); gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_init(col_pins[i]); gpio_set_dir(col_pins[i], GPIO_IN);
    }

    while (1) {
        char key = scanKeypad();
        if (key) {
            printf("Tecla pressionada: %c\n", key);
            controlLights(key);
        }
        sleep_ms(200);
    }

    return 0;
}
