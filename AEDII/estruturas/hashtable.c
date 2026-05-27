#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* Chave;

int hash(Chave chave, int M) {
    int i, h = 0;
    int primo = 127;
    for (i = 0; chave[i] != '\0'; i++)
    h = (h * primo + chave[i]) % M;
    printf("Hash ok!\n");
    return h;
}

struct cell {
    Chave key;
    int valor;
    struct cell* next;
} typedef Cell;

static Cell **table;
static int nKeys = 0;
static int M;

void init(int max) {
    int h;
    M = max;
    nKeys = 0;
    table = malloc(M * sizeof(Cell *));

    for (h = 0; h < M; h++) {
        table[h] = NULL;
    }
}

int search(Chave k) {
    Cell *p;
    int h = hash(k, M);

    p = table[h];

    while (p != NULL && strcmp(p->key, k) != 0) {
        p = p->next;
    }

    if (p != NULL) {
        return p->valor;
    }

    return 0;
}

void insert(Chave k, int v) {
    Cell *p;

    int h = hash(k, M);

    p = table[h];

    while (p != NULL && strcmp(p->key, k) != 0) {
        p = p->next;
    }

    if (p == NULL) {
        p = malloc(sizeof(*p));
        p->key = malloc((strlen(k) + 1) * sizeof(char));
        strcpy((*p).key, k);
        

        nKeys += 1;
        p->next = table[h];

        table[h] = p;
    }
    p->valor = v;
}

void delete(Chave k) {
    Cell *p, *aux;

    int h = hash(k, M);

    p = table[h];

    if (p == NULL) {
        return;
    }

    if (strcmp(p->key, k) == 0) {
        table[h] = p->next;
        free(p->key);
        free(p);
        nKeys--;
        return;
    }

    while (p->next != NULL && strcmp((p->next)->key, k) != 0) {
        p = p->next;
    }

    if (p->next != NULL) {
        aux = p->next;
        p->next = aux->next;
        free(aux->key);
        free(aux);
        nKeys--;
    }
}

void Free() {
    Cell *p = NULL, *q = NULL;

    int h;
    for (h = 0; h < M; h++) {
        p = table[h];
        while (p != NULL) {
            q = p;
            p = p->next;
            free(q->key);
            free(q);
        }
    }

    free(table);
    table = NULL;
    nKeys = 0;
}






