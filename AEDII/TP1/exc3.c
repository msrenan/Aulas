#include <stdio.h>
#include <stdlib.h>

int merge(int *arr, int *aux, int esq, int meio, int dir) {
    int i = esq, j = meio + 1, k = esq;
    int inv = 0;

    while (i <= meio && j <= dir) {
        if (arr[i] <= arr[j]) {
            aux[k++] = arr[i++];
        } else {
            aux[k++] = arr[j++];
            inv += (meio - i + 1);
        }
    }
    while (i <= meio) aux[k++] = arr[i++];
    while (j <= dir) aux[k++] = arr[j++];
    for (i = esq; i <= dir; i++) arr[i] = aux[i];
    return inv;
}

long countInversions(int *arr, int *aux, int esq, int dir) {
    if (esq >= dir) return 0;
    int meio = esq + (dir - esq) / 2;
    long inv = 0;
    inv += countInversions(arr, aux, esq, meio);
    inv += countInversions(arr, aux, meio + 1, dir);
    inv += merge(arr, aux, esq, meio, dir);
    return inv;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int *g1_pos = malloc(n * sizeof(int));
    int *g2 = malloc(n * sizeof(int));
    int temp_val;

    for (int i = 0; i < n; i++) {
        scanf("%d", &temp_val);
        g1_pos[temp_val] = i;
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &temp_val);
        g2[i] = g1_pos[temp_val];
    }

    int *aux = malloc(n * sizeof(int));
    printf("%lld\n", countInversions(g2, aux, 0, n - 1));

    free(g1_pos);
    free(g2);
    free(aux);
}