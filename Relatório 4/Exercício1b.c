int main () {
  DDRD |= 0b11000000;             // Pinos PD7 e Pd6 definidos como saída
  PORTD |= 0b00110000;            // Habilitar PULL-UP no PD5 e PD4
  PORTD &= ~(0b11000000);         // Desligar as saídas PD7 e PD6

  for(;;) {
    int botao = PIND & 0b00100000;      // Lê o estado do PD5
    int botao2 = PIND & 0b00010000;     // Lê o estado do PD4

    if(botao == 0) {         // Botão 1 pressionado ?
      // LED1 liga e após 1000ms desliga
      PORTD |= 0b10000000;
      _delay_ms(1000);
      PORTD &= ~(0b10000000);
    }
    if(botao2 == 0) {       // Botão 2 pressionado ?
      // LED2 liga e após 1000ms desliga
      PORTD |= 0b01000000;
      _delay_ms(1000);
      PORTD &= ~(0b01000000);
    }
  }
}
