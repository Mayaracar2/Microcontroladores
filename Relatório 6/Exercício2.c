int main (void){
  
  DDRD |= (1<<PD5)| (1<<PD4);
  
  EICRA = (1<<ISC01)|(1<<ISC00)|(1<<ISC11);
  EIMSK = (1<<INT0)| (1<<INT1);
  
  sei();
  
  while(1){
      PORTD |= (1<<PD4);
      _delay_ms(500);
      PORTD &= ~(1<<PD4);
      _delay_ms(500);
	}
}
ISR(INT0_vect){
    PORTD |= (1<<PD5);
    _delay_ms(1000);
    PORTD &= ~(1<<PD5);
 }
ISR(INT1_vect){
  if(EIMSK & (1<<INT0)){
    EIMSK &= ~(1<<INT0);
    _delay_ms(1000);
  }else{
    EIMSK |= (1<<INT0);
    _delay_ms(1000);
  }
}



https://www.tinkercad.com/things/04y0tlkoi7z-relatorio-6-exercicio-2?sharecode=2OrEooIY3lFkljD9vgY5Pludl4qfmZ083Alo2qxyHpY&classId=0a854c39-d170-496f-a141-9b6c96d9d745&assignmentId=3568f2cc-4570-42ea-ab18-061c0617c993&submissionId=30639f80-5b62-0a2b-8af2-8a0d21b5c417
