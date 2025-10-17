#include <iostream>
#include "queue.h"

using namespace std;

void mostrar_elementos(Fila& fila) {
    Fila fila_auxiliar = Fila();
    int counter = 0;
    while (!fila.vazia()) {
        int elemento;
        bool resultado_retira = fila.retira(elemento);
        if (resultado_retira) {
            printf("Elemento #%d = %d\n", counter, elemento);
            fila_auxiliar.insere(elemento);
            counter++;
        } else {
            break;
        }
    }

    while (!fila.cheia()) {
        int elemento;
        bool resultado_retira = fila_auxiliar.retira(elemento);
        if (resultado_retira) {
            fila.insere(elemento);
        } else {
            break;
        }
    }
}

int get_primeiro_elemento(Fila& fila) {
    Fila auxiliar = Fila();
    int counter = 0;
    int primeiro;
    while (!fila.vazia()) {
        int elemento;
        bool resultado_retira = fila.retira(elemento);
        if (resultado_retira) {
            if (counter == 0) {
                primeiro = elemento;
            }
            auxiliar.insere(elemento);
        } else {
            break;
        }
        counter++;
    }

    while (!fila.cheia()) {
        int elemento;
        bool resultado_retira = auxiliar.retira(elemento);
        if (resultado_retira) {
            fila.insere(elemento);
        } else {
            break;
        }
    }

    return primeiro;

}

int get_ultimo_elemento(Fila& fila) {
    Fila auxiliar = Fila();
    int ultimo;
    int elemento;
    while (!fila.vazia()) {
        bool resultado_retira = fila.retira(elemento);
        if (resultado_retira) {
            auxiliar.insere(elemento);
        } else {
            break;
        }
    }

    ultimo = elemento;

    while (!auxiliar.vazia()) {
        int elemento;
        bool resultado_retira = auxiliar.retira(elemento);
        if (resultado_retira) {
            fila.insere(elemento);
        } else {
            break;
        }
    }

    return ultimo;
}

bool sao_iguais(Fila& f1, Fila& f2) {
    bool resultado = true;
    Fila auxiliar1 = Fila();
    Fila auxiliar2 = Fila();
    while (!f1.vazia() || !f2.vazia()) {
        int e1;
        int e2;

        bool res1 = f1.retira(e1);
        if (res1) {
            auxiliar1.insere(e1);
        }
        bool res2 = f2.retira(e2);
        if (res2) {
            auxiliar2.insere(e2);
        }

        if (res1 != res2) {
            resultado = false;
        } else {
            if (e1 == e2) {
                continue;
            } else {
                resultado = false;
            }
        }
    }

    while(!auxiliar1.vazia()) {
        int elemento;
        bool resultado_retira = auxiliar1.retira(elemento);
        if (resultado_retira) {
            f1.insere(elemento);
        } else {
            break;
        }
    }

    while(!auxiliar2.vazia()) {
        int elemento;
        bool resultado_retira = auxiliar2.retira(elemento);
        if (resultado_retira) {
            f2.insere(elemento);
        } else {
            break;
        }
    }

    return resultado;
}



int main() {

    Fila fila = Fila();

    fila.insere(1);
    fila.insere(2);
    fila.insere(3);
    fila.insere(4);
    fila.insere(5);
    fila.insere(6);
    fila.insere(6);
    fila.insere(6);
    fila.insere(6);
    fila.insere(6);

    int valor_retirado;
    fila.retira(valor_retirado);
    fila.retira(valor_retirado);

    printf("primeiro elemento: %d\n", get_primeiro_elemento(fila));
    printf("ultimo elemento: %d\n", get_ultimo_elemento(fila));

    mostrar_elementos(fila);
    cout << endl;
    Fila f1 = Fila();
    f1.insere(1);
    f1.insere(2);

    Fila f2 = Fila();

    f2.insere(10);
    f2.insere(50);
    f2.insere(1);
    f2.insere(2);
    mostrar_elementos(f1);
    cout << endl;
    mostrar_elementos(f2);
    cout << endl;
    bool iguais = sao_iguais(f1, f2);
    printf("São iguais: %s\n", iguais ? "Sim" : "Não");
    mostrar_elementos(f1);
    cout << endl;
    mostrar_elementos(f2);
    cout << endl;
    int v;
    f2.retira(v);
    f2.retira(v);
    bool iguais2 = sao_iguais(f1, f2);

    printf("São iguais: %s\n", iguais2 ? "Sim" : "Não");


}