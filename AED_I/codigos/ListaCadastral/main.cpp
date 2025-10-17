#include <iostream>
#include "listacadastral.h"

using namespace std;

void mostra(ListaCadastral& lista) {
    cout << endl;
    int current;
    bool res = lista.getFirst(current);
    if (!res) {
        cout << "Lista vazia" << endl;
        return;
    }

    while (res) {
        cout << current << endl;
        res = lista.getNext(current);
    }
    cout << "Fim da lista" << endl;
    cout << endl;

}

int main() {
    ListaCadastral l = ListaCadastral();

    l.insere(2);
    mostra(l);
    l.insere(5);
    mostra(l);
    l.insere(10);
    cout << "Elementos inseridos" << endl;
    mostra(l);

    bool res;
    res = l.remove(3);
    cout << "res=" << res << endl;
    mostra(l);

    res = l.remove(5);
    cout << "res=" << res << endl;
    mostra(l);

    l.insere(1);
    mostra(l);
    l.insere(3);
    mostra(l);
    l.insere(6);
    mostra(l);
    l.insere(4);
    mostra(l);
    l.remove(2);
    mostra(l);



}