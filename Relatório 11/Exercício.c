#define PINO PC4 // ADC4 // PINOS - A0 a A5

unsigned long int LeituraAD = 0;
unsigned long int tensao = 0;
unsigned long int cont = 0;

int main(){

  Serial.begin(9600);

  // CONFIGURAÇÃO DO ADC
  ADMUX = (1 << REFS0);                                               // TENSÃO DE REFERENCIA = 5V
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // CONTROLE DO CLOCK (ADPS0/1/2) VELOCIDADE DE CONVERSÃO

  //PWM
  //Pino de saida PD6
  DDRD |= (1<<PD6);

  // Modo de operação - fast pwm
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
 
  TCCR0B = (1 << CS00);
  
  OCR0A = 100;

  while(1){

    // PINO DE LEITURA
    ADMUX = (ADMUX & 0b11111000) + PINO;

    // INICIAR A CONVERSÃO
    ADCSRA |= (1 << ADSC);

    // ESPERA A CONVERSÃO FINALIZAR
    while((ADCSRA & (1 << ADSC)) == (1 << ADSC));
    // verifica se a posição ADSC está em 1, aguarda enquanto está fazendo a conversão

    // ARMAZENAR CONVERSÃO
    // LeituraAD = ADC;            // ADC é para onde vai o valor convertido

    // LEITOR AD VARIA DE 0 A 1023 PARA 5V
    LeituraAD = ADCL;
    LeituraAD |= (ADCH << 8);

    // CALCULAR TENSÃO
    tensao = (LeituraAD * 5000) / 1023;

    Serial.println(tensao);
    //Serial.println(" mV");

    cont = 0.051 * tensao;

    Serial.println(cont);

    OCR0A = cont;
  }
}
