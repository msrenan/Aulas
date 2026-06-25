#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int *autorizados = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &autorizados[i]);
    }

    int m;
    if (scanf("%d", &m) != 1) return 0;

    int mediana = n / 2;
    int limite = n / 4;

    for (int i = 0; i < m; i++) {
        int pacote;
        scanf("%d", &pacote);

        int esq = 0;
        int dir = n - 1;
        int pos = -1;

        while (esq <= dir) {
            int meio = (esq + dir) / 2;
            
            if (autorizados[meio] == pacote) {
                pos = meio;
                break;
            } else if (autorizados[meio] < pacote) {
                esq = meio + 1;
            } else {
                dir = meio - 1;
            }
        }

        int resultado;
        
        if (pos == -1) {
            resultado = -1;
        } else {
            int dist = pos - mediana;
            if (dist < 0) {
                dist = -dist;
            }

            if (dist <= limite) {
                resultado = 1;
            } else {
                resultado = 0;
            }
        }

        if (i == m - 1) {
            printf("%d\n", resultado);
        } else {
            printf("%d ", resultado);
        }
    }

    free(autorizados);
    return 0;
}