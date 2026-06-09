uint8_t cont = 0;

void ConfigTimer0(void) {
    TCCR0A |= (1 << WGM01);           // Modo CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); // Divisor 1024
    OCR0A   = 249;              //250 ciclos × 64μs = 16ms por interrupção
    TIMSK0 |= (1 << OCIE0A);          // Habilita interrupção comparador A
}

ISR(TIMER0_COMPA_vect) {
    cont++;
    if (cont >= 10) {           // 10 × 16ms = 160ms ≈ 150ms
        PORTB ^= (1 << PB0);   // Inverte LED vermelho (toggle)
        cont = 0;
    }
}

int main(void) {
    DDRB |= (1 << PB0);   // PB0 como saída (LED Vermelho)
    ConfigTimer0();
    sei();                 // Habilita interrupção global

    while (1) {
        // Loop vazio — tudo controlado por interrupção
    }
}
