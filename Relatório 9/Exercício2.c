#define pwm_out (1 << PD6)
#define botao (1 << PD2)
int brightness = 0;

int main(){
  DDRD |= pwm_out; //configura saída para o pwm
  PORTD &= ~pwm_out; //pwm inicia desligado
  DDRD &= ~botao; //Botão como entrada 
  PORTD |= botao; //pull-up

  //Configura modo FAST PWM e modo do comparador A
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
  TCCR0B = (1 << CS00); 
  OCR0A = 0;

  //Configura int0 como descida
  EICRA = (1 << ISC01);
  EIMSK = (1 << INT0);

  sei(); //Habilita interrupção

  for(;;) {}
}

 ISR(INT0_vect) {
    brightness += 25;     // +~10% (25/255 ≈ 10%)
    if (brightness > 255) // Ao chegar em ~100%...
        brightness = 0;   // ...desliga
    OCR0A = brightness;
} 
