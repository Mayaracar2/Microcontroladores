#include <iostream>
using namespace std;

// Função que recebe 5 valores e faz a contagem
void analisarValores() {
    int num;
    int pares = 0;
    int impares = 0;
    int positivos = 0;
    int negativos = 0;

    // for para ler os 5 números
    for (int i = 0; i < 5; i++) {
        cin >> num;

        // par ou ímpar
        if (num % 2 == 0) {
            pares++;
        } else {
            impares++;
        }

        // positivo ou negativo
        if (num >= 0) {
            positivos++;
        } else if (num < 0) {
            negativos++;
        }
    }

    // Saída
    cout << "Quantidade de numeros pares: " << pares << endl;
    cout << "Quantidade de numeros impares: " << impares << endl;
    cout << "Quantidade de numeros positivos: " << positivos << endl;
    cout << "Quantidade de numeros negativos: " << negativos << endl;
    cout << endl;
}

int main() {

    int n;
    cout << "Digite a quantidade de testes:";
    cin >> n;  // quantidade de testes

    int contador = 0;

    // while para repetir os testes
    while (contador < n) {
        analisarValores();
        contador++;
    }

    return 0;
}
