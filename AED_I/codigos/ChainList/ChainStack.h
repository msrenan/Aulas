#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int valor;
    struct Node* proximo;
};

Node* newNode() {
    struct Node* node = new Node;
    return node;
}

class Stack {
    private:
        struct Node* topo;
    
    public:
        Stack() {
            this->topo = nullptr;
        }

        bool vazia() {
            return this->topo == nullptr;
        }

        bool insere(int num) {
            struct Node* aux = newNode();
            aux->valor = num;
            aux->proximo = this->topo;
            this->topo = aux;
            return true;
        }

        bool retira(int& num) {
            if (!this->vazia()) {
                struct Node* aux = newNode();
                aux = this->topo;
                num = aux->valor;
                this->topo = aux->proximo;
                return true;
            } else {
                return false;
            }
        }
};