#include <stdio.h>
#include <stdlib.h>

int encontrarSaida(char **labirinto, int N, int linha, int coluna)
{
    if (linha < 0 || linha >= N || coluna < 0 || coluna >= N ||
        labirinto[linha][coluna] == '0')
    {
        return 0;
    }

    if (linha == N - 1 && coluna == N - 1)
    {
        return 1;
    }

    labirinto[linha][coluna] = '0';

    return encontrarSaida(labirinto, N, linha - 1, coluna) ||
          encontrarSaida(labirinto, N, linha + 1, coluna) ||
          encontrarSaida(labirinto, N, linha, coluna - 1) ||
          encontrarSaida(labirinto, N, linha, coluna + 1);
}

int main(void)
{
    int N;
    scanf("%d", &N);

    char **labirinto = malloc(N * sizeof(char *));

    for (int i = 0; i < N; i++)
    {
        labirinto[i] = malloc((N + 1) * sizeof(char));
        scanf("%s", labirinto[i]);
    }

    printf("%d\n", encontrarSaida(labirinto, N, 0, 0));

    for (int i = 0; i < N; i++)
    {
        free(labirinto[i]);
    }
    free(labirinto);

    return 0;
}
