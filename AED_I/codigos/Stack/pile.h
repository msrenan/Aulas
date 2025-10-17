#include <iostream>

using namespace std;

#define TAM 10
/*
Classe TAD pilha

# Dados privados
- ```int topo``` - Armazena em qual posição se encontra o elemento no topo da pilha
- ```int data[TAM]``` - Armazena os valores na pilha estática, de tamanho = TAM

# Dados publicos
- ``PilhaEstatica`` - Construtor da pilha
*/
class PilhaEstatica {
    private:
        int topo;
        int data[TAM];
    
    public:

        // Construtor da pilha
        PilhaEstatica() {
            topo = -1;
        }

        // Retorna ``true`` se a pilha estiver cheia e ``False`` se não estiver
        bool cheia() {
            if (topo >= TAM) {
                return true;
            } else {
                return false;
            }
        }

        // Retorna ``true`` se a pilha estiver vazia e ``False`` se não estiver
        bool vazia() {
            if (topo == -1) {
                return true;
            } else {
                return false;
            }
        }

/*
Adiciona um valor no topo da pilha

# Arguments
- ```int valor``` - Valor a ser adicionado no topo da pilha

# Returns
- ``true`` se o valor foi corretamente adicionado a pilha
- ``false`` se o valor não pôde ser adicionado
*/
        bool empilha(int valor) {
            if (!cheia()) {
                topo++;
                data[topo] = valor;
                return true;
            } else {
                return false;
            }
        }

/*
Remove um valor no topo da pilha

# Arguments
- ```int &valor_desempilhado``` - Referência que receberá o valor a ser desempilhado do topo da pilha

# Returns
- ``true`` se o valor foi corretamente removido da pilha
- ``false`` se o valor não pôde ser removido
*/
        bool desempilha(int& valor_desempilhado) {
            if (!vazia()) {
                valor_desempilhado = data[topo];
                data[topo] = 0;
                topo--;
                return true;
            } else {
                return false;
            }
        }
};