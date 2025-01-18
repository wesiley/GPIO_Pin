#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"


#define LED_R 13 //LED VERMELHO NO PINO 12
#define LED_B 12 //LED AZUL NO PINO 12
#define LED_G 11 //LED VERDE NO PINO 11
#define BUZZER 21 //BUZZER NO PINO 21


uint columns[4] = {4, 3, 2, 28}; //AS COLUNAS ESTÃO NOS PINOS 4,3,2,1 (1,2,3,4)
uint rows[4] = {8, 7, 6, 5}; //AS LINHAS ESTÃO NOS PINOS 8,7,6,5 (1,2,3,4)

// CHAR CONFORME ESTÁ O TECLADO MATRICIAL
char KEY_MAP[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

uint _columns[4]; // GPIOs conectados às colunas.
uint _rows[4]; // GPIOs conectados às linhas.

//As colunas do teclado estão conectadas aos pinos GPIO 4, 3, 2 e 1.
//As linhas do teclado estão conectadas aos pinos GPIO 8, 7, 6 e 5.

char _matrix_values[16]; 

//Define os valores das 16 teclas do teclado, representando os caracteres que cada tecla retorna quando pressionada. 

uint all_columns_mask = 0x0; // Todos os bits são 0

//A máscara de bits serve para isolar ou verificar apenas esses pinos GPIO específicos que estão conectados às colunas do teclado

uint column_mask[4]; //é uma máscara que representa exclusivamente a coluna 4.

// Inicializa o teclado matricial
void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]) {
    for (int i = 0; i < 16; i++) {
        _matrix_values[i] = matrix_values[i];
    }

    //INICIALIZAÇÃO DA MATRIZ DE TECLAS (O ATÉ 16 (4X4))

    //O loop percorre as 4 colunas e 4 linhas do teclado.

    for (int K = 0; K < 4; K++) {
        _columns[K] = columns[K];
        _rows[K] = rows[K];

        gpio_init(_columns[K]); //Inicializa o pino GPIO especificado.
        gpio_init(_rows[K]); //Inicializa o pino GPIO especificado.

        gpio_set_dir(_columns[K], GPIO_IN); //Define a direção do pino (entrada ou saída)
        gpio_set_dir(_rows[K], GPIO_OUT); //Define a direção do pino (entrada ou saída)

        gpio_put(_rows[K], 1); // Inicializa as linhas com nível lógico alto (1), ou seja, ativa as linhas.

        all_columns_mask += (1 << _columns[K]); //Esta variável cria uma máscara de bits que contém o estado de todas as colunas. Ela é usada para detectar quais colunas estão ativas. 
        column_mask[K] = 1 << _columns[K]; //Aqui, cada coluna tem uma máscara associada individualmente. A máscara é criada da mesma maneira, mas agora a máscara é armazenada no vetor column_mask
    }
}

// Captura o caractere pressionado
char pico_keypad_get_key(void) {
    int row;
    uint32_t cols;

    cols = gpio_get_all(); // captura o estado de todos os pinos GPIO.
    cols = cols & all_columns_mask; // filtra apenas os pinos das colunas, utilizando uma máscara

    //Se o valor de cols for 0x0 (nenhuma coluna ativa), significa que não há tecla pressionada, então a função retorna 0 (indicando nenhum valor válido).

    if (cols == 0x0) {
        return 0;
    }

    //Antes de começar a varredura das linhas, todas as linhas são desativadas para garantir que a verificação seja feita de forma controlada e sem interferência.

    for (int j = 0; j < 4; j++) {
        gpio_put(_rows[j], 0);
    }

    //A função percorre cada linha do teclado matricial e ativa uma linha por vez.

    for (row = 0; row < 4; row++) {
        gpio_put(_rows[row], 1); //ativa a linha corrente.
        busy_wait_us(10000); //Atraso para estabilizar

        cols = gpio_get_all(); //captura novamente o estado de todas as colunas.
        gpio_put(_rows[row], 0);  //Desativa a linha
        cols = cols & all_columns_mask; //o estado das colunas é filtrado novamente usando a máscara 

    //significa que uma tecla foi pressionada naquela linha e coluna, e o loop é interrompido.

        if (cols != 0x0) {
            break;
        }
    }

    //Após a verificação, todas as linhas são reativadas, voltando para o estado original.
    for (int w = 0; w < 4; w++) {
        gpio_put(_rows[w], 1);
    }


    //Identifica a Coluna Pressionada e Retorna o Caractere
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

int main() {
    stdio_init_all();  // Inicializa a comunicação padrão (UART)
    pico_keypad_init(columns, rows, KEY_MAP);  // Inicializa o teclado matricial

    // Configuração dos LEDs e do Buzzer
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 0);  // Desliga o LED vermelho inicialmente

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);  // Desliga o LED verde

    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 0);  // Desliga o LED azul

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_put(BUZZER, 0);  // Desliga o Buzzer inicialmente

    char key_pressed;

    while (true) {
        key_pressed = pico_keypad_get_key();  // Obtém a tecla pressionada

        // Controle dos LEDs e do Buzzer
        switch (key_pressed) {
            case 'A':
                gpio_put(LED_R, 1);  //LIGA O LED VERMELHO
                break;
            case 'B':
                gpio_put(LED_B, 1);  //LIGA O LED AZUL
                break;
            case 'C':
                gpio_put(LED_G, 1);  //LIGA O LED VERDE
                break;
            case 'D':
                gpio_put(LED_R, 1);  //LIGA TODOS OS LEDS
                gpio_put(LED_B, 1);
                gpio_put(LED_G, 1);
                break;
            case '#':
                gpio_put(BUZZER, 1);  //LIGA O BUZZER
                break;
            default:
                //O RESTO
                gpio_put(LED_R, 0);
                gpio_put(LED_B, 0);
                gpio_put(LED_G, 0);
                gpio_put(BUZZER, 0);
                break;
        }

        sleep_ms(50);  //Delay para evitar leituras rápidas demais
    }

    return 0;
}