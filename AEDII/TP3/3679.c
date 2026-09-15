#include <stdio.h>
#include <stdlib.h>

#define MIN_INF -2000000000

int *head;
int *to;
int *peso;
int *nxt;
int *dp;
int *in_degree;
int *fila;
int *top_order;
int edge_cnt = 0;

void add_edge(int u, int v, int w) {
    to[edge_cnt] = v;
    peso[edge_cnt] = w;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt++;
    in_degree[v]++;
}

int main() {
    int n, m, v;

    scanf("%d %d %d", &n, &m, &v);

    head = (int *)malloc(n * sizeof(int));
    dp = (int *)malloc(n * sizeof(int));
    in_degree = (int *)calloc(n, sizeof(int));
    fila = (int *)malloc(n * sizeof(int));
    top_order = (int *)malloc(n * sizeof(int));
    
    to = (int *)malloc(m * sizeof(int));
    peso = (int *)malloc(m * sizeof(int));
    nxt = (int *)malloc(m * sizeof(int));

    for (int i = 0; i < n; i++) {
        head[i] = -1;
        dp[i] = MIN_INF;
    }

    for (int i = 0; i < m; i++) {
        int u, w, p;
        scanf("%d %d %d", &u, &w, &p);
        add_edge(u, w, p);
    }

    int q_head = 0, q_tail = 0;
    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0) {
            fila[q_tail++] = i;
        }
    }

    int top_idx = 0;
    while (q_head < q_tail) {
        int u = fila[q_head++];
        top_order[top_idx++] = u;
        
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int vizinho = to[e];
            in_degree[vizinho]--;
            if (in_degree[vizinho] == 0) {
                fila[q_tail++] = vizinho;
            }
        }
    }

    dp[0] = v;

    for (int i = 0; i < n; i++) {
        int u = top_order[i];
        if (dp[u] != MIN_INF) {
            for (int e = head[u]; e != -1; e = nxt[e]) {
                int vizinho = to[e];
                int custo = peso[e];
                if (dp[u] + custo > dp[vizinho]) {
                    dp[vizinho] = dp[u] + custo;
                }
            }
        }
    }

    printf("%d\n", dp[n - 1]);
}