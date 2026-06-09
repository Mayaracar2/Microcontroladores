#include <iostream>
using namespace std;

int main()
{
    int entrada;
    
    do {
        cout << "Digite um numero entre 0 e 50: ";
        cin >> entrada;
    } while(entrada <=0 || entrada >= 50);      // Garante que está entre 0 e 50
    
    for (int i=entrada-1; i > 0; i--){          // Loop de todos os valores exceto 0 e o valor de entrada
        cout << "Resto da divisao de " << entrada << " por " << i << ": " << entrada%i << endl;
    }
    
    return 0;
}
