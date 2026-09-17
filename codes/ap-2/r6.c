#include <stdio.h>
#include <stdlib.h>

long long contarRainhas(int linha, int N, int *colunas,
                        int *diagonaisPrincipais, int *diagonaisSecundarias)
{
    if (linha == N)
    {
        return 1;
    }

    long long quantidade = 0;

    for (int coluna = 0; coluna < N; coluna++)
    {
        int diagonalPrincipal = linha - coluna + N - 1;
        int diagonalSecundaria = linha + coluna;

        if (colunas[coluna] || diagonaisPrincipais[diagonalPrincipal] ||
            diagonaisSecundarias[diagonalSecundaria])
        {
            continue;
        }

        colunas[coluna] = 1;
        diagonaisPrincipais[diagonalPrincipal] = 1;
        diagonaisSecundarias[diagonalSecundaria] = 1;

        quantidade += contarRainhas(linha + 1, N, colunas,
                                    diagonaisPrincipais, diagonaisSecundarias);

        colunas[coluna] = 0;
        diagonaisPrincipais[diagonalPrincipal] = 0;
        diagonaisSecundarias[diagonalSecundaria] = 0;
    }

    return quantidade;
}

int main(void)
{
    int N;
    scanf("%d", &N);

    int *colunas = calloc(N, sizeof(int));
    int *diagonaisPrincipais = calloc(2 * N - 1, sizeof(int));
    int *diagonaisSecundarias = calloc(2 * N - 1, sizeof(int));

    printf("%lld\n", contarRainhas(0, N, colunas,
                                diagonaisPrincipais, diagonaisSecundarias));

    free(colunas);
    free(diagonaisPrincipais);
    free(diagonaisSecundarias);

    return 0;
}
