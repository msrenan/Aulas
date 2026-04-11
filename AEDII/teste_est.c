#include <stdio.h>
#include "estruturas/vetor.h"

int main() {
    struct Vec* v = newVec();

    push(v, 1);
    push(v, 2);
    push(v, 3);
    push(v, 4);
    push(v, 5);

    mostra_vec(*v);

    int r;
    pop(v, &r);

    printf("Removed: %d\n", r);

    mostra_vec(*v);

    remove_pos(v, 1, &r);

    printf("Removed: %d\n", r);

    mostra_vec(*v);

    troca(getNode(v, 0), getNode(v, v->size - 1));

    printf("Element #1: %d\n", getElement(v, 1));

    mostra_vec(*v);

    struct Node* n = getNode(v, 0);

    mostra_node(*n);

    delVec(v);
}