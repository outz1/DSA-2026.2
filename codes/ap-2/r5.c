/* TIPO DE RECURSÃO -> BINÁRIA 
    Para cada elemento existem duas decisões, incluir ou excluir, o que gera uma árvore binária de decisões. A complexidade do tempo é O(2^n), pois para cada elemento existem duas possibilidades, e a complexidade do espaço é O(n), pois a profundidade máxima da recursão é n.

    Comp. Tempo = O(2^n)
    Comp. Espaço = O(n)
*/
#include <stdio.h>

int N;
long long numeros[24];

int existeSubconjunto(int posicao, long long restante)
{
    if (restante == 0) // ponto de parada 1
    {
        return 1;
    }

    if (posicao == N || restante < 0) // ponto de parada 2
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
