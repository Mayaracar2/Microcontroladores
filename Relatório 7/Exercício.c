int main(void){
  DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD7);
  DDRB |= (1 << PB1);

  PORTB |= (1 << PB0) | (1 << PB2);
  PORTD |= (1 << PD4);
 
  PCICR |= (1 << PCIE2) | (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT2);
  PCMSK2 |= (1 << PCINT20);
 
  sei();
 
  for(;;){
    PORTD |= (1 << PD5);
    _delay_ms(250);
    PORTD &= ~(1 << PD5);
    _delay_ms(250);
  }
 
  return 0;
}
volatile uint8_t lastB = 0;

ISR(PCINT0_vect){
  uint8_t currentB = PINB;

  if( (currentB & (1 << PB0)) && !(lastB & (1 << PB0)) ){
    PORTD |= (1 << PD7);
    _delay_ms(1000);
    PORTD &= ~(1 << PD7);
  }

  if( (currentB & (1 << PB2)) && !(lastB & (1 << PB2)) ){
    PORTD |= (1 << PD6);
    _delay_ms(500);
    PORTD &= ~(1 << PD6);
  }

  lastB = currentB;
}
volatile uint8_t lastD = 0;

ISR(PCINT2_vect){
  uint8_t currentD = PIND;

  if( (currentD & (1 << PD4)) && !(lastD & (1 << PD4)) ){
    PORTB |= (1 << PB1);
    _delay_ms(2000);
    PORTB &= ~(1 << PB1);
  }

  lastD = currentD;
}
