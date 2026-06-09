#include <stdio.h>
#include <unistd.h>

#define VERDE 2
#define AMARELO 3
#define VERMELHO 4

#define DELAY_VERDE sleep(12)
#define DELAY_AMARELO sleep(3)
#define DELAY_VERMELHO sleep(15)
unsigned char estado = VERMELHO, led_vermelho = 0, led_amarelo = 0, led_verde = 0;

int main(void)
{
    for(;;) {
        
        switch(estado){
            case VERDE:
                led_verde = 1;
                printf("Verde \n");
                DELAY_VERDE;
                led_verde = 0;
                estado = AMARELO;
                break;
            case AMARELO:
                led_amarelo = 1;
                printf("Amarelo \n");
                DELAY_AMARELO;
                led_amarelo = 0;
                estado = VERMELHO;
                break;
            case VERMELHO:
                led_vermelho = 1;
                printf("Vermelho \n");
                DELAY_VERMELHO;
                led_vermelho = 0;
                estado = VERDE;
                break;
            default:
                break;
        }
    }

    return 0;
}
