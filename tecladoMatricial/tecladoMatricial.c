/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define RED 13 //Define GPIO13
#define BLUE 12 //Define GPIO12
#define GREEN 11 //Define GPIO11
#define BUZZER 21 //Define GPI021

//Define a pinagem das colunas e linhas do teclado matricial
uint columns[4] = {4, 3, 2, 28}; 
uint rows[4] = {8, 7, 6, 5};

//Vetor das teclas do teclado matricial
char KEY_MAP[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

uint _columns[4]; // GPIOs conectados às colunas.
uint _rows[4]; // GPIOs conectados às linhas.

char _matrix_values[16]; 

uint all_columns_mask = 0x0; //Máscara das posições dos GPIOs das colunas
uint column_mask[4]; 

int sleep = 100; //Tempo de duração padrão

// Inicializa o teclado matricial
void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]) {
    for (int i = 0; i < 16; i++) {
        _matrix_values[i] = matrix_values[i];
    }

    for (int K = 0; K < 4; K++) {
        _columns[K] = columns[K];
        _rows[K] = rows[K];

        gpio_init(_columns[K]); // Inicializa a coluna correspondente ao pino especificado
        gpio_init(_rows[K]); // Inicializa a linha correspondente ao pino especificado

        gpio_set_dir(_columns[K], GPIO_IN); // Define a coluna correspondente ao pino especificado como entrada
        gpio_set_dir(_rows[K], GPIO_OUT); // Define a linha correspondente ao pino especificado como saída

        gpio_put(_rows[K], true); // Ativa a linha correspondente ao pino especificado

        all_columns_mask += (1 << _columns[K]); // Realiza um deslocamento do bit 1 conforme o número da coluna
        column_mask[K] = 1 << _columns[K]; // Salva a máscara da coluna
    }
}

// Retorna a tecla selecionada
char pico_keypad_get_key(void) {
    int row;
    uint32_t cols;

    cols = gpio_get_all(); // Pega os estados de todos os GPIOs
    cols = cols & all_columns_mask;  // Faz uma operação de AND entre os estados do GPIO e a máscara das colunas

    //Se o valor de cols for 0x0 (nenhuma coluna ativa), significa que não há tecla pressionada, então a função retorna 0.
    if (cols == 0x0) {
        return 0;
    }

    //Antes de começar a varredura das linhas, todas as linhas são desativadas para garantir que a verificação seja feita de forma controlada e sem interferência.
    for (int j = 0; j < 4; j++) {
        gpio_put(_rows[j], 0);
    }

    //Implementa a varredura das linhas, uma por vez
    for (row = 0; row < 4; row++) {
        gpio_put(_rows[row], true); 
        busy_wait_us(1000); 
        cols = gpio_get_all(); 
        gpio_put(_rows[row], false);  
        cols = cols & all_columns_mask; 

        if (cols != 0x0) {
            break;
        }
    }

    // Retorna ao estado padrão
    for (int w = 0; w < 4; w++) {
        gpio_put(_rows[w], 1);
    }

    //Identifica a coluna correspondente a tecla pressionada e retorna o caractere
    if (cols == column_mask[0]) {
        return (char)_matrix_values[row * 4 + 0];
    } else if (cols == column_mask[1]) {
        return (char)_matrix_values[row * 4 + 1];
    } else if (cols == column_mask[2]) {
        return (char)_matrix_values[row * 4 + 2];
    } else if (cols == column_mask[3]) {
        return (char)_matrix_values[row * 4 + 3];
    } else {
        return 0;
    }
}

//Inicializa e Define os pinos 11, 12, 13 e 21 como saídas
void setting_outputs() {
    gpio_init(RED);
    gpio_set_dir(RED, GPIO_OUT);
    gpio_put(RED, false);  

    gpio_init(GREEN);
    gpio_set_dir(GREEN, GPIO_OUT);
    gpio_put(GREEN, false); 

    gpio_init(BLUE);
    gpio_set_dir(BLUE, GPIO_OUT);
    gpio_put(BLUE, false);  

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_put(BUZZER, false); 
}

//Liga ou desliga os LEDs conforme os parâmetros
void blinking(bool rstate,bool bstate,bool gstate) {
    gpio_put(RED, rstate);  
    gpio_put(BLUE, bstate);
    gpio_put(GREEN, gstate);
}

//Liga ou desliga o buzzer
void buzzing(bool state) {
    gpio_put(BUZZER, state);
}

//Realiza um liga-desliga alternado dos LEDs
void interspersed_blinking() {
    gpio_put(RED, true);
    sleep_ms(sleep);
    gpio_put(RED, false);

    gpio_put(BLUE, true);
    sleep_ms(sleep);
    gpio_put(BLUE, false);

    gpio_put(GREEN, true);
    sleep_ms(sleep);
    gpio_put(GREEN, false);
}

//Altera o tempo padrão de duração dos LEDs conforme o fator multiplicativo 'time'
void sleep_time(int time) {
    sleep = time * 100;
}

//Função principal
int main() {
    stdio_init_all();  // Inicializa a biblioteca de entrada e saída padrão

    printf("=== TESTE DO TECLADO MATRICIAL ===\n");
    printf("- 1 a 9: Aumenta o tempo de duracao padrão (100 ms) em N vezes\n"
            "- A: LED Vermelho\n"
            "- B: LED Azul\n"
            "- C: LED Verde\n"
            "- D: Todos os LEDs\n"
            "- #: Buzzer\n"
            "- *: Intercala os LEDs\n"
        "\n\n");

    pico_keypad_init(columns, rows, KEY_MAP);  // Inicializa o teclado matricial

    setting_outputs(); //Configura os pinos de saída

    char key;

    while (true) {
        key = pico_keypad_get_key();

        if (key != '\0') printf("Tecla digitada: %c\n", key);

        if(key == 'A') blinking(true,false,false); // Ativa led vermelho
        else if(key == 'B') blinking(false,true,false); // Ativa led azul
        else if(key == 'C') blinking(false,false,true); // Ativa led verde
        else if(key == 'D') blinking(true,true,true);  // Ativa todos os leds
        else if(key == '#') buzzing(true); // Ativa o buzzer
        else if(key == '*') interspersed_blinking(); // Intercala os leds
        else if (key >= '1' && key <= '9') sleep_time(key - '0'); // Altera o tempo padrão
        else {
            blinking(false,false,false);
            buzzing(false);
        }

        // Caso o usuário não digite nada, ou digite teclas sem função, ou digite as teclas numéricas, ou digite asterico, o programa gerará um sleep padrão de 100ms.
        sleep_ms(((key >= '0' && key <= '9') || key == '\0' || key == '*') ? 100 : sleep); 
    }

    return 0;
}