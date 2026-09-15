#include <stdio.h>
#include <stdlib.h>

#define MIN_INF -2000000000

int *head;
int *to;
int *peso;
int *nxt;
int *dp;
int edge_cnt = 0;

void add_edge(int u, int v, int w) {
    to[edge_cnt] = v;
    peso[edge_cnt] = w;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt++;
}

int main() {
    int n, m, v;

    scanf("%d %d %d", &n, &m, &v);


    head = (int *)malloc(n * sizeof(int));
    dp = (int *)malloc(n * sizeof(int));
    
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

    dp[0] = v;

    for (int i = 0; i < n; i++) {
        if (dp[i] != MIN_INF) {
            for (int e = head[i]; e != -1; e = nxt[e]) {
                int vizinho = to[e];
                int custo = peso[e];
                if (dp[i] + custo > dp[vizinho]) {
                    dp[vizinho] = dp[i] + custo;
                }
            }
        }
    }

    printf("%d\n", dp[n - 1]);
}