#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definições de pinos
#define PIN_LED_R 13
#define PIN_LED_B 12
#define PIN_LED_G 11
#define PIN_BUZZER 21

// Definição dos pinos do teclado
#define NUM_COLUNAS 4
#define NUM_LINHAS 4

uint pinos_colunas[NUM_COLUNAS] = {4, 3, 2, 28};
uint pinos_linhas[NUM_LINHAS] = {8, 7, 6, 5};

char MAPA_TECLAS[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

// Máscaras de coluna e estado do teclado
uint mascara_colunas[NUM_COLUNAS];
uint todas_colunas_mascara = 0x0;
char valores_matriz[16];

// Inicializa o teclado matricial
void inicializar_teclado(uint colunas[NUM_COLUNAS], uint linhas[NUM_LINHAS], char valores[16]) {
    for (int i = 0; i < 16; i++) {
        valores_matriz[i] = valores[i];
    }

    for (int col = 0; col < NUM_COLUNAS; col++) {
        gpio_init(colunas[col]);
        gpio_init(linhas[col]);
        gpio_set_dir(colunas[col], GPIO_IN);
        gpio_set_dir(linhas[col], GPIO_OUT);
        gpio_put(linhas[col], 1); // Linhas ativas com nível lógico alto

        todas_colunas_mascara |= (1 << colunas[col]);
        mascara_colunas[col] = 1 << colunas[col];
    }
}

// Captura a tecla pressionada
char obter_tecla(void) {
    int linha;
    uint32_t colunas = gpio_get_all() & todas_colunas_mascara;
    if (colunas == 0x0) {
        return 0;
    }

    for (int l = 0; l < NUM_LINHAS; l++) {
        gpio_put(pinos_linhas[l], 0); // Desativa todas as linhas antes da varredura
    }

    for (linha = 0; linha < NUM_LINHAS; linha++) {
        gpio_put(pinos_linhas[linha], 1);  // Ativa linha
        busy_wait_us(10000); // Atraso para estabilizar
        colunas = gpio_get_all() & todas_colunas_mascara;
        gpio_put(pinos_linhas[linha], 0); // Desativa linha

        if (colunas != 0x0) {
            break;
        }
    }

    for (int l = 0; l < NUM_LINHAS; l++) {
        gpio_put(pinos_linhas[l], 1); // Reativa todas as linhas
    }

    for (int col = 0; col < NUM_COLUNAS; col++) {
        if (colunas == mascara_colunas[col]) {
            return valores_matriz[linha * NUM_COLUNAS + col];
        }
    }
    return 0;
}

// Funções para controlar LEDs
void ativar_led(uint pino) {
    gpio_put(pino, 1);
}

void desativar_led(uint pino) {
    gpio_put(pino, 0);
}

// Função para controlar o Buzzer
void ativar_buzzer(void) {
    gpio_put(PIN_BUZZER, 1);
}

void desativar_buzzer(void) {
    gpio_put(PIN_BUZZER, 0);
}

int main() {
    inicializar_teclado(pinos_colunas, pinos_linhas, MAPA_TECLAS);

    // Configuração dos LEDs e do Buzzer
    gpio_init(PIN_LED_R);
    gpio_set_dir(PIN_LED_R, GPIO_OUT);
    gpio_put(PIN_LED_R, 0);

    gpio_init(PIN_LED_G);
    gpio_set_dir(PIN_LED_G, GPIO_OUT);
    gpio_put(PIN_LED_G, 0);

    gpio_init(PIN_LED_B);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);
    gpio_put(PIN_LED_B, 0);

    gpio_init(PIN_BUZZER);
    gpio_set_dir(PIN_BUZZER, GPIO_OUT);
    gpio_put(PIN_BUZZER, 0);

    char tecla_pressionada;

    while (true) {
        tecla_pressionada = obter_tecla();  // Obtém a tecla pressionada

        // Controle dos LEDs e do Buzzer
        switch (tecla_pressionada) {
            case 'A':
                ativar_led(PIN_LED_R); // Liga o LED vermelho
                break;
            case 'B':
                ativar_led(PIN_LED_B); // Liga o LED azul
                break;
            case 'C':
                ativar_led(PIN_LED_G); // Liga o LED verde
                break;
            case 'D':
                ativar_led(PIN_LED_R); // Liga todos os LEDs
                ativar_led(PIN_LED_B);
                ativar_led(PIN_LED_G);
                break;
            case '#':
                ativar_buzzer(); // Liga o buzzer
                break;
            default:
                // Desliga os LEDs e o buzzer
                desativar_led(PIN_LED_R);
                desativar_led(PIN_LED_B);
                desativar_led(PIN_LED_G);
                desativar_buzzer();
                break;
        }

        sleep_ms(50);  // Delay para evitar leituras rápidas demais
    }

    return 0;
}
