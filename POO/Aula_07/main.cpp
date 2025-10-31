#include <iostream>
#include "inheritance.h"

using namespace std;

int main() {    

    Ponto a = Ponto(1, 1);
    Ponto b(1, -1);
    
    a.print();

    cout << b << endl;

    Vetor v = a - b;

    Vetor w = Vetor(1, 1);
    Vetor z = Vetor(1, -1);
    cout << v << endl;
    cout << z.pEsc(w) << endl;
    

}