#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int *temperaturas = (int *)malloc(n * sizeof(int));
    if (temperaturas == NULL) {
        return 1;
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &temperaturas[i]);
    }

    int x;
    if (scanf("%d", &x) != 1) {
        free(temperaturas);
        return 0;
    }

    int max_delta = 0; 
    int min_delta = 0; 

    int anterior = 0;
    int primeiro_valido = 1;

    for (int i = 0; i < n; i++) {
        if (temperaturas[i] == x) {
            continue;
        }

        if (primeiro_valido) {
            anterior = temperaturas[i];
            primeiro_valido = 0;
        } else {
            int delta = temperaturas[i] - anterior;

            if (delta > max_delta) {
                max_delta = delta;
            }
            if (delta < min_delta) {
                min_delta = delta;
            }

            anterior = temperaturas[i];
        }
    }

    printf("%d %d\n", max_delta, min_delta);

    free(temperaturas);
}