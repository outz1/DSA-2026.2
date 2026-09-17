# Relatório de Estudo: Busca Saddleback em Matriz Ordenada

## 1. Problema e ideia central

O programa resolve o problema B4, **Busca na Matriz**. Ele recebe uma matriz quadrada `N x N` e `Q` consultas. Para cada valor consultado, imprime `1` se o valor aparece na matriz ou `0` caso contrário.

O enunciado oferece duas garantias fundamentais:

- cada linha está em ordem não decrescente, da esquerda para a direita;
- cada coluna está em ordem não decrescente, de cima para baixo.

“Não decrescente” significa que um elemento nunca é menor que o anterior; valores repetidos são permitidos. A busca **saddleback**, também chamada de *staircase search*, explora as duas ordenações simultaneamente. Começando no canto superior direito, cada comparação elimina uma linha ou uma coluna inteira da região onde o valor ainda pode estar.

Limites relevantes:

```text
1 <= N <= 1000
1 <= Q <= 50000
-10^9 <= valores <= 10^9
```

## 2. Código estudado

O código está no arquivo `apresentacao1_saddleback2.c`:

```c
#include <stdio.h>

#define TAMANHO_MAXIMO 1000

int matriz[TAMANHO_MAXIMO][TAMANHO_MAXIMO];

int buscaSaddleback(int tamanhoDaMatriz, int valorProcurado)
{
    int linha = 0;
    int coluna = tamanhoDaMatriz - 1;

    while (linha < tamanhoDaMatriz && coluna >= 0)
    {
        int valorAtual = matriz[linha][coluna];

        if (valorAtual == valorProcurado)
        {
            return 1;
        }

        if (valorAtual > valorProcurado)
        {
            coluna--;
        }
        else
        {
            linha++;
        }
    }

    return 0;
}

int main(void)
{
    int tamanhoDaMatriz;
    int quantidadeDeConsultas;

    scanf("%d %d", &tamanhoDaMatriz, &quantidadeDeConsultas);

    for (int linha = 0; linha < tamanhoDaMatriz; linha++)
    {
        for (int coluna = 0; coluna < tamanhoDaMatriz; coluna++)
        {
            scanf("%d", &matriz[linha][coluna]);
        }
    }

    for (int consulta = 0; consulta < quantidadeDeConsultas; consulta++)
    {
        int valorProcurado;

        scanf("%d", &valorProcurado);

        printf("%d\n", buscaSaddleback(tamanhoDaMatriz, valorProcurado));
    }

    return 0;
}
```

## 3. Bibliotecas

### `#include <stdio.h>`

Declara as funções de entrada e saída:

- `scanf`: lê dados da entrada padrão;
- `printf`: escreve respostas na saída padrão.

Não é necessário incluir `<stdlib.h>`, porque o programa não usa alocação dinâmica nem funções declaradas nesse cabeçalho.

Os cabeçalhos fornecem ao compilador os tipos corretos das funções. Em C moderno, não se deve chamar uma função sem a declaração apropriada.

## 4. Assinatura da busca

```c
int buscaSaddleback(int tamanhoDaMatriz, int valorProcurado)
```

### Retorno `int`

A função responde a uma pergunta booleana:

- `1`: encontrou pelo menos uma ocorrência;
- `0`: não encontrou.

Poderia ser usado `bool`, de `<stdbool.h>`, mas `int` é válido e corresponde diretamente ao formato de saída do problema.

### Matriz global

`matriz` é uma matriz global definida com capacidade máxima de `1000 x 1000`. A função de busca acessa essa matriz diretamente e não precisa receber um ponteiro como parâmetro. Como a função apenas lê os elementos, a matriz não é modificada.

### `int tamanhoDaMatriz`

O tamanho efetivamente usado é passado separadamente. Uma única dimensão basta porque a matriz é quadrada.

### `int valorProcurado`

É uma cópia do valor da consulta. Em C, os argumentos são passados por valor; alterar esse parâmetro não alteraria a variável existente no `main`.

## 5. Matriz bidimensional e memória contígua

Os dados são bidimensionais e o programa os guarda em uma matriz global de tamanho fixo:

```c
#define TAMANHO_MAXIMO 1000
int matriz[TAMANHO_MAXIMO][TAMANHO_MAXIMO];
```

```text
Matriz:                       Memória (row-major):

a00 a01 a02                  a00 a01 a02 a10 a11 a12 a20 a21 a22
a10 a11 a12                  └─ linha 0 ─┘└─ linha 1 ─┘└─ linha 2 ─┘
a20 a21 a22
```

Essa disposição é chamada de **ordem por linhas** (*row-major order*) e coincide com a forma como matrizes nativas de C são armazenadas.

Uma coordenada é acessada diretamente por:

```c
matriz[linha][coluna]
```

Em uma matriz `3 x 3`, a posição `(1, 2)` é acessada como `matriz[1][2]`. O acesso custa `O(1)`, e o compilador calcula internamente a posição correspondente na memória contígua da matriz.

## 6. Por que o canto superior direito?

```c
int linha = 0;
int coluna = tamanho - 1;
```

A posição inicial é:

```text
             início
                ↓
  0   5  [10]
  2   7   12
  7  12   17
```

Nesse canto:

- à esquerda estão valores menores ou iguais;
- abaixo estão valores maiores ou iguais.

Portanto, uma comparação determina uma eliminação segura. O canto inferior esquerdo também funcionaria: se o atual fosse maior, subiríamos; se fosse menor, andaríamos para a direita.

O superior esquerdo e o inferior direito não oferecem a mesma decisão. No superior esquerdo, tanto à direita quanto abaixo ficam valores maiores; a comparação não informa qual caminho escolher.

## 7. Limites e condição do laço

```c
while (linha < tamanho && coluna >= 0)
```

A busca continua enquanto a posição estiver na matriz:

- `linha < tamanho` impede passar da última linha;
- `coluna >= 0` impede passar antes da primeira coluna.

Não é preciso testar `linha >= 0`, pois a linha começa em zero e só aumenta. Não é preciso testar `coluna < tamanho`, pois a coluna começa em `tamanho - 1` e só diminui.

O `&&` usa curto-circuito: se a primeira expressão for falsa, a segunda não precisa ser avaliada. Quando o laço termina, `linha == tamanho` ou `coluna == -1`; a região candidata ficou vazia.

## 8. Os três resultados de uma comparação

```c
int valorAtual = matriz[linha][coluna];
```

### Igual

```c
if (valorAtual == valorProcurado)
    return 1;
```

Uma ocorrência basta. O `return` encerra a função imediatamente, inclusive quando há valores repetidos.

### Atual maior: esquerda

```c
if (valorAtual > valorProcurado)
    coluna--;
```

Todos os elementos abaixo na coluna são maiores ou iguais a `valorAtual`. Se o atual já é grande demais, toda a parte restante dessa coluna também é. A coluna é eliminada.

### Atual menor: baixo

```c
else
    linha++;
```

O `else` significa necessariamente `valorAtual < valorProcurado`, pois a igualdade já foi tratada. Todos os valores à esquerda na linha são menores ou iguais ao atual; se o atual é pequeno demais, o restante relevante da linha também é. A linha é eliminada.

Não há laço infinito: cada iteração retorna, incrementa `linha` ou decrementa `coluna`. Uma posição nunca é visitada novamente.

## 9. Região candidata e invariante

Durante a execução, a região onde o valor ainda pode estar é:

```text
linhas:  linha até N - 1
colunas: 0 até coluna
```

Linhas acima de `linha` e colunas à direita de `coluna` já foram descartadas.

Invariante fundamental:

> No início de cada iteração, se o valor existe e ainda não foi encontrado, ele está no retângulo formado pelas linhas de `linha` até `N - 1` e pelas colunas de `0` até `coluna`.

Ao andar para a esquerda, só é removida uma coluna comprovadamente grande demais. Ao andar para baixo, só é removida uma linha comprovadamente pequena demais. Logo, a invariante é preservada.

## 10. Prova de corretude

### Inicialização

No início, `linha = 0` e `coluna = N - 1`. A região candidata abrange toda a matriz. Se o valor existe, está nessa região.

### Manutenção

Para a posição atual:

1. se o valor é igual, retornar `1` é correto;
2. se é maior, a ordenação vertical prova que a coluna pode ser eliminada;
3. se é menor, a ordenação horizontal prova que a linha pode ser eliminada.

Nenhum movimento descarta uma possível ocorrência.

### Término

Se o laço termina sem encontrar, não restam linhas ou colunas candidatas. Pela invariante, uma ocorrência teria de estar nessa região agora vazia. Portanto, `return 0` é correto.

## 11. Rastreamento manual

Matriz:

```text
 0   5  10
 2   7  12
 7  12  17
```

Procurando `7`:

| Passo | Posição | Atual | Decisão |
|---:|---:|---:|---|
| 1 | `(0,2)` | 10 | maior: esquerda |
| 2 | `(0,1)` | 5 | menor: baixo |
| 3 | `(1,1)` | 7 | igual: retorna `1` |

Procurando `9`:

| Passo | Posição | Atual | Decisão |
|---:|---:|---:|---|
| 1 | `(0,2)` | 10 | esquerda |
| 2 | `(0,1)` | 5 | baixo |
| 3 | `(1,1)` | 7 | baixo |
| 4 | `(2,1)` | 12 | esquerda |
| 5 | `(2,0)` | 7 | baixo |
| 6 | fora | — | retorna `0` |

O caminho forma visualmente uma escada, origem de *staircase search*. O nome *saddleback search* é usado para essa técnica em matrizes monotônicas.

## 12. Complexidade da busca

Cada iteração elimina uma linha ou uma coluna. Há no máximo `N` movimentos para baixo e `N - 1` para a esquerda:

```text
máximo de posições examinadas = 2N - 1
```

Assim:

```text
melhor caso:             O(1)
pior caso por consulta:  O(N)
espaço auxiliar:         O(1)
```

O melhor caso ocorre quando o canto inicial já contém o valor. `O(1)` de espaço auxiliar significa que a busca usa apenas uma quantidade fixa de variáveis; a matriz de entrada não é espaço auxiliar.

## 13. Complexidade total

Ler os `N²` elementos custa `O(N²)`, um custo inevitável. Processar `Q` consultas de `O(N)` custa `O(QN)`.

```text
tempo total:  O(N² + QN)
memória:      O(N²)
```

Para `N = 1000`, uma consulta examina no máximo 1.999 posições. Com 50.000 consultas, o limite aproximado é 100 milhões de posições examinadas.

## 14. Comparação com busca binária por linha

| Característica | Binária em cada linha | Saddleback |
|---|---:|---:|
| Usa ordem das linhas | sim | sim |
| Usa ordem das colunas | não | sim |
| Tempo por consulta | `O(N log N)` | `O(N)` |
| Espaço auxiliar | `O(1)` | `O(1)` |

Para `N = 1000`:

```text
binária por linha: aproximadamente 1000 * log2(1000) ≈ 10000 comparações
saddleback:         no máximo 2 * 1000 - 1 = 1999 comparações
```

A melhoria assintótica é um fator `log N`. A poda pelos extremos usada na versão antiga pode ajudar em entradas favoráveis, mas não muda o pior caso `O(N log N)`.

## 15. Por que não fazer busca binária no vetor inteiro?

Ordenação por linha e coluna não implica ordenação global na memória:

```text
1  10
2  11
```

A sequência linear é `1, 10, 2, 11`, que não está ordenada. Uma busca binária nela seria incorreta. Seria necessária uma garantia extra: o último elemento de cada linha ser menor ou igual ao primeiro da próxima.

## 16. Função `main`

```c
int main(void)
```

`void` declara explicitamente que a função não recebe argumentos. O retorno é entregue ao sistema operacional:

- `return 0`: sucesso;
- `return 1`: falha tratada.

## 17. Leitura de `N` e `Q`

```c
if (scanf("%d %d", &tamanhoDaMatriz, &quantidadeDeConsultas) != 2)
    return 1;
```

`scanf` precisa dos endereços, obtidos com `&`, porque deve modificar as variáveis. Ele retorna a quantidade de conversões bem-sucedidas; como existem dois `%d`, espera-se `2`.

Para `%d`, espaços e quebras de linha são separadores equivalentes. Verificar o retorno impede continuar com variáveis não inicializadas caso a entrada falhe.

## 18. Limite da matriz global

```c
#define TAMANHO_MAXIMO 1000
int matriz[TAMANHO_MAXIMO][TAMANHO_MAXIMO];
```

A matriz é alocada estaticamente, sem `malloc`, `calloc` ou `free`. Como o limite oficial é `N <= 1000`, a capacidade máxima cobre qualquer entrada válida. Quando `int` tem quatro bytes, ela ocupa aproximadamente 4 MB.

O tamanho efetivamente usado é `tamanhoDaMatriz x tamanhoDaMatriz`; o restante da capacidade não participa da busca. Como a matriz é global, seu armazenamento não depende do limite da pilha.

## 19. Leitura da matriz

Os dois laços percorrem as linhas e colunas efetivamente usadas:

```c
for (int linha = 0; linha < tamanhoDaMatriz; linha++)
    for (int coluna = 0; coluna < tamanhoDaMatriz; coluna++)
        scanf("%d", &matriz[linha][coluna]);
```

Cada chamada de `scanf` recebe o endereço da posição correspondente. A matriz é preenchida diretamente na estrutura global, sem cópia ou reserva adicional de memória.

## 22. Processamento e saída das consultas

O segundo `for` executa exatamente `Q` vezes. Cada consulta é independente:

1. lê `valorProcurado`;
2. chama `buscaSaddleback`;
3. imprime o retorno com `%d\n`.

Não é necessário um vetor de resultados. Imprimir imediatamente preserva a ordem das respostas e reduz o espaço extra de `O(Q)` para `O(1)`. A saída padrão normalmente possui buffer interno, então cada `printf` não implica necessariamente uma chamada separada ao sistema operacional.

## 23. Gerenciamento da memória

Como a matriz tem armazenamento estático global, não há memória alocada manualmente para liberar. O sistema operacional gerencia esse espaço durante a execução e o recupera quando o processo termina.

## 24. Casos extremos e pré-condições

- **Matriz `1 x 1`:** examina a única posição e retorna corretamente.
- **Valor menor que todos:** anda para a esquerda até `coluna == -1`.
- **Valor maior que todos:** anda para baixo até `linha == N`.
- **Negativos:** são comparados normalmente e `[-10^9, 10^9]` cabe em `int` de 32 bits.
- **Duplicatas:** qualquer ocorrência faz a função retornar `1`.
- **Consultas repetidas:** a busca é refeita; não existe cache.

Pré-condições da função:

1. `matriz` aponta para pelo menos `tamanho * tamanho` inteiros válidos;
2. `tamanho > 0`;
3. linhas e colunas estão em ordem não decrescente.

A função não verifica a ordenação porque o enunciado a garante. Se ela for violada, o algoritmo pode descartar uma região que contém o valor e produzir falso negativo.

## 25. Alternativas

### Tabela hash

```text
construção: O(N²) esperado
consulta:   O(1) esperado
espaço:     O(N²), com sobrecusto
```

Pode ser mais rápida com muitas consultas, mas exige mais memória, uma implementação mais complexa em C e oferece custo esperado, não necessariamente garantido.

### Ordenar todos os elementos

```text
preparação: O(N² log(N²))
consulta:   O(log(N²))
```

Tem preparação mais cara e não usa diretamente a propriedade bidimensional.

### Cache

Consultas repetidas poderiam ser memorizadas em uma estrutura valor-resposta. A solução atual prefere simplicidade e `O(1)` de espaço auxiliar.

A saddleback é a solução mais natural para demonstrar o uso das duas ordenações, com pior caso determinístico `O(N)` e código pequeno.

## 26. Melhorias adicionais de robustez

Um programa fora do juiz poderia:

- validar explicitamente os limites de `N` e `Q`;
- garantir que `N` não ultrapasse `TAMANHO_MAXIMO` antes de acessar a matriz;
- imprimir mensagens em `stderr`;
- separar leitura e processamento em funções.

Nos limites oficiais, `TAMANHO_MAXIMO` garante espaço para a matriz. Fora do juiz, também seria recomendável verificar os retornos de `scanf` antes de usar os valores lidos.

## 27. Perguntas prováveis de sabatina

### Por que começar no canto superior direito?

Porque à esquerda há valores menores ou iguais e abaixo há valores maiores ou iguais. Isso permite eliminar uma linha ou coluna com uma única comparação.

### Qual é a invariante?

Se o valor existe e ainda não foi encontrado, ele permanece na região formada pelas linhas `linha...N-1` e colunas `0...coluna`.

### Por que o algoritmo termina?

Cada passo retorna, aumenta a linha ou diminui a coluna; os índices nunca voltam. São examinadas no máximo `2N - 1` posições.

### Qual é a complexidade?

`O(N)` por consulta, `O(N² + QN)` no total e `O(1)` de espaço auxiliar para a busca.

### Por que não usar uma busca binária única?

Porque concatenar linhas não produz necessariamente uma sequência globalmente ordenada.

### Como a função acessa a matriz?

A matriz é global, portanto a função usa diretamente `matriz[linha][coluna]` e recebe apenas o tamanho efetivamente usado e o valor procurado.

### Por que verificar `scanf`?

Para confirmar as conversões e não usar dados inválidos ou não inicializados.

### Por que imprimir imediatamente?

As consultas são independentes; não há necessidade de gastar `O(Q)` com um vetor de respostas.

### Aceita repetidos e negativos?

Sim. Repetidos são compatíveis com ordem não decrescente e negativos usam comparações normais de `int`.

### O que ocorre se a matriz não estiver ordenada?

A justificativa das eliminações deixa de valer e pode haver falso negativo.

### O canto inferior esquerdo funciona?

Sim: se o atual for maior, sobe; se for menor, anda para a direita.

### A busca modifica a matriz ou é recursiva?

Não modifica, como mostra `const`, e é iterativa. Assim usa `O(1)` de espaço auxiliar.

## 28. Resumo de última hora

- Linhas e colunas são monotônicas.
- A busca começa no canto superior direito.
- Igual: retorna `1`.
- Atual maior: elimina a coluna e anda à esquerda.
- Atual menor: elimina a linha e anda para baixo.
- A região candidata diminui sem perder uma possível resposta.
- No máximo `2N - 1` posições são examinadas.
- Busca: `O(N)` no pior caso e `O(1)` de espaço auxiliar.
- Programa: `O(N² + QN)` no tempo e `O(N²)` na memória.
- A matriz global tem capacidade máxima definida por `TAMANHO_MAXIMO`.
- O acesso usa `matriz[linha][coluna]`.
- Não há alocação dinâmica nem necessidade de `free`.
- Não se pode aplicar busca binária à matriz linear sem ordenação global.

## 29. Resposta oral curta

> A matriz global tem capacidade máxima de `1000 x 1000` e uma posição é acessada por `matriz[linha][coluna]`. Para cada consulta, a saddleback começa no canto superior direito. Se o atual é maior que o procurado, elimina a coluna porque os valores abaixo são ainda maiores ou iguais. Se é menor, elimina a linha porque os valores à esquerda são menores ou iguais. Assim, a região candidata diminui sem perder uma possível ocorrência. Como há no máximo `N` movimentos verticais e `N - 1` horizontais, cada consulta custa `O(N)`, melhor que `O(N log N)` da busca binária em todas as linhas.
