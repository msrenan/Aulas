#include <iostream>
#include "pile.h"

using namespace std;

/*
Retorna o número de elementos de uma pilha

# Arguments
- ``PilhaEstatica& pilha`` - Referência parapilha que será utilizada na contagem de elementos
*/
int num_of_elements(PilhaEstatica& pilha) {
    PilhaEstatica pilha_intermediaria = PilhaEstatica();
    int counter = 0;
    while (!pilha.vazia()) {
        int valor;
        bool resultado_desempilha = pilha.desempilha(valor);
        if (resultado_desempilha) {
            bool resultado_empilha = pilha_intermediaria.empilha(valor);
            if (resultado_empilha) {
                counter++;
            } else {
                break;
            }
        }
    }

     while (!pilha_intermediaria.vazia()) {
        int valor;
        bool resultado_desempilha = pilha_intermediaria.desempilha(valor);
        if (resultado_desempilha) {
            bool resultado_empilha = pilha.empilha(valor);
            if (resultado_empilha) {
                continue;
            } else {
                break;
            }
        }
    }
    return counter;
}

/*
Mostra todos os elementos contidos na pilha na ordem: TOPO -> BASE

# Arguments
- ``PilhaEstatica& pilha`` - Referência para a pilha que terá seus elementos mostrados
*/
void show_elements(PilhaEstatica& pilha) {
    PilhaEstatica pilha_intermediaria = PilhaEstatica();
    int counter = 0;
    while (!pilha.vazia()) {
        int valor;
        bool resultado_desempilha = pilha.desempilha(valor);
        if (resultado_desempilha) {
            bool resultado_empilha = pilha_intermediaria.empilha(valor);
            if (resultado_empilha) {
                printf("Valor #%d = %d\n", counter, valor);
                counter++;
            } else {
                break;
            }
        }
    }

    while (!pilha_intermediaria.vazia()) {
        int valor;
        bool resultado_desempilha = pilha_intermediaria.desempilha(valor);
        if (resultado_desempilha) {
            bool resultado_empilha = pilha.empilha(valor);
            if (resultado_empilha) {
                continue;
            } else {
                break;
            }
        }
    }
}

int main() {
    PilhaEstatica pilha = PilhaEstatica();
    pilha.empilha(1);
    pilha.empilha(2);
    pilha.empilha(3);
    show_elements(pilha);
    int valor_topo;
    pilha.desempilha(valor_topo);
    printf("Último elemento da pilha: %d\n", valor_topo);

    pilha.empilha(4);
    pilha.empilha(5);

    show_elements(pilha);

    printf("Pilha tem %d elemento(s)\n", num_of_elements(pilha));
}