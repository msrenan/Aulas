#include <stdio.h>
#include <stdlib.h>

int *pai;
int *tamanho;
int maior_componente = 1;

int find_set(int v) {
    if (v == pai[v])
        return v;
    return pai[v] = find_set(pai[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (tamanho[a] < tamanho[b]) {
            int temp = a;
            a = b;
            b = temp;
        }
        pai[b] = a;
        tamanho[a] += tamanho[b];
        if (tamanho[a] > maior_componente) {
            maior_componente = tamanho[a];
        }
    }
}

int main() {
    int n, m;
    pai = (int *)malloc(n * sizeof(int));
    tamanho = (int *)malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        pai[i] = i;
        tamanho[i] = 1;
    }
    
    if (n > 0 && m == 0) {
        printf("1\n");
        return 0;
    }
    
    for (int i = 0; i < m; i++) {
        int u, w;
        scanf("%d %d", &u, &w);
        union_sets(u, w);
    }
    
    printf("%d\n", maior_componente);
}