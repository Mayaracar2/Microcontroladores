int main(void){
    DDRD = 0; //PD5 entrada de NA , PD4 entrada de NF , PD3 S1 
    DDRD |= (1 << PD7) | (1 << PD6); //PD7 motor high  e PD6 alarme
	PORTD |= (1 << PD4) | (1 << PD3);
	PORTD &= ~(1 << PD5);

  while(1){
    if(PIND & (1 << PD5)){
      PORTD |=(1 << PD7);
      PORTD &= ~(1<< PD6);

    } else if(!(PIND & (1 << PD4 ))){
      PORTD &= ~(1 << PD7);

    }else if(!(PIND & (1 << PD3))){
      PORTD |=(1 << PD6);
      PORTD &= ~(1<< PD7);

    }
  }
}


https://www.tinkercad.com/things/iYCigYXMusA-relatorio-5-exercicio-1?sharecode=MCbSt9r4mRu73mrnl53nlXC9QUNnbhd-VLMHKb1e_b0&classId=0a854c39-d170-496f-a141-9b6c96d9d745&assignmentId=ebd9dab6-0000-4625-a266-74c6f6bdd1e4&submissionId=64147c98-dfc7-2f45-b454-9ad9329f0988
