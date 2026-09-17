#include <stdio.h>

int quantidadeDeNumeros;   
int permutacaoAtual[9];    
int numeroJaUsado[9];     

void imprimirPermutacao()
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
    if(posicao == quantidadeDeNumeros)
    {
        imprimirPermutacao();
        return;
    }

    for(int numero = 1; numero <= quantidadeDeNumeros; numero++)
    {
        if(numeroJaUsado[numero])
        {
            continue;
        }

        permutacaoAtual[posicao] = numero;
        numeroJaUsado[numero] = 1;

        gerarPermutacoes(posicao + 1);

        numeroJaUsado[numero] = 0;
    }
}

int main()
{
    scanf("%d", &quantidadeDeNumeros);

    gerarPermutacoes(0);
}
