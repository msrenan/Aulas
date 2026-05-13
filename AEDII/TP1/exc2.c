#include <stdio.h>
#include <stdlib.h>

struct { 
    int id, 
    pos; 
} typedef Voto;

struct { 
    int id, 
    votos, 
    ultimaPos; 
} typedef  Candidato;

void qSortVotos(Voto *v, int esq, int dir) {
    if (esq >= dir) return;
    int i = esq, j = dir, pivo = v[(esq + dir) / 2].id;
    while (i <= j) {
        while (v[i].id < pivo) i++;
        while (v[j].id > pivo) j--;
        if (i <= j) {
            Voto t = v[i]; v[i] = v[j]; v[j] = t;
            i++; j--;
        }
    }
    qSortVotos(v, esq, j); qSortVotos(v, i, dir);
}

void qSortCandidatos(Candidato *v, int esq, int dir) {
    if (esq >= dir) return;
    int i = esq, j = dir;
    Candidato pivo = v[(esq + dir) / 2];
    while (i <= j) {
        while (v[i].votos > pivo.votos || (v[i].votos == pivo.votos && v[i].ultimaPos < pivo.ultimaPos)) i++;
        while (v[j].votos < pivo.votos || (v[j].votos == pivo.votos && v[j].ultimaPos > pivo.ultimaPos)) j--;
        if (i <= j) {
            Candidato t = v[i]; v[i] = v[j]; v[j] = t;
            i++; j--;
        }
    }
    qSortCandidatos(v, esq, j); qSortCandidatos(v, i, dir);
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    Voto *vts = malloc(n * sizeof(Voto));
    for (int i = 0; i < n; i++) {
        scanf("%d", &vts[i].id);
        vts[i].pos = i;
    }

    qSortVotos(vts, 0, n - 1);

    Candidato *cands = malloc(n * sizeof(Candidato));
    int totalC = 0;
    for (int i = 0; i < n; ) {
        int id = vts[i].id, count = 0, last = -1;
        while (i < n && vts[i].id == id) {
            count++;
            if (vts[i].pos > last) last = vts[i].pos;
            i++;
        }
        cands[totalC++] = (Candidato){id, count, last};
    }

    qSortCandidatos(cands, 0, totalC - 1);

    for (int i = 0; i < k && i < totalC; i++)
        printf("%d: %d\n", cands[i].id, cands[i].votos);

    free(vts); 
    free(cands);
}