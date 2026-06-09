#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    double oQ;             // Orange Quantity
    double Total = 0;      // Total a pagar
    
    cout << "Digite o numero de laranjas: " << endl;
    cin >> oQ;
    
    cout << fixed << setprecision(2);
    if (oQ >= 12) {     // Pelo menos 12
        Total = oQ*0.65;
        cout << "Preco da unidade: R$0.65" << endl;
        cout << "Preco total: R$" << Total << endl;
    } else {            // Menos de 12
        Total = oQ*0.80;
        cout << "Preco da unidade: R$0.80" << endl;
        cout << "Preco total: R$" << Total << endl;
    }
    return 0;
}
