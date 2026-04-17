#pragma once
#include <stdlib.h>
#include "../estruturas/vetor.h"

void merge(struct Vec *v, int start, int m, int end) {
    struct Vec* aux = newVec();
    int i, j, k, tam;
    i = start;
    j = m;
    k = 0;
    tam = end - start;
    while (i < m && j < end) {
        if (getElement(v, i) <= getElement(v, j)) push(aux, getElement(v, i++));
        else push(aux, getElement(v, j++));
    }

    while (i < m) push(aux, getElement(v, i++));
    while (j < end) push(aux, getElement(v, j++));
    for (k = 0; k < tam; k++) setElement(getNode(v, start + k), getElement(aux, k));
    delVec(aux);
}

void algorithm(struct Vec* v, int start, int end) {
    int m;
    if (end - start > 1) {
        m = start + ((end - start)/ 2);
        algorithm(v, start, m);
        algorithm(v, m, start);
        merge(v, start, m, end);
    }
}

void merge_sort(struct Vec* v) {
    if (v->size > 0) {
        algorithm(v, 0, v->size - 1);
    }
}