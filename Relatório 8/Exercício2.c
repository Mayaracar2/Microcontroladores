uint8_t ticks   = 0;
uint8_t segundos = 0;
uint8_t minutos  = 0;

void ConfigTimer0(void) {
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS02) | (1 << CS00);
    OCR0A   = 249;
    TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
    ticks++;

    if (ticks == 7) {
        PORTB &= ~(1 << PB0);   // Apaga LED vermelho após 100ms
        PORTB &= ~(1 << PB1);   // Apaga LED verde após 100ms
    }

    if (ticks >= 63) {
        ticks = 0;
        segundos++;

        PORTB |= (1 << PB0);    // Acende LED vermelho

        if (segundos >=  60) {
            segundos = 0;
            minutos++;
            PORTB |= (1 << PB1); // Acende LED verde
        }
    }
}

int main(void) {
    DDRB |= (1 << PB0) | (1 << PB1);
    ConfigTimer0();
    sei();

    while (1) {
        // Tudo gerenciado por interrupção
    }
}
