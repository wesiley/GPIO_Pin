## Aluno
Lucas Carneiro de Araújo Lima
.
## PROJETO PUBLICADO NO WOKWI WEB

[Teclado Matricial - Lucas Carneiro de Araújo Lima](https://wokwi.com/projects/420539028431556609)<br>

### Funcionamento
Este programa conta com as seguintes funcionalidades:

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
- **LEDs:** Pressione:<br>
   'A' para ativar o LED vermelho.<br>
   'B' para ativar o LED azul.<br>
   'C' para ativar o LED verde.<br>
   'D' para ativar todos os LEDs ao mesmo tempo.<br>
- **Buzzer:** Pressione '#' para ativá-lo.
- **Piscar LEDs:** Pressione '*' para acionar uma sequência intercalada de piscamento dos LEDs.

### Exemplo de Uso
Se o usuário pressionar '5', o tempo de atividade será ajustado para 500 ms. Pressionando 'A' em seguida, o LED vermelho acenderá por esse período.

### Finalização
O programa continuará em execução até ser interrompido manualmente. Para parar, desconecte o Raspberry Pi Pico ou encerre o processo no terminal.

### Observações de Segurança
- Sempre use resistores apropriados para os LEDs.
- Evite sobrecarga de corrente nos pinos GPIO.

