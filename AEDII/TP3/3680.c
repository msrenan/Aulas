#include <stdio.h>
#include <stdlib.h>

#define MIN_INF -20000000000000000000000000000000

int *head;
int *to;
long long *peso;
int *nxt;
long long *dp;
int *in_degree;
int *fila;
int *top_order;
int edge_cnt;

void add_edge(int u, int v, long long w) {
    to[edge_cnt] = v;
    peso[edge_cnt] = w;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt++;
    in_degree[v]++;
}

int main() {
    int n, m;
    long long v;

    while (scanf("%d %d %lld", &n, &m, &v) != EOF) {
        edge_cnt = 0;

        head = (int *)malloc(n * sizeof(int));
        dp = (long long *)malloc(n * sizeof(long long));
        in_degree = (int *)calloc(n, sizeof(int));
        fila = (int *)malloc(n * sizeof(int));
        top_order = (int *)malloc(n * sizeof(int));
        
        to = (int *)malloc(m * sizeof(int));
        peso = (long long *)malloc(m * sizeof(long long));
        nxt = (int *)malloc(m * sizeof(int));

        for (int i = 0; i < n; i++) {
            head[i] = -1;
            dp[i] = MIN_INF;
        }

        for (int i = 0; i < m; i++) {
            int u, w;
            long long p;
            scanf("%d %d %lld", &u, &w, &p);
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
                    long long custo = peso[e];
                    if (dp[u] + custo > dp[vizinho]) {
                        dp[vizinho] = dp[u] + custo;
                    }
                }
            }
        }

        printf("%lld\n", dp[n - 1]);

        free(head);
        free(dp);
        free(in_degree);
        free(fila);
        free(top_order);
        free(to);
        free(peso);
        free(nxt);
    }

    return 0;
}