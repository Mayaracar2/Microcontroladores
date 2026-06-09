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

  for(;;) {

   if (!(PIND & botao)){
      OCR0A = 127; //Intensidade 50%
   }else{
    OCR0A = 0; // Led apagado
   }

  }

}
