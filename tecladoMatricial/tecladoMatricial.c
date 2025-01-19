#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Definições das portas GPIO dos LEDs
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12
#define RED_LED_PIN 13

// Definição da porta GPIO do BUZZER
#define BUZZER_PIN 21

// Define os GPIOs para as linhas e colunas do teclado matricial 4x4
#define ROWS 4
#define COLS 4

// Mapear GPIOs para linhas e colunas
const uint8_t row_pins[ROWS] = {8, 7, 6, 5};
const uint8_t col_pins[COLS] = {4, 3, 2, 1};

// Matriz de teclas
const char key_map[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Frequências das notas musicais (em Hz)
enum NotasMusicais {
    DO = 2640, // Dó
    RE = 2970, // Ré
    MI = 3300, // Mi
    FA = 3520, // Fá
    SOL = 3960, // Sol
    LA = 4400, // Lá
    SI = 4950  // Si
};

// Inicializa as linhas como saída e colunas como entrada
void init_gpio() {
    for (int i = 0; i < ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1); // Linha inicialmente em HIGH
    }

    for (int i = 0; i < COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]); // Ativa pull-up nas colunas
    }
}

// Inicializa LEDs e Buzzer
void init_leds_and_buzzer() {
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_put(GREEN_LED_PIN, 0);

    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);
    gpio_put(BLUE_LED_PIN, 0);

    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_put(RED_LED_PIN, 0);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}

// Configura o PWM no pino do buzzer com uma frequência especificada
void set_buzzer_frequency(uint pin, uint frequency) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (frequency * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

// Função para tocar o buzzer por um tempo especificado (em milissegundos)
void play_buzzer(uint pin, uint frequency, uint duration_ms) {
    set_buzzer_frequency(pin, frequency);
    pwm_set_gpio_level(pin, 32768);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
}

// Verifica qual tecla foi pressionada
char scan_keypad() {
    for (int row = 0; row < ROWS; row++) {
        gpio_put(row_pins[row], 0);
        for (int col = 0; col < COLS; col++) {
            if (gpio_get(col_pins[col]) == 0) {
                while (gpio_get(col_pins[col]) == 0);
                gpio_put(row_pins[row], 1);
                return key_map[row][col];
            }
        }
        gpio_put(row_pins[row], 1);
    }
    return '\0';
}

// Associa números do teclado a notas musicais
void play_note_for_key(char key) {
    switch (key) {
        case '1':
            play_buzzer(BUZZER_PIN, DO, 500);
            break;
        case '2':
            play_buzzer(BUZZER_PIN, RE, 500);
            break;
        case '3':
            play_buzzer(BUZZER_PIN, MI, 500);
            break;
        case '4':
            play_buzzer(BUZZER_PIN, FA, 500);
            break;
        case '5':
            play_buzzer(BUZZER_PIN, SOL, 500);
            break;
        case '6':
            play_buzzer(BUZZER_PIN, LA, 500);
            break;
        case '7':
            play_buzzer(BUZZER_PIN, SI, 500);
            break;
        case '8':
            play_buzzer(BUZZER_PIN, DO * 2, 500); // Dó mais agudo
            break;
        case '9':
            play_buzzer(BUZZER_PIN, RE * 2, 500); // Ré mais agudo
            break;
        case '0':
            play_buzzer(BUZZER_PIN, MI * 2, 500); // Mi mais agudo
            break;
        default:
            break;
    }
}
// Controla os LEDs com base na tecla pressionada
void control_leds(char key) {
    switch (key) {
        case 'A':
            gpio_put(GREEN_LED_PIN, 1);
            gpio_put(BLUE_LED_PIN, 0);
            gpio_put(RED_LED_PIN, 0);
            break;
        case 'B':
            gpio_put(GREEN_LED_PIN, 0);
            gpio_put(BLUE_LED_PIN, 1);
            gpio_put(RED_LED_PIN, 0);
            break;
        case 'C':
            gpio_put(GREEN_LED_PIN, 0);
            gpio_put(BLUE_LED_PIN, 0);
            gpio_put(RED_LED_PIN, 1);
            break;
        case 'D':
            gpio_put(GREEN_LED_PIN, 1);
            gpio_put(BLUE_LED_PIN, 1);
            gpio_put(RED_LED_PIN, 1);
            break;
        case '*':
            gpio_put(GREEN_LED_PIN, 1);
            sleep_ms(1000);
            gpio_put(GREEN_LED_PIN, 0);

            gpio_put(BLUE_LED_PIN, 1);
            sleep_ms(1000);
            gpio_put(BLUE_LED_PIN, 0);

            gpio_put(RED_LED_PIN, 1);
            sleep_ms(1000);
            gpio_put(RED_LED_PIN, 0);
            break;
        default:
            gpio_put(GREEN_LED_PIN, 0);
            gpio_put(BLUE_LED_PIN, 0);
            gpio_put(RED_LED_PIN, 0);
            break;
    }
}


int main() {
    stdio_init_all();
    init_gpio();
    init_leds_and_buzzer();

    printf("Teclado Matricial 4x4, pressione de 1 a 9 para controlar o buzzer \n e precione de A a D para controlar os leds \n pressione * para ligar o led de forma intercalada\n");

    while (1) {
        
        char key = scan_keypad();
        if (key != '\0') printf("Tecla digitada: %c\n", key);
            play_note_for_key(key);
            control_leds(key);
        sleep_ms(100);
    }

    return 0;
}
