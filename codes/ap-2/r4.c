// Complexidade total = O(n x n!)

/* Gera todas as permutações de 1 a n, funciona melhor em numeros pequenos, a partir de 9 a maquina começa a ficar lenta para imprimir N<= 8, e 8! = 40.320*/

#include <stdio.h>

int quantidadeDeNumeros;   
int permutacaoAtual[9];    
int numeroJaUsado[9];     

void imprimirPermutacao() // Custo O(n) para printar
{
    for(int i = 0; i < quantidadeDeNumeros; i++)
    {
        if(i > 0)
        {
            printf(" ");
        }

        printf("%d", permutacaoAtual[i]);
    }

    printf("\n");
}

void gerarPermutacoes(int posicao)
{
    if(posicao == quantidadeDeNumeros) // ponto de parada /caso base
    {
        imprimirPermutacao();
        return;
    }

    for(int numero = 1; numero <= quantidadeDeNumeros; numero++)
    {
        if(numeroJaUsado[numero]) // poda da arvore
        {
            continue;
        }

        permutacaoAtual[posicao] = numero;
        // Marca o número como usado
        numeroJaUsado[numero] = 1;

        gerarPermutacoes(posicao + 1);

        // Desmarca o número como usado
        numeroJaUsado[numero] = 0; //backtracking
    }
}

int main()
{
    scanf("%d", &quantidadeDeNumeros);

    gerarPermutacoes(0);
}