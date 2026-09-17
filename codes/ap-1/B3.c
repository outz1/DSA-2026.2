#include <stdio.h>

long long contar_comparacoes(long long tamanho, long long chave) {
    long long esquerda = 0;
    long long direita = tamanho - 1;
    long long comparacoes = 0;

    while (esquerda <= direita) {
        long long meio = esquerda + (direita - esquerda) / 2;
        comparacoes++;

        if (meio == chave) {
            return comparacoes;
        }

        if (meio < chave) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return comparacoes;
}

int main(void) {
    long long n;
    int q;

    scanf("%lld %d", &n, &q);

    for (int i = 0; i < q; i++) {
        long long chave;
        scanf("%lld", &chave);
        printf("%lld\n", contar_comparacoes(n, chave));
    }

    return 0;
}
