#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define RED 13 
#define BLUE 12
#define GREEN 11
#define BUZZER 21

uint columns[4] = {4, 3, 2, 28}; 
uint rows[4] = {8, 7, 6, 5};

char KEY_MAP[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

uint _columns[4]; 
uint _rows[4]; 

char _matrix_values[16]; 

uint all_columns_mask = 0x0; 
uint column_mask[4]; 

int sleep = 100;

void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]) {
    for (int i = 0; i < 16; i++) {
        _matrix_values[i] = matrix_values[i];
    }

    for (int K = 0; K < 4; K++) {
        _columns[K] = columns[K];
        _rows[K] = rows[K];

        gpio_init(_columns[K]); 
        gpio_init(_rows[K]); 

        gpio_set_dir(_columns[K], GPIO_IN); 
        gpio_set_dir(_rows[K], GPIO_OUT); 

        gpio_put(_rows[K], true);

        all_columns_mask += (1 << _columns[K]); 
        column_mask[K] = 1 << _columns[K]; 
    }
}

char pico_keypad_get_key(void) {
    int row;
    uint32_t cols;

    cols = gpio_get_all(); 
    cols = cols & all_columns_mask; 

    if (cols == 0x0) {
        return 0;
    }

    for (int j = 0; j < 4; j++) {
        gpio_put(_rows[j], 0);
    }

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

    for (int w = 0; w < 4; w++) {
        gpio_put(_rows[w], 1);
    }

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

void blink(bool rstate,bool bstate,bool gstate) {
    gpio_put(RED, rstate);  
    gpio_put(BLUE, bstate);
    gpio_put(GREEN, gstate);
}

void buzz(bool state) {
    gpio_put(BUZZER, state);
}

void sleep_time(int time) {
    sleep = time * 100;
}

int main() {
    stdio_init_all();  

    printf("=== TECLADO MATRICIAL ===\n");
    printf( "- A: LED Vermelho\n"
            "- B: LED Azul\n"
            "- C: LED Verde\n"
            "- D: Todos os LEDs\n"
            "- #: Buzzer\n"
        "\n\n");

    pico_keypad_init(columns, rows, KEY_MAP);  

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

    char key;
    char last_key = '\0';  // Variável para armazenar a última tecla

    while (true) {
        key = pico_keypad_get_key();

        if (key != '\0' && key != last_key) {
            printf("Tecla digitada: %c\n", key);
            last_key = key;  // Atualiza a última tecla
        } else if (key == '\0') {
            last_key = '\0';  // Reseta quando nenhuma tecla está pressionada
        }

        if (key == 'A') blink(true,false,false); 
        else if (key == 'B') blink(false,true,false); 
        else if (key == 'C') blink(false,false,true); 
        else if (key == 'D') blink(true,true,true); 
        else if (key == '#') buzz(true);
        else {
            blink(false,false,false);
            buzz(false);
        }
    }

    return 0;
}
