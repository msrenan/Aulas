#include <stdio.h>
#include "algoritmos/mergesort.h"
#include "estruturas/vetor.h"
#include "random.h"

int main() {
    struct Vec *v = newVec();

    preenche(v);

    mostra_vec(*v, 0, v->size - 1);

    merge_sort(v);

    printf("---------------\n");

    mostra_vec(*v, 0, v->size - 1);

    delVec(v);
}