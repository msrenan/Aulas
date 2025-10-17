#include <iostream>
#include "ChainStack.h"

using namespace std;

void mostrar(Stack& q) {
    Stack aux = Stack();

    while (!q.vazia()) {
        int n;
        q.retira(n);
        cout << n << endl;
        aux.insere(n);
    }

    cout << "Fim dos elementos" << endl;

    while (!aux.vazia()) {
        int n;
        aux.retira(n);
        q.insere(n);
    }
}

int main() {

    Stack fila = Stack();

    fila.insere(5);
    fila.insere(1);
    fila.insere(20);

    mostrar(fila);

    int val;
    fila.retira(val);

    cout << "val:" << val << endl;

    mostrar(fila);


}