#include <stdio.h>
#include <stdlib.h>

int buscar_na_linha(const long long *linha, int tamanho, long long chave) {
    int esquerda = 0;
    int direita = tamanho - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;

        if (linha[meio] == chave) {
            return 1;
        }

        if (linha[meio] < chave) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return 0;
}

int buscar_na_matriz(const long long *matriz, int linhas, int colunas, long long chave) {
    for (int i = 0; i < linhas; i++) {
        const long long *linha = matriz + (size_t)i * colunas;

        if (chave < linha[0] || chave > linha[colunas - 1]) {
            continue;
        }

        if (buscar_na_linha(linha, colunas, chave)) {
            return 1;
        }
    }

    return 0;
}

int main(void) {
    int m;
    int n;
    int q;

    scanf("%d %d %d", &m, &n, &q);

    long long *matriz = malloc((size_t)m * n * sizeof(long long));
    if (matriz == NULL) {
        return 1;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%lld", &matriz[(size_t)i * n + j]);
        }
    }

    for (int i = 0; i < q; i++) {
        long long chave;
        scanf("%lld", &chave);
        printf("%d\n", buscar_na_matriz(matriz, m, n, chave));
    }

    free(matriz);
    return 0;
}
