// basicamente um algoritmo de busca em profundidade (DFS) - mas com exclusividade para matrizes quadradas

// complexidade temporal de O(NxN) = O(N²) pq a matriz é quadrada
#include <stdio.h>

int N;
int labirinto[100][100];
int visitado[100][100];

int encontrarSaida(int linha, int coluna)
{
    if (linha < 0 || linha >= N || coluna < 0 || coluna >= N) // verificar se esta dentro da matriz (caso de parada)
    {
    return 0;
    }

    if (labirinto[linha][coluna] == 0) // verificar se é parede (caso de parada)
    {
        return 0;
    }

    if (visitado[linha][coluna]) // verificar se já foi visitado (caso de parada) - impedir que se ande em circulos, nao é bem um backtracking por que ele nao se desfaz das escolhas, mas sim marca como visitado e nao volta para ele
    {
        return 0;
    }

    if (linha == N - 1 && coluna == N - 1) // caso base de sucesso (verificar se é a saida) / ultima posicao da matriz, se chegou nela, significa que encontrou a saida 

    /*
    (0,0) (0,1) (0,2)
    (1,0) (1,1) (1,2)
    (2,0) (2,1) (2,2)
    Saida = (2,2)
    
    */
    {
        return 1;
    }

    visitado[linha][coluna] = 1; // marcar cada celula que anda como visitado (faz ela cair no if)

    // recursao do codigo (faz andar), faz ele tentar andar em cada uma das direcoes, se alguma delas retornar 1, significa que encontrou a saida e retorna 1
    if (encontrarSaida(linha - 1, coluna) || // cima
        encontrarSaida(linha + 1, coluna) || // baixo
        encontrarSaida(linha, coluna - 1) || // esquerda
        encontrarSaida(linha, coluna + 1)  // direita
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