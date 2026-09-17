#include <stdio.h>

#define TAMANHO_MAXIMO 1000

int matriz[TAMANHO_MAXIMO][TAMANHO_MAXIMO];

int buscaSaddleback(int tamanhoDaMatriz, int valorProcurado)
{
    int linha = 0;
    int coluna = tamanhoDaMatriz - 1;

    while (linha < tamanhoDaMatriz && coluna >= 0)
    {
        int valorAtual = matriz[linha][coluna];

        if (valorAtual == valorProcurado)
        {
            return 1;
        }

        if (valorAtual > valorProcurado) // elimina a coluna
        {
            coluna--;
        }
        else // elimina a linha (valorAtual < valorProcurado)
        {
            linha++;
        }
    }

    return 0;
}

int main(void)
{
    int tamanhoDaMatriz;
    int quantidadeDeConsultas;

    scanf("%d %d", &tamanhoDaMatriz, &quantidadeDeConsultas);

    for (int linha = 0; linha < tamanhoDaMatriz; linha++)
    {
        for (int coluna = 0; coluna < tamanhoDaMatriz; coluna++)
        {
            scanf("%d", &matriz[linha][coluna]);
        }
    }

    for (int consulta = 0; consulta < quantidadeDeConsultas; consulta++)
    {
        int valorProcurado;

        scanf("%d", &valorProcurado);

        printf("%d\n", buscaSaddleback(tamanhoDaMatriz, valorProcurado));
    }

    return 0;
}
