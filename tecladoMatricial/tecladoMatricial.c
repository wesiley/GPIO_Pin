#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdlib.h"  

#include "hardware/pwm.h"
#include <locale.h>


char senha = 0;
//Definindo pinos dos LEDs
const uint led_green = 11;
const uint led_blue = 12;
const uint led_red  = 13;

//Definindo pino do buzzer
const uint buzzer = 21;


//Definindo pinos das linhas e colunas do teclado matricial
#define R1 9
#define R2 8
#define R3 7
#define R4 6
#define C1 5
#define C2 4
#define C3 3
#define C4 2

//Configurando pwd no pino do buzzer para saída sonora
void pwd_buzzer(){
  gpio_set_function(buzzer, GPIO_FUNC_PWM);
  int slice_num = pwm_gpio_to_slice_num(buzzer);
  pwm_config config = pwm_get_default_config();
  pwm_init(slice_num, &config, true);
  pwm_set_gpio_level(buzzer, 0); 
}

//função para emitir um bip quando a senha digitada for a incorreta
void bipERRO(){
  pwm_set_gpio_level(buzzer, 2048);
  sleep_ms(200);
  pwm_set_gpio_level(buzzer, 0);
  sleep_ms(100);
}

//função para emitir dois bips indicando que a configuração foi concluída com sucesso
void bipCORRETO(){
  pwm_set_gpio_level(buzzer, 2048);
  sleep_ms(200);
  pwm_set_gpio_level(buzzer, 0);
  sleep_ms(100);
  pwm_set_gpio_level(buzzer, 2048);
  sleep_ms(200);
  pwm_set_gpio_level(buzzer, 0);
  sleep_ms(100);
}

//Configurando pinos
void init_pinos(){
  gpio_init(R1);
  gpio_init(R2);
  gpio_init(R3);
  gpio_init(R4);
  gpio_init(C1);
  gpio_init(C2);
  gpio_init(C3);
  gpio_init(C4);
  gpio_set_dir(R1, GPIO_OUT);
  gpio_set_dir(R2, GPIO_OUT);
  gpio_set_dir(R3, GPIO_OUT);
  gpio_set_dir(R4, GPIO_OUT);
  gpio_set_dir(C1, GPIO_IN);
  gpio_set_dir(C2, GPIO_IN);
  gpio_set_dir(C3, GPIO_IN);
  gpio_set_dir(C4, GPIO_IN);
  gpio_pull_up(C1);
  gpio_pull_up(C2);
  gpio_pull_up(C3);
  gpio_pull_up(C4);

  gpio_init(led_red);
  gpio_init(led_green);
  gpio_init(led_blue);
  gpio_set_dir(led_red,GPIO_OUT);
  gpio_set_dir(led_blue,GPIO_OUT);
  gpio_set_dir(led_green,GPIO_OUT);
  gpio_put(led_red,0);
  gpio_put(led_green,0);
  gpio_put(led_green,0);
}

//Leitura do teclado
char keypad_leitura(){
  char teclado [4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
  };
  int linhas [] = {R1,R2,R3,R4};
  int colunas [] = {C1,C2,C3,C4};

  for(int lin =0; lin<4; lin ++){
    gpio_put(linhas[lin],0);
    for(int col =0; col<4; col++){
      if(!gpio_get(colunas[col])){
        sleep_ms(300);
        gpio_put(linhas[lin],1);
        return(teclado[lin][col]);
      }
    }
    gpio_put(linhas[lin],1);
  }
  return '\0';
}

//limpar monitor para simulador wokwi
void limparmonitor(){
  for(int i=0;i<10;i++){
    printf("\n");
  }
}

void configurar_sistema(){
  int j=-1;
  senha='\0';
  gpio_put(led_blue,1);
  printf("\nIniciando modo de configuração...\n");
  while(j==-1){
    printf("\n\tSelecione do teclado a senha desejada.\nObservação:A senha é formada por um caractere no intervalo de 1 a 9\n");
    while(senha=='\0'){
      senha=keypad_leitura();
      sleep_ms(200);
    }
    if(senha=='1' || senha =='9' || senha=='2' || senha=='3' || senha =='4' || senha == '5' || senha == '6' || senha== '7' || senha == '8'){
      j=0;
      gpio_put(led_blue,0);
      limparmonitor();
      printf("Senha cadastrada com sucesso");
      gpio_put(led_red,1);
      gpio_put(led_green,1);
      gpio_put(led_blue,1);
      bipCORRETO();
      sleep_ms(1000);
      gpio_put(led_red,0);
      gpio_put(led_green,0);
      gpio_put(led_blue,0);
    }else{
      senha='\0';
      limparmonitor();
      printf("Erro!Tente novamente");
      bipERRO();
      sleep_ms(1100);
    }
     limparmonitor();
  }
}


int main() {
  int usuario='\0';
  setlocale(LC_ALL,"");
  stdio_init_all();
  pwd_buzzer();
  init_pinos();
  printf("\nSistema iniciado!\n");
  configurar_sistema();
  
  while (true) {
    limparmonitor();
    printf("\tSistema habilitado\n\nPressione # se quiser acessar as Configurações\n");
    while(usuario=='\0'){
      usuario = keypad_leitura();
      sleep_ms(200);
    }
    if(usuario==senha){
      limparmonitor();
      printf("\nSenha correta! Cofre aberto!");
      gpio_put(led_green,1);
      bipCORRETO();
      sleep_ms(2000);
      gpio_put(led_green,0);
    }else {
      if(usuario=='#'){
        limparmonitor();
        configurar_sistema();
      }else{
        
        limparmonitor();
        printf("\nSenha incorreta");
        gpio_put(led_red,1);
        bipERRO();
        sleep_ms(1500);
        gpio_put(led_red,0);
      }
    }
    usuario='\0';
  }
}
