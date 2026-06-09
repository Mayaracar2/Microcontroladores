#include <stdio.h>
#include <unistd.h>

#define ESTADO1 0b00000000
#define ESTADO2 0b00000001
#define ESTADO3 0b00000010
#define ESTADO4 0b00000011
#define DELAY sleep(0.5) // Meio segundo

int main()
{
    int estado = 0;
    
    while(1) {
        switch (estado) {
            case ESTADO1:
                printf("00 \n");
                DELAY;
                estado = ESTADO2;
                break;
            case ESTADO2:
                printf("01 \n");
                DELAY;
                estado = ESTADO3;
                break;
            case ESTADO3:
                printf("10 \n");
                DELAY;
                estado = ESTADO4;
                break;
            case ESTADO4:
                printf("11 \n");
                DELAY;
                estado = ESTADO1;
                break;
            default:
                break;
        }
    }

    return 0;
}
