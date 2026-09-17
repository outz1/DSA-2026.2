/*
    Esse código possui custo O(n), mas o normal é custo O(2^n), pois a função hanoi é recursiva e faz chamadas para si mesma duas vezes para cada valor de n, resultando em um crescimento exponencial do número de chamadas à medida que n aumenta. No entanto, a implementação atual calcula o número mínimo de movimentos necessários para resolver o problema das Torres de Hanoi de forma eficiente, utilizando a fórmula matemática conhecida para o problema, que é 2^n - 1.
*/

/* COMPLEXIDADE -- TEMPO = O(n) ESPAÇO = O(n) 
    TIPO DE RECURSÃO -> LINEAR
*/
#include <stdio.h>

long long int hanoi(int n) {
    if (n == 0) {
        return 0;
    }

    return 2 * hanoi(n - 1) + 1;
}

int main(void) {
    int N;

    scanf("%d", &N);

    printf("%lld\n", hanoi(N));

    return 0;
}