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
	if (posicao == K)
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
