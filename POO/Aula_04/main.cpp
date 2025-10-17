#include <iostream>
#include "vetor.h"

using namespace std;

int main() {
    Vetor v = Vetor(10);

    v.setValor(0, 5);
    v.setValor(1, 10);
    v.setValor(4, 5);
    int val;
    for (int i = 0; i < v.len(); i++) {
        v.getValor(i, val);
        cout << "#" << i << " -> " << val << endl;
    }
}