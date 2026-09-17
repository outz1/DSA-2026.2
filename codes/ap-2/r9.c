#include <stdio.h>

long long memoria[81][81];

long long contarParticoes(int restante, int maiorParcela)
{
    if (restante == 0)
    {
        return 1;
    }

    if (memoria[restante][maiorParcela] != 0)
    {
        return memoria[restante][maiorParcela];
    }

    long long quantidade = 0;
    int limite = restante < maiorParcela ? restante : maiorParcela;

    for (int parcela = limite; parcela >= 1; parcela--)
    {
        quantidade += contarParticoes(restante - parcela, parcela);
    }

    memoria[restante][maiorParcela] = quantidade;
    return memoria[restante][maiorParcela];
}

int main(void)
{
    int N;
    scanf("%d", &N);

    printf("%lld\n", contarParticoes(N, N));

    return 0;
}
