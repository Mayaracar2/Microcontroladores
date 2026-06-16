#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

// ================= PINOS =================
#define LED PB1
#define BUZZER PB2
#define BOTAO PD2

#define TEMP_CH 0
#define BPM_CH  1

// ============ LIMITES (DEMO) ============
#define TEMP_HIPOT 10
#define TEMP_MAX   390

#define BPM_LOW  60
#define BPM_HIGH 100

// ============ TEMPOS (TICKS ~16ms) ============
#define T_100MS 6
#define T_250MS 15
#define T_500MS 31
#define T_800MS 49
#define T_1S    62
#define T_2S    124

// ============ ESTADOS ============
typedef enum {
    NORMAL,
    ALERTA,
    CRITICO,
    SILENCIADO
} Estado;

volatile Estado estado = NORMAL;
volatile uint32_t tick = 0;
volatile uint8_t botao_flag = 0;
uint8_t ultimo_estado = 255;
uint32_t t_mute = 0;
uint8_t mute_ativo = 0;

// temporizadores
uint32_t t_buzzer = 0;
uint32_t t_led = 0;
uint32_t t_uart = 0;
uint32_t t_silencio = 0;

// sensores
uint16_t temp = 0;
uint16_t bpm = 0;
uint32_t t_bip = 0;
uint8_t buzzer_on = 0;

// ================= ISR =================
ISR(TIMER2_OVF_vect){
    tick++;
}

ISR(INT0_vect){
    botao_flag = 1;
}

// ================= ADC =================
void adc_init(){
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch){
    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    return ADC;
}

// ================= UART =================
void uart_init(){
    UBRR0 = 103;
    UCSR0B = (1<<TXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void uart_char(char c){
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void uart_str(char *s){
    while(*s) uart_char(*s++);
}

void uart_num(uint16_t n){
    char b[6];
    itoa(n,b,10);
    uart_str(b);
}

// ================= TIMER2 =================
void timer2_init(){
    TCCR2A = 0;
    TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
    TIMSK2 = (1<<TOIE2);
}

// ================= PWM (REQUISITO) =================
void pwm_init(){

    DDRB |= (1<<BUZZER);

    // Timer1 CTC mode (TOP = OCR1A)
    TCCR1A = (1<<COM1B0);              // toggle OC1B (D10)
    TCCR1B = (1<<WGM12) | (1<<CS11);  // CTC + prescaler 8

    OCR1A = 0;
}

// =================
void buzzer_freq(uint16_t freq){

    if(freq == 0){
        TCCR1A &= ~(1<<COM1B0);  // desliga toggle
        PORTB &= ~(1<<BUZZER);
        return;
    }

    TCCR1A |= (1<<COM1B0); // garante toggle ativo

    OCR1A = (F_CPU / (2UL * 8 * freq)) - 1;
}

// ================= GPIO =================
void gpio_init(){
    DDRB |= (1<<LED) | (1<<BUZZER);
    DDRD &= ~(1<<BOTAO);
    PORTD |= (1<<BOTAO);
}

// ================= UTIL =================
uint8_t temp_estado(){
    if(temp < TEMP_HIPOT) return 1;
    if(temp > TEMP_MAX) return 2;
    return 0;
}

uint8_t bpm_estado(){
    if(bpm < BPM_LOW) return 1;
    if(bpm > BPM_HIGH) return 2;
    return 0;
}

// ================= SENSOR =================
void ler_sensores(){
    temp = (adc_read(TEMP_CH) * 500UL) / 1023;
    bpm  = 30 + (adc_read(BPM_CH) * 170UL) / 1023;
}

// ================= UART STATUS =================
void uart_status(){
    uart_str("\r\nTEMP: ");
    uart_num(temp/10);
    uart_char('.');
    uart_num(temp%10);
    uart_str("C | BPM: ");
    uart_num(bpm);

    uart_str("\r\n");

    uart_str("TEMP STATUS: ");
    if(temp_estado()==0) uart_str("NORMAL");
    else if(temp_estado()==1) uart_str("HIPO");
    else uart_str("FEBRE");

    uart_str(" | BPM STATUS: ");
    if(bpm_estado()==0) uart_str("NORMAL");
    else if(bpm_estado()==1) uart_str("BRADICARDIA");
    else uart_str("TAQUICARDIA");

    uart_str("\r\nESTADO: ");
    if(estado==NORMAL) uart_str("NORMAL");
    else if(estado==ALERTA) uart_str("ALERTA");
    else if(estado==CRITICO) uart_str("CRITICO");
    else uart_str("SILENCIADO");

    uart_str("\r\n----------------------\r\n");
}

// ================= ESTADO =================
void atualizar_estado(){

    uint8_t t = temp_estado();
    uint8_t b = bpm_estado();

    // Botão: silencia buzzer por 3s
    if(botao_flag){
        botao_flag = 0;

        mute_ativo = 1;
        t_mute = tick;

        uart_str("\r\nALARMES SILENCIADOS POR 3s\r\n");
        return;
    }

    // Determina o novo estado
    Estado novo_estado;

    if(t != 0 && b != 0){
        novo_estado = CRITICO;
    }
    else if(t != 0 || b != 0){
        novo_estado = ALERTA;
    }
    else{
        novo_estado = NORMAL;
    }

    // Atualiza somente se houve mudança
    if(novo_estado != estado){

        estado = novo_estado;

        if(estado == ALERTA){
            uart_str("\r\nALERTA DETECTADO\r\n");
        }
        else if(estado == CRITICO){
            uart_str("\r\nCRITICO DETECTADO\r\n");
        }
        else if(estado == NORMAL){
            uart_str("\r\nPACIENTE ESTAVEL\r\n");
        }
    }
}

// ================= LED =================
void led_exec(){
    if(estado == NORMAL){
        PORTB &= ~(1<<LED);
    }

    else if(estado == ALERTA){
        if(tick - t_led > T_500MS){
            PORTB ^= (1<<LED);
            t_led = tick;
        }
    }

    else if(estado == CRITICO){
        PORTB |= (1<<LED);
    }

    else{
        if(tick - t_led > T_1S){
            PORTB ^= (1<<LED);
            t_led = tick;
        }
    }
}

// ================= BUZZER (ATIVO) =================
void buzzer_exec(){

  if(mute_ativo){
    if(tick - t_mute < 187){ // ~3s (62 ticks = 1s)
        buzzer_freq(0);
        return;
    } else {
        mute_ativo = 0; // libera novamente
    }
}

    if(estado == SILENCIADO){
        buzzer_freq(0);
        return;
    }

    uint16_t intervalo;

    if(estado == NORMAL) intervalo = T_2S;
    else if(estado == ALERTA) intervalo = T_800MS;
    else intervalo = T_250MS;

    if(tick - t_bip >= intervalo){

        if(buzzer_on){
            buzzer_freq(0);   // OFF
            buzzer_on = 0;
        } else {
            if(estado == NORMAL) buzzer_freq(800);
            else if(estado == ALERTA) buzzer_freq(1200);
            else buzzer_freq(2000);

            buzzer_on = 1;
        }

        t_bip = tick;
    }
}

// ================= MAIN =================
int main(){

    gpio_init();
    adc_init();
    uart_init();
    timer2_init();
    pwm_init();

    EICRA = (1<<ISC01);
    EIMSK = (1<<INT0);

    sei();

    uart_str("MONITOR DE SINAIS VITAIS INICIADO\r\n");

    while(1){

        ler_sensores();
        atualizar_estado();

        led_exec();
        buzzer_exec();

        if(tick - t_uart > T_1S){
            uart_status();
            t_uart = tick;
        }
    }
}
