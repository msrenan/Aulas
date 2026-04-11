#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    int value;
    struct Node* next;
    struct Node* prev;
    int index;
};

struct Node* newNode() {
    struct Node* n = malloc(sizeof(struct Node));
    n->index = -1;
    n->next = NULL;
    n->prev = NULL;
    n->value = 0;
    return n;
}

struct Vec {
    struct Node* start;
    struct Node* end;
    int size;
};

struct Vec* newVec() {
    struct Vec* v = malloc(sizeof(struct Vec));
    v->size = 0;
    v->start = NULL;
    v->end = NULL;
    return v;
}

void delVec(struct Vec* v) {
    struct Node* aux = v->start;
    struct Node* tmp;

    while (aux != NULL) {
        tmp = aux;
        aux = aux->next;
        free(tmp);
    }

    free(v);
    printf("Vetor deletado com sucesso.\n");
}

bool push(struct Vec* v, int x) {
    struct Node* aux = newNode();
    aux->value = x;
    aux->index = v->size;
    if (v->size == 0) {
        aux->next = NULL;
        aux->prev = NULL;
        v->start = aux;
        v->end = aux;
    } else {
        aux->prev = v->end;
        v->end->next = aux;
        aux->next = NULL;
        v->end = aux;
    }

    v->size++;
    return true;
}

bool pop(struct Vec* v, int* e) {
    if (v->size == 0) {
        return false;
    }

    *e = v->end->value;
    struct Node* aux = v->end;
    v->end = aux->prev;
    v->end->next = NULL;
    free(aux);
    v->size--;
    return true;
}



bool remove_pos(struct Vec* v, int i, int* e) {
    if (v->size == 0 || i < 0 || i > v->size - 1) return false;

    if (i == v->size - 1) return pop(v, e);

    struct Node *aux, *deletedNode;
    int swap, aux_i;
    if ((v->size - 1) - i < i) {
        aux = v->end;

        while(aux != NULL && aux->index != i) aux = aux->prev;

        if (aux == NULL) {
            return false;
        }

        aux->next->prev == aux->prev;

        if (aux != v->start) {
            aux->prev->next = aux->next;
        }

        *e = aux->value;
        aux_i = aux->index;
        deletedNode = aux;
    } else {
        aux = v->start;

        while (aux != NULL && aux->index != i) aux = aux->next;

        if (aux == NULL) {
            return false;
        }

        if (aux != v->end) {
            aux->prev->next = aux->next;
        }

        aux->next->prev = aux->prev;

        *e = aux->value;
        aux_i = aux->index;
        deletedNode = aux;
    }
    
    while (aux->next != NULL) {
        aux = aux->next;
        swap = aux->index;
        aux->index = aux_i;
        aux_i = swap;
    }

    v->size--;
    free(deletedNode);
    return true;
}

void mostra_vec(struct Vec v, int min, int max) {
    if (min > max) return;
    struct Node* current = v.start;
    printf("[ ");
    while (current != NULL && current->index <= max) {
        if (current->index < min) {
            current = current->next;
            continue;
        }
        printf("%d ", current->value);
        current = current->next;
    }
    printf("]\n");
}


void mostra_node(struct Node n) {
    bool next = false, prev = false;
    if (n.next != NULL) next = true;
    if (n.prev != NULL) prev = true;
    printf("{ Value:%d Index:%d Next?:%d Prev?:%d }\n", n.value, n.index, next, prev);
}

void troca(struct Node* a, struct Node* b) {
    int swap = a->value;
    a->value = b->value;
    b->value = swap;
}

struct Node* getNode(struct Vec *v, int i) {
    if (i < 0 || i > v->size - 1 || v->size == 0) return NULL;
    struct Node* aux;
    if ((v->size - 1) - i < i) {
        aux = v->end;

        while (aux != NULL && aux->index != i) aux = aux->prev;

        
    } else {
        aux = v->start;

        while (aux != NULL && aux->index != i) aux = aux->next;
    }

    if (aux == NULL) return NULL;

    return aux;
}

int getElement(struct Vec *v, int i) {
    if (i < 0 || i > v->size - 1 || v->size == 0) return -1;
    struct Node* aux;
    if ((v->size - 1) - i < i) {
        aux = v->end;

        while (aux != NULL && aux->index != i) aux = aux->prev;

        
    } else {
        aux = v->start;

        while (aux != NULL && aux->index != i) aux = aux->next;
    }

    if (aux == NULL) return -1;

    return aux->value;
}

void preenche(struct Vec *v) {
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        int j = randInt(0, 100);
        push(v, j);
    }
}