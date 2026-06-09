#include <stdio.h>
#include <unistd.h>

#define ESTADO1 0b00000000
#define ESTADO2 0b00000001
#define ESTADO3 0b00000010

int main()
{
    int estado = 0;
    int button = 0;
    
    while(1) {
        switch (estado) {
            case ESTADO1:
                printf("00 \n");
                scanf("%d", &button);
                if(button == 1){
                    estado = ESTADO2;
                }
                break;
            case ESTADO2:
                printf("01 \n");
                scanf("%d", &button);
                if(button == 1){
                    estado = ESTADO3;
                }
                break;
            case ESTADO3:
                printf("10 \n");
                scanf("%d", &button);
                if(button == 1){
                    estado = ESTADO1;
                }
                break;
            default:
                break;
        }
    }

    return 0;
}
