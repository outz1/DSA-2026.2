#include <stdio.h>

#define MAX 1048576   /* 2^20: os valores vao de 0 a MAX - 1 */
#define MAXN 15000000 /* N maximo do enunciado */
#define MOD 1000000007LL

/* globais: ficam fora da pilha e ja comecam zerados */
int vetor[MAXN];
int contagem[MAX];

/* counting sort: conta cada valor e reescreve o vetor em ordem crescente */
void counting_sort(int v[], int n) {
    int k = 0;

    for (int i = 0; i < n; i++){
        contagem[v[i]]++;
    }    

    for (int valor = 0; valor < MAX; valor++) {
        while (contagem[valor] > 0) { /* cada ocorrencia ocupa uma posicao */
            v[k] = valor;
            k++;
            contagem[valor]--;
        }
    }
}

int main() {
    int N;
    unsigned int S, c; /* o enunciado exige 32 bits sem sinal */
    long long resposta = 0;

    scanf("%d %u", &N, &S);

    /* 1. gera o vetor */
    c = S;
    for (int i = 0; i < N; i++) {
        c = c ^ (c << 13);
        c = c ^ (c >> 17);
        c = c ^ (c << 5);
        vetor[i] = c % MAX;
    }

    /* 2. ordena */
    counting_sort(vetor, N);

    /* 3. soma i * y[i], com i comecando em 1 */
    for (int i = 0; i < N; i++){
        resposta = (resposta + (long long)(i + 1) * vetor[i]) % MOD;
    }  

    printf("%lld\n", resposta);

    return 0;
}
