#include <stdio.h>

int N;
long long numeros[24];

int existeSubconjunto(int posicao, long long restante)
{
    if (restante == 0)
    {
        return 1;
    }

    if (posicao == N || restante < 0)
    {
        return 0;
    }

    return existeSubconjunto(posicao + 1, restante - numeros[posicao]) ||
        existeSubconjunto(posicao + 1, restante);
}

int main(void)
{
    long long alvo;

    scanf("%d %lld", &N, &alvo);

    for (int i = 0; i < N; i++)
    {
        scanf("%lld", &numeros[i]);
    }

    printf("%d\n", existeSubconjunto(0, alvo));

    return 0;
}
