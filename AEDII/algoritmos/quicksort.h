#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "../random.h"
#include "../estruturas/vetor.h"

int sep(struct Vec *v, int start, int end) {
    int i, j;
    int c = getElement(v, end);
    i = start;
    for (j = start; j < end; j++) {
        if (getElement(v, j) <= c) {
            troca(getNode(v, i), getNode(v, j));
            i++;
        }
    }
    troca(getNode(v, i), getNode(v, end));
    return i;
}

void algorithm(struct Vec *v, int start, int end, int call) {
    int off, sep_i;
    if (end > start) {
        off = randInt(0, end - start);
        troca(getNode(v, (start + off)), getNode(v, end));
        sep_i = sep(v, start, end);
        call++;
        algorithm(v, start, sep_i - 1, call);
        algorithm(v, sep_i + 1, end, call);
    }
    return;
}


bool quick_sort(struct Vec* v) {
    if (v->size == 0) return false;

    if (v->size == 1) return true;

    algorithm(v, 0, v->size - 1, 0);
    return true;
}
