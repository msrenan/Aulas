#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void quickSort(int *v, int esq, int dir) {
    if (esq >= dir) return;
    int i = esq, j = dir;
    int pivo = v[(esq + dir) / 2];
    while (i <= j) {
        while (v[i] < pivo) i++;
        while (v[j] > pivo) j--;
        if (i <= j) {
            swap(&v[i], &v[j]);
            i++; j--;
        }
    }
    quickSort(v, esq, j);
    quickSort(v, i, dir);
}

int main() {
    int nX, nY;
    if (scanf("%d %d", &nX, &nY) != 2) return 0;

    int *X = malloc(nX * sizeof(int));
    int *Y = malloc(nY * sizeof(int));

    for (int i = 0; i < nX; i++) scanf("%d", &X[i]);
    for (int i = 0; i < nY; i++) scanf("%d", &Y[i]);

    quickSort(X, 0, nX - 1);
    quickSort(Y, 0, nY - 1);

    int mX = X[(nX - 1) / 2];
    int mY = Y[(nY - 1) / 2];
    int alvo = mX + mY;

    long long totalPares = 0;
    int i = 0, k = 0;

    while (i < nX) {
        int valX = X[i], cX = 0;
        while (i < nX && X[i] == valX) { cX++; i++; }

        int busca = alvo - valX;
        int cY = 0;

        int low = 0, high = nY - 1, pos = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (Y[mid] == busca) { pos = mid; break; }
            if (Y[mid] < busca) low = mid + 1;
            else high = mid - 1;
        }

        if (pos != -1) {
            int p = pos;
            while (p >= 0 && Y[p] == busca) { cY++; p--; }
            p = pos + 1;
            while (p < nY && Y[p] == busca) { cY++; p++; }
        }
        totalPares += (cX < cY) ? cX : cY;
    }
    printf("%d\n", totalPares);
    free(X); 
    free(Y);
}