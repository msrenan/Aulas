#include <iostream>

using namespace std;

struct Node {
    int info;
    Node* esq;
    Node* dir;
    Node* pai;
    int index;
};

class Heap {
    private:
        Node* F;
        int size;
        int currIndex;

        void troca(Node* P);
        void deletaHeap(Node* P) {
            if (P->esq == NULL && P->dir == NULL) {
                Node* aux = P;
                P = aux->pai;
                delete aux;
                return;
            }

            if (P->esq != NULL) {
                deletaHeap(P->esq);
            }
            if (P->dir != NULL) {
                deletaHeap(P->dir);
            }

            Node* aux = P;
            P = aux->pai;
            delete aux;
            return;
        }

    public:
        Heap() {
            F = NULL;
            size = 0;
            currIndex = 0;
        }

        bool insere(Node* P, int val);
        bool remove(Node* P, int val);
        bool cheia();
        bool vazia() {
            return size == 0;
        }
        void mostra(Node* P);
        Node* getRoot() {
            return F;
        }

        ~Heap() {
            deletaHeap(F);
            cout << "Heap deleted" << endl;
        }

};

bool Heap::insere(Node* P, int val) {
    if (P == NULL) {
        P = new Node;
        P->info = val;
        P->dir = NULL;
        P->esq = NULL;
        P->pai = NULL;
        P->index = currIndex;
        Heap::currIndex++;
        size++;
        return true;
    }

    if (val > P->info) {
        if (P->dir == NULL) {
            Node* aux = new Node;
            P->dir = aux;
            aux->pai = P;
            aux->info = val;
            aux->dir = NULL;
            aux->esq = NULL;
            aux->index = currIndex;
            currIndex++;
            size++;
            return true;
        }

        return insere(P->dir, val);
    } else {
        if (P->esq == NULL) {
            Node* aux = new Node;
            P->esq = aux;
            aux->pai = P;
            aux->info = val;
            aux->dir = NULL;
            aux->esq = NULL;
            aux->index = currIndex;
            currIndex++;
            size++;
            return true;
        }

        return insere(P->esq, val);
    }
}

void Heap::troca(Node* P) {
    if (P->info > P->pai->info) {
        int aux = P->info;
        int auxI = P->index;
        P->info = P->pai->info;
        P->index = P->pai->index;
        P->pai->info = aux;
        P->pai->index = auxI;
        return troca(P->pai);
    }

    return;
}

void Heap::mostra(Node* P) {
    if (vazia()) {
        cout << "There's nothing to show." << endl;
    }

    if (P->esq == NULL && P->dir == NULL) {
        Node* aux = P;
        P = aux->pai;
        cout << P->index << ":" << P->info << endl;
        return;
    }

    if (P->esq != NULL) {
        deletaHeap(P->esq);
    }
    cout << P->index << ":" << P->info << endl;
    if (P->dir != NULL) {
        deletaHeap(P->dir);
    }
    return;
}
