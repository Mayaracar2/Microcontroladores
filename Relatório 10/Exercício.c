// Programa: Controle de Motor com PWM, Timer e Interrupções
// Linguagem: C puro AVR (ATmega328P)
// Compilação: avr-gcc -mmcu=atmega328p -o programa.elf programa.c
// Upload: avrdude

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Definições de pinos
#define BOTAO_LIGA_PIN    PD2    // INT0
#define BOTAO_DESLIGA_PIN PD3    // INT1
#define LED_50_PIN        PD4
#define LED_100_PIN       PD5
#define MOTOR_PWM_PIN     PD6    // OC0A (Timer0)

// Estados do sistema
volatile uint8_t sistema_ativo = 0;
volatile uint8_t led_50_ativo = 0;
volatile uint16_t tempo_decorrido = 0;  // em ms (0 a 10000)
volatile uint8_t contador_timer = 0;

// Protótipos
void configurar_pinos(void);
void configurar_timer0_pwm(void);
void configurar_timer2_contador(void);
void configurar_interrupcoes_externas(void);
void ligar_sistema(void);
void desligar_sistema(void);
void atualizar_motor(void);

int main(void) {
  // Desabilitar interrupções globais inicialmente
  cli();
  
  configurar_pinos();
  configurar_timer0_pwm();
  configurar_timer2_contador();
  configurar_interrupcoes_externas();
  
  // Habilitar interrupções globais
  sei();
  
  // Loop principal
  for(;;) {
    if (sistema_ativo) {
      atualizar_motor();
      _delay_ms(10);
    }
  }
  
  return 0;
}

// Configurar direção dos pinos (entrada/saída)
void configurar_pinos(void) {
  // PORTD: Saídas (LEDs, PWM) e Entradas (Botões)
  DDRD = (1 << BOTAO_LIGA_PIN) | (1 << BOTAO_DESLIGA_PIN) | 
         (1 << LED_50_PIN) | (1 << LED_100_PIN) | (1 << MOTOR_PWM_PIN);
  
  // Apenas bit 2 (PD2) e 3 (PD3) são entradas (botões)
  DDRD &= ~(1 << BOTAO_LIGA_PIN);
  DDRD &= ~(1 << BOTAO_DESLIGA_PIN);
  
  // Ativar pull-up interno nos botões
  PORTD |= (1 << BOTAO_LIGA_PIN);
  PORTD |= (1 << BOTAO_DESLIGA_PIN);
  
  // Iniciar com LEDs desligados
  PORTD &= ~(1 << LED_50_PIN);
  PORTD &= ~(1 << LED_100_PIN);
}

// Configurar Timer0 para PWM no pino 6 (OC0A)
void configurar_timer0_pwm(void) {
  // TCCR0A: Modo PWM Fast, saída em OC0A (PD6)
  TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << WGM01) | (1 << WGM00);
  
  // TCCR0B: Desabilitado inicialmente (CS02=0, CS01=0, CS00=0)
  // Será habilitado apenas quando ligar_sistema() é chamado
  TCCR0B = 0;
  
  // OCR0A: Valor inicial PWM = 0
  OCR0A = 0;
}

// Configurar Timer2 para contar tempo com overflow
void configurar_timer2_contador(void) {
  // TCCR2A: Modo Normal
  TCCR2A = 0x00;
  
  // TCCR2B: Será habilitado apenas quando ligar_sistema() é chamado
  TCCR2B = 0;
  
  // TIMSK2: Será habilitado apenas quando ligar_sistema() é chamado
  TIMSK2 = 0;
  
  // TCNT2: Valor inicial
  TCNT2 = 0;
}

// Configurar interrupções externas INT0 e INT1
void configurar_interrupcoes_externas(void) {
  // EICRA: Borda de descida (01) para INT0 e INT1
  EICRA = (1 << ISC01) | (0 << ISC00) | (1 << ISC11) | (0 << ISC10);
  
  // EIMSK: Habilitar INT0 e INT1
  EIMSK = (1 << INT0) | (1 << INT1);
}

// Ligar sistema
void ligar_sistema(void) {
  sistema_ativo = 1;
  tempo_decorrido = 0;
  led_50_ativo = 0;
  PORTD &= ~(1 << LED_50_PIN);
  PORTD &= ~(1 << LED_100_PIN);
  OCR0A = 0;
  
  // Resetar Timer2
  TCNT2 = 0;
  contador_timer = 0;
  
  // Habilitar Timer0 para PWM
  TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00);  // Pre-scaler 64
  
  // Habilitar Timer2 para contar tempo
  TCCR2B = (1 << CS22) | (0 << CS21) | (1 << CS20);  // Pre-scaler 128
  TIMSK2 |= (1 << TOIE2);  // Habilitar overflow interrupt
}

// Desligar sistema
void desligar_sistema(void) {
  sistema_ativo = 0;
  tempo_decorrido = 0;
  led_50_ativo = 0;
  OCR0A = 0;
  PORTD &= ~(1 << LED_50_PIN);
  PORTD &= ~(1 << LED_100_PIN);
  
  // Desabilitar Timer0
  TCCR0B = 0;
  
  // Desabilitar Timer2
  TCCR2B = 0;
  TIMSK2 &= ~(1 << TOIE2);
  TCNT2 = 0;
}

// Atualizar velocidade do motor e controlar LEDs
void atualizar_motor(void) {
  // Calcular velocidade PWM (0-255) proporcional ao tempo (0-10000ms)
  uint16_t velocidade = (tempo_decorrido * 255) / 10000;
  if (velocidade > 255) velocidade = 255;
  
  // Aplicar PWM
  OCR0A = (uint8_t)velocidade;
  
  // LED 50% - acende aos 5 segundos
  if (tempo_decorrido >= 5000 && !led_50_ativo) {
    PORTD |= (1 << LED_50_PIN);
    led_50_ativo = 1;
  }
  
  // LED 100% e fim do ciclo - acontece aos 10 segundos
  if (tempo_decorrido >= 10000) {
    PORTD &= ~(1 << LED_50_PIN);
    PORTD |= (1 << LED_100_PIN);
    
    // Aguardar um ciclo antes de desligar para visualizar LED 100%
    _delay_ms(500);
    desligar_sistema();
  }
}

// Interrupção INT0 - Botão LIGA (PD2)
ISR(INT0_vect) {
  _delay_ms(20);  // Debounce
  
  if (!(PIND & (1 << BOTAO_LIGA_PIN))) {  // Botão pressionado (LOW)
    ligar_sistema();
  }
}

// Interrupção INT1 - Botão DESLIGA (PD3)
ISR(INT1_vect) {
  _delay_ms(20);  // Debounce
  
  if (!(PIND & (1 << BOTAO_DESLIGA_PIN))) {  // Botão pressionado (LOW)
    desligar_sistema();
  }
}

// Interrupção Timer2 Overflow (~2.048ms com Pre-Scaler 128)
ISR(TIMER2_OVF_vect) {
  contador_timer++;
  
  // A cada ~2ms, incrementar tempo_decorrido
  if (contador_timer >= 5) {  // 5 * 2ms = 10ms
    tempo_decorrido += 10;
    if (tempo_decorrido > 10000) tempo_decorrido = 10000;
    contador_timer = 0;
  }
}
