#include <stdio.h>
#include <stdlib.h>

// #define MAX 100

void imprime(int v[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

int main(int argc, char *argv[])
{
    // char file_name[MAX];
    // FILE *entrada;
    int i, n;

    if (argc != 1)
    {
        printf("Numero incorreto de parametros. Ex: .\\nome_arquivo_executavel\n");
        return 0;
    }
    // scanf("%s", file_name);
    // entrada = fopen(file_name, "r");
    // if (entrada == NULL)
    // {
    //     printf("\nNao encontrei o arquivo! Informe o nome da instancia\n");
    //     exit(EXIT_FAILURE);
    // }

    // lendo do arquivo da instância
    // fscanf(entrada, "%d", &n);
    // int *v = (int *)malloc(n * sizeof(int));
    // for (i = 0; i < n; i++)
    //     fscanf(entrada, "%d", &v[i]);

    // lendo o tamanho de um vetor e os elementos do mesmo
    scanf("%d", &n);
    int *v = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        scanf("%d", &v[i]);

    // crie uma funcao que resolva o problema

    printf("%d\n", n);
    imprime(v, n);

    free(v);

    // fclose(entrada);
    return 0;
}
