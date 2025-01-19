# Integrantes
Lucas Carneiro de Araújo Lima

# GPIO_Pin

__Subgrupo 0__<br>
Repositório criado a fim de registrar a realização da atividade de cunho __obrigatório__ referente à aula síncrona do dia 14/01/2025.

## ATIVIDADE 
__Microcontroladores - GPIO__<br>

A atividade pede que haja controle de pinos GPIO com o emprego de um teclado matricial e para tanto os seguintes componentes se fazem necessários:

1) Teclado matricial 4x4;
2) 03 LEDs - tipo RGB;
3) Componente Buzzer;
4) Microcontrolador Raspberry Pi Pico W.

O código com a lógica adotada pelo grupo se encontra no presente reposítório e o resultado pode ser assistido no vídeo alocado neste link: [Aplicação com teclado matricial]().

## PROJETOS PUBLICADOS NO WOKWI WEB

[Teclado Matricial - Ana Karolina Disigant Reis](https://wokwi.com/projects/420523283314492417)<br>
[Teclado Matricial - Andressa Sousa Fonseca](https://wokwi.com/projects/420513649334097921)<br>
[Teclado Matricial - Gabriel Vitor](https://wokwi.com/projects/420540418141968385)<br>
[Teclado Matricial - Lucas Carneiro de Araújo Lima](https://wokwi.com/projects/420539028431556609)<br>
[Teclado Matricial - Luiz Rodrigo Oliveira da Silva](https://wokwi.com/projects/420521267008450561)<br>
[Teclado Matricial - Matheus Santos Souza](https://wokwi.com/projects/420371965130407937)<br>
[Teclado Matricial - Wesley R.](https://wokwi.com/projects/420390283337036801)

## INSTRUÇÕES DE USO DO PROGRAMA

Este programa permite o controle interativo de LEDs e um buzzer usando um teclado matricial conectado a um Raspberry Pi Pico. Siga os passos abaixo para utilizar corretamente a aplicação.

### Requisitos
- Raspberry Pi Pico com o SDK configurado.
- Teclado matricial com 4 colunas e 4 linhas.
- LEDs conectados aos pinos GPIO 13 (vermelho), 12 (azul), e 11 (verde).
- Buzzer conectado ao pino GPIO 21.
- Conexões elétricas adequadas entre o teclado, LEDs, buzzer e o Raspberry Pi Pico.

### Inicialização
1. Conecte os componentes conforme especificado.
2. Compile e carregue o código para o Raspberry Pi Pico.
3. Inicie o programa e abra uma interface de terminal para visualizar as mensagens.

### Funcionamento
Após o início do programa, a interface de terminal mostrará as seguintes instruções:

```
=== TESTE DO TECLADO MATRICIAL ===
1 a 9: Aumenta o tempo de duração padrão (100 ms) em N vezes.
A: Liga o LED vermelho.
B: Liga o LED azul.
C: Liga o LED verde.
D: Liga todos os LEDs.
#: Liga o buzzer.
*: Pisca os LEDs em sequência intercalada.
```

### Detalhes do Controle
- **Controle de Duração:** Pressione as teclas de '1' a '9' para aumentar o tempo de atividade dos LEDs e do buzzer. Por exemplo, pressionar '3' define o tempo como 300 ms.
- **LEDs:** Pressione:
  - 'A' para ativar o LED vermelho.
  - 'B' para ativar o LED azul.
  - 'C' para ativar o LED verde.
  - 'D' para ativar todos os LEDs ao mesmo tempo.
- **Buzzer:** Pressione '#' para ativá-lo.
- **Piscar LEDs:** Pressione '*' para acionar uma sequência intercalada de piscamento dos LEDs.

### Considerações
- Se nenhuma tecla for pressionada, os LEDs e o buzzer permanecerão desligados.
- O tempo padrão de atividade é de 100 ms, ajustável usando as teclas numéricas de 1 a 9.
- As mensagens no terminal indicarão a tecla pressionada e a ação correspondente.

### Exemplo de Uso
Se o usuário pressionar '5', o tempo de atividade será ajustado para 500 ms. Pressionando 'A' em seguida, o LED vermelho acenderá por esse período.

### Finalização
O programa continuará em execução até ser interrompido manualmente. Para parar, desconecte o Raspberry Pi Pico ou encerre o processo no terminal.

### Observações de Segurança
- Sempre use resistores apropriados para os LEDs.
- Evite sobrecarga de corrente nos pinos GPIO.
