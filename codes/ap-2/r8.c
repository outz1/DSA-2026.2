#include <stdio.h>

int N;
int labirinto[100][100];
int visitado[100][100];

int encontrarSaida(int linha, int coluna)
{
    if (linha < 0 || linha >= N || coluna < 0 || coluna >= N)
    {
    return 0;
    }

    if (labirinto[linha][coluna] == 0)
    {
        return 0;
    }

    if (visitado[linha][coluna])
    {
        return 0;
    }

    if (linha == N - 1 && coluna == N - 1)
    {
        return 1;
    }

    visitado[linha][coluna] = 1;

    if (encontrarSaida(linha - 1, coluna) ||
        encontrarSaida(linha + 1, coluna) ||
        encontrarSaida(linha, coluna - 1) ||
        encontrarSaida(linha, coluna + 1)  
        )
    {
        return 1;
    }

    return 0;
}

int main(void)
{
    scanf("%d", &N);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            scanf("%1d", &labirinto[i][j]);
        }
    }

    printf("%d\n", encontrarSaida(0, 0));

    return 0;
}