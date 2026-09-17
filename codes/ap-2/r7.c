/* TIPO DE RECURSÃO -> LINEAR
    A complexidade do tempo é O(C(n, k)), onde C(n, k) é o coeficiente binomial, pois estamos gerando todas as combinações possíveis de k elementos escolhidos de um conjunto de n elementos. A complexidade do espaço é O(k), pois a profundidade máxima da recursão é k.

    Comp. Tempo = O(C(n, k))
    Comp. Espaço = O(k)
*/
#include <stdio.h>

int N;
int K;
int combinacao[18];

void imprimirCombinacao(void)
{
	for (int i = 0; i < K; i++)
	{
		if (i > 0)
		{
			printf(" ");
		}

		printf("%d", combinacao[i]);
	}

	printf("\n");
}

void gerarCombinacoes(int inicio, int posicao)
{
	if (posicao == K) // ponto de parada
	{
		imprimirCombinacao();
		return;
	}

	int elementosRestantes = K - posicao;
	int ultimoPossivel = N - elementosRestantes + 1;

	for (int numero = inicio; numero <= ultimoPossivel; numero++)
	{
		combinacao[posicao] = numero;
		gerarCombinacoes(numero + 1, posicao + 1);
	}
}

int main(void)
{
	scanf("%d %d", &N, &K);
	gerarCombinacoes(1, 0);

	return 0;
}
