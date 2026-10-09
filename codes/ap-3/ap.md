# Problema O4 (Ordenação 1): guia de apresentação do código final

Guia para apresentar e defender o código em C com Counting Sort.
Tudo foi compilado e testado com `gcc` (C99, `-O2`). Onde algo é raciocínio e não foi testado, está indicado.

---

## 1. O problema em 30 segundos

- O vetor é grande demais para a entrada, então **o programa gera o vetor** com um gerador pseudoaleatório (xorshift de 32 bits sem sinal).
- Entrada: `N` (tamanho) e `S` (semente).
- Geração, para cada `i` de 1 até N, nesta ordem:
  ```
  c = c ^ (c << 13);
  c = c ^ (c >> 17);
  c = c ^ (c << 5);
  x[i] = c % 1048576;      // 2^20, valores entre 0 e 1048575
  ```
- Ordenar `x` em ordem não decrescente, obtendo `y1 <= y2 <= ... <= yN`.
- Saída: `(soma de i * yi, para i de 1 até N) mod (10^9 + 7)`.
- Limites: `1 <= N <= 1,5 x 10^7` e `1 <= S <= 2^32 - 1`.

| Entrada | Saída |
|---|---|
| `10 12345` | `34851791` |
| `1 1` | `270369` |

**A pista do enunciado:** "todo xi fica entre 0 e 1048575". Valores numa faixa pequena e conhecida são exatamente o que o Counting Sort explora.

---

## 2. O código

```c
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
```

### Resultado dos testes

| Teste | Esperado | Obtido |
|---|---|---|
| `10 12345` | `34851791` | `34851791` |
| `1 1` | `270369` | `270369` |
| `200000 12345` | `991662911` | `991662911` |
| `15000000 987654321` | `121170593` | `121170593` |
| `15000000 1` | `277375864` | `277375864` |
| `15000000 4294967295` | `474968418` | `474968418` |

(Os valores esperados dos testes grandes vêm de outras implementações que escrevi e comparei entre si: bubble sort, merge sort e outras versões do counting sort.)

- **Tempo no N máximo:** cerca de **0,23 s**.
- **Memória no N máximo:** cerca de **63 MB** (60 MB do `vetor` mais 4 MB da `contagem`).
- **Warnings:** com `-Wall -Wextra`, só um: o retorno do `scanf` não é verificado (detalhes na seção 11).

---

## 3. Roteiro da apresentação

### Slide 1: o problema e a pista
- O programa gera o vetor, ordena e calcula uma soma ponderada pela posição.
- N chega a 15 milhões, então o algoritmo de ordenação importa.
- **Pista:** valores entre 0 e 2^20 - 1.

### Slide 2: geração do vetor (passo 1 da `main`)
- `unsigned int` é obrigatório: o enunciado pede 32 bits **sem sinal**.
- `<<` e `>>` deslocam bits, `^` é o XOR. Os três passos embaralham os bits de `c`.
- `c % MAX` limita cada valor a 20 bits (0 a 1048575).

### Slide 3: Counting Sort (`counting_sort`)

**Frase central:** "Tenho uma tabela com uma casinha para cada valor possível. Cada número do vetor soma 1 na sua casinha. Depois leio a tabela do menor para o maior valor e reescrevo o vetor."

- **Fase A (contar):** `contagem[v[i]]++` para cada elemento. Um laço, nenhuma comparação entre elementos.
- **Fase B (reescrever):** para cada `valor` de 0 a `MAX - 1`, enquanto ainda restam ocorrências, escreve o valor em `v[k]`, avança `k` e desconta uma ocorrência.
- `k` é a **próxima posição livre** do vetor. Por isso nasce fora dos laços e só cresce.

### Slide 4: a soma (passo 3 da `main`)
- Percorre o vetor ordenado e acumula `posição * valor`.
- A posição começa em **1**, por isso `i + 1`.
- `% MOD` a cada passo mantém o valor pequeno.
- `(long long)` converte **antes** de multiplicar, para a conta ser em 64 bits.

### Slide 5: complexidade e resultado
- Tempo O(N + K) e espaço O(N + K), com K = 2^20.
- No N máximo: cerca de 0,23 s.

### Slide 6: por que não bubble sort
- Bubble sort é O(N²): cerca de 10^14 comparações no N máximo. Inviável.
- Medido em C++ na versão anterior: 50.000 elementos levam cerca de 6,6 s, 100.000 levam cerca de 26,6 s, 200.000 levam cerca de 109 s. Cada vez que N dobra, o tempo quadruplica.

### Slide 7: limitações e alternativas
- Counting sort só serve com faixa pequena de valores.
- Se a faixa fosse grande, usar Merge Sort ou `qsort`, em O(N log N). O Merge Sort que testei levou cerca de 2,4 s no N máximo.

---

## 4. O código, linha a linha

| Trecho | O que faz | Por que está assim |
|---|---|---|
| `#define MAX 1048576` | Define 2^20, o número de valores possíveis | Uma constante só: usada no gerador (`c % MAX`), na tabela e no laço da Fase B |
| `#define MAXN 15000000` | Tamanho máximo do vetor | O enunciado garante N ≤ 1,5 x 10^7 |
| `#define MOD 1000000007LL` | Módulo da resposta | O sufixo `LL` torna a constante `long long` |
| `int vetor[MAXN]; int contagem[MAX];` | Arrays **globais** | Ficam fora da pilha (um array local de 60 MB estouraria a pilha) e já começam **zerados** |
| `void counting_sort(int v[], int n)` | Ordena `v` em ordem crescente | `v` é só um ponteiro para o `vetor` global, então a função altera o original |
| `int k = 0;` | Próxima posição livre do vetor | Fica fora dos laços para não ser reiniciada |
| `contagem[v[i]]++` | Soma 1 na casinha do valor | O **valor** do elemento é usado como **índice** da tabela |
| `while (contagem[valor] > 0)` | Repete enquanto restam ocorrências | Cada passada escreve uma ocorrência |
| `v[k] = valor; k++;` | Escreve o valor e avança a posição | Reconstrói o vetor ordenado |
| `contagem[valor]--` | Desconta a ocorrência já escrita | É o que faz o `while` terminar |
| `unsigned int S, c;` | Semente e estado do gerador sem sinal | Com sinal, o `>>` e o `%` se comportam diferente |
| `scanf("%d %u", &N, &S)` | Lê N e a semente | `%d` para `int`, `%u` para `unsigned int` |
| `c % MAX` | Resto da divisão por 2^20 | Como `c` é sem sinal, o resultado é sempre ≥ 0 |
| `(long long)(i + 1) * vetor[i]` | Posição vezes valor, em 64 bits | Em `int`, o produto estouraria |
| `% MOD` | Mantém a soma abaixo de 10^9 + 7 | Vale porque `(a + b) mod m = ((a mod m) + (b mod m)) mod m` |

---

## 5. Exemplo na mão (para o quadro)

Vetor `[3, 1, 3, 0, 1, 3]`, com valores de 0 a 3 (faixa pequena, só para o exemplo).

**Fase A: contar**

| Valor | 0 | 1 | 2 | 3 |
|---|---|---|---|---|
| `contagem` | 1 | 2 | 0 | 3 |

**Fase B: reescrever com o `while`**

| `valor` | Ocorrências | O que o `while` faz | `k` ao final | Vetor |
|---|---|---|---|---|
| 0 | 1 | `v[0] = 0` | 1 | `[0, ?, ?, ?, ?, ?]` |
| 1 | 2 | `v[1] = 1`, `v[2] = 1` | 3 | `[0, 1, 1, ?, ?, ?]` |
| 2 | 0 | não entra no `while` | 3 | igual |
| 3 | 3 | `v[3] = v[4] = v[5] = 3` | 6 | `[0, 1, 1, 3, 3, 3]` |

Ao final, **a tabela `contagem` está toda zerada**, porque o `while` descontou cada ocorrência.

**Soma:** `1*0 + 2*1 + 3*1 + 4*3 + 5*3 + 6*3 = 0 + 2 + 3 + 12 + 15 + 18 = 50`.

Nenhuma comparação entre elementos foi feita.

---

## 6. Análise de complexidade

Seja **N** o tamanho do vetor e **K = MAX = 2^20** o número de valores possíveis.

### Tempo

| Trecho | Custo | Explicação |
|---|---|---|
| Geração do vetor | O(N) | N iterações, cada uma com poucas operações |
| Fase A (contar) | O(N) | Um incremento por elemento |
| Fase B (reescrever) | O(N + K) | O laço externo roda K vezes. O `while` roda, **somando todas as casinhas**, exatamente N vezes (uma por elemento) |
| Soma | O(N) | Uma multiplicação e um módulo por elemento |
| **Total** | **O(N + K)** | Linear |

- Como K = 2^20 (cerca de 10^6) é fixo, na prática é **O(N)**.
- **Mesmo custo em qualquer caso:** melhor, médio e pior. O algoritmo não olha a ordem dos elementos.
- Para ver por que a Fase B é O(N + K): são K visitas às casinhas e N escritas no total. As escritas não se multiplicam pelo laço externo, porque cada ocorrência é escrita uma única vez.

### Espaço

| Estrutura | Tamanho | Complexidade |
|---|---|---|
| `vetor` | N inteiros de 4 bytes (cerca de 60 MB no máximo) | O(N) |
| `contagem` | K inteiros de 4 bytes (4 MB) | O(K) |
| **Total** | cerca de **63 MB medidos** | **O(N + K)** |

Os arrays globais têm tamanho **fixo** (`MAXN` e `MAX`), então o programa reserva esse espaço independente do N da entrada. O sistema só materializa as páginas de memória que são de fato tocadas, por isso N pequeno usa pouca memória real.

### Propriedades do Counting Sort

| Propriedade | Resposta |
|---|---|
| Baseado em comparações? | **Não**. Por isso não tem o limite inferior de O(N log N) |
| In-place? | Não: usa a tabela extra `contagem` de tamanho K |
| Estável? | Esta versão (contar e reescrever) não preserva a identidade dos elementos, mas para inteiros isso é irrelevante. A versão com soma acumulada é estável (seção 9, variante C) |
| Depende da faixa dos valores? | **Sim**. É o seu ponto fraco |

---

## 7. Perguntas sobre eficiência

**1. Qual a complexidade do seu código?**
Tempo O(N + K) e espaço O(N + K), com K = 2^20. Como K é constante, é linear em N.

**2. Por que isso "vence" o limite de O(N log N) dos algoritmos de ordenação?**
O limite de N log N vale para algoritmos que ordenam **comparando** elementos. O counting sort não compara: usa o valor como índice da tabela. Ele paga isso com a restrição de a faixa de valores ser pequena.

**3. Qual trecho do código é o mais caro?**
Todos são lineares. Medi cada etapa no N máximo:

| Etapa | Tempo |
|---|---|
| Gerar o vetor | cerca de 0,055 s |
| Ordenar (`counting_sort`) | cerca de 0,10 s |
| Somar | cerca de 0,06 s |
| **Total** | **cerca de 0,22 s** |

A ordenação é a maior fatia (cerca de metade), mas gerar e somar custam juntas quase o mesmo que ela. Nenhuma etapa é um gargalo grave.

**4. E se K fosse muito maior que N (ex.: N = 1000 e K = 2^20)?**
A Fase B continuaria visitando as 2^20 casinhas para ordenar só 1000 elementos. O custo O(N + K) seria dominado por K, e um algoritmo O(N log N) (cerca de 10^4 operações) ganharia. O counting sort só vale a pena quando K não é muito maior que N.

**5. E se K fosse 10^9?**
A tabela teria cerca de 4 GB e o laço da Fase B faria 10^9 visitas. Inviável. Usaria Merge Sort ou `qsort`.

**6. A tabela é de `int`. Isso pode estourar?**
Cada casinha guarda quantas vezes um valor apareceu, no máximo N. Com N ≤ 1,5 x 10^7, cabe folgadamente em `int` (limite cerca de 2,1 x 10^9). Se N passasse de 2^31 - 1, seria preciso `long long` ou `unsigned`.

**7. Dá para gastar menos memória?**
Dá. Se contarmos **enquanto geramos**, não precisamos guardar o vetor de 60 MB. A variante A (seção 9) usa cerca de 11 MB medidos e roda em cerca de 0,10 s.

**8. Dá para ser ainda mais rápido?**
Dá. A variante B (seção 9) calcula a soma de cada grupo de valores iguais com uma **fórmula fechada**, sem escrever nem percorrer elemento por elemento. Depois da contagem, o custo cai de O(N) para O(K). Mediu cerca de 0,05 s.

**9. O vetor de 60 MB é um problema?**
Para este problema, não: 63 MB costuma caber nos limites típicos de juízes online, mas **o limite de memória não está no enunciado que recebi**. Se o limite fosse apertado (ex.: 16 MB), a variante A resolveria.

**10. A tabela `contagem` cabe no cache do processador?**
São 4 MB, que cabem no cache L2/L3 de boa parte das CPUs atuais, o que ajuda nos acessos aleatórios da Fase A. Isso depende da máquina e **não foi medido**.

**11. Por que não usar `qsort` da biblioteca padrão?**
`qsort` é O(N log N) e chama uma função de comparação a cada par. Para N = 1,5 x 10^7 seriam da ordem de 3,5 x 10^8 comparações. Seria correto, mas mais lento que o counting sort (não medi o `qsort` em C). Não aproveita a pista do enunciado.

**12. O counting sort é estável? Isso importa aqui?**
Esta versão não é estável no sentido formal, mas não importa: só os valores contam, não a identidade dos elementos. A versão estável (variante C) gasta o dobro da memória e cerca de 2 vezes o tempo.

**13. Por que arrays globais e não locais?**
Um array local fica na pilha (stack), que costuma ter cerca de 8 MB. Um `int vetor[15000000]` local **derruba o programa** (testei: falha de segmentação até com N = 1). Globais ficam em outra região da memória e comportam isso.

**14. O que acontece se N for maior que `MAXN`?**
O programa escreve além do fim de `vetor`. Aqui isso não ocorre, porque o enunciado garante N ≤ 1,5 x 10^7. Testei com `MAXN` reduzido para 10^6: exemplos pequenos funcionam, e N = 1,5 x 10^7 dá falha de segmentação.

**15. Qual a vantagem de `while` com decremento sobre um `for` interno?**
É questão de estilo. O `while` com `contagem[valor]--` é curto e fácil de explicar, mas **zera a tabela** no processo. Um `for (int j = 0; j < contagem[valor]; j++)` preserva a tabela, útil se ela fosse reaproveitada depois.

---

## 8. Perguntas de mutação: "o que mudou?"

Cada linha é uma alteração que o professor pode fazer. Os resultados abaixo são do que **realmente aconteceu** nos meus testes.

### 8.1 Tipos e aritmética

| Mudança | O que acontece |
|---|---|
| `unsigned int S, c` vira `int c` (com `S` ainda `unsigned`) | **Falha de segmentação** já no exemplo 1. Com sinal, o `>>` propaga o bit de sinal e `c % MAX` pode dar negativo, gerando índice inválido em `contagem` |
| `unsigned int S, c` vira `int S, c` (com `%u` no `scanf`) | Também **falha de segmentação** |
| `scanf("%d %u")` vira `scanf("%d %d")` | Tecnicamente comportamento indefinido: a semente máxima (2^32 - 1) não cabe em `int`. **No meu teste funcionou por acaso**, mas não é garantido. Correto é `%u` |
| Tirar o `(long long)` da multiplicação | O produto `(i + 1) * vetor[i]` fica em `int` e **estoura com sinal**. Com N = 20000 a resposta saiu **negativa** (`-617330928`). Resposta negativa é sinal claro de estouro |
| `long long resposta` vira `int resposta` | **Continua certo**, mesmo no N máximo. O valor guardado é sempre menor que `MOD` (cerca de 10^9), que cabe em `int`. A conta intermediária é em `long long` por causa do cast. O cast é o que importa, não o tipo da variável |
| Tirar o `% MOD` de dentro do laço | A soma chega a cerca de 10^20, acima do limite do `long long` (cerca de 9,2 x 10^18). Só aparece com N grande (raciocínio, não testado com a saída) |
| `% MAX` vira `& (MAX - 1)` | **Mesmo resultado**, porque `MAX` é potência de 2 e `c` é sem sinal. Vale como explicação de otimização |
| `>> 17` vira `>> 16` no gerador | Resposta errada já no exemplo 1 (`34845519`). Mostra que a ordem e os números do gerador são parte do enunciado |

### 8.2 Counting sort

| Mudança | O que acontece |
|---|---|
| Tirar o `contagem[valor]--` | **Laço infinito** no `while`, que escreve cada vez mais longe de `v` até dar **falha de segmentação** |
| Trocar `> 0` por `>= 0` no `while` | Comportamento imprevisível: a contagem passa de 0 para negativo e o laço continua escrevendo. Nos testes deu respostas erradas (`330` no exemplo 1) e falha de segmentação no N máximo |
| Tirar o `k++` | Todas as ocorrências são escritas em `v[0]`. Resposta errada (`29238611` no exemplo 1) |
| `int k = 0` movido para dentro do laço de `valor` | `k` é reiniciado a cada valor e as escritas se sobrepõem. Resposta errada |
| `valor < MAX` vira `valor <= MAX` | Acessa `contagem[MAX]`, **fora da tabela**. Deu resposta errada nos exemplos pequenos (`34815023` e `0`) e certa no N máximo, por acaso. Erro clássico de "off-by-one" |
| Percorrer de trás para frente (`valor = MAX - 1` até `0`) | Ordena em ordem **não crescente**. No exemplo 1 dá `19833477` |
| Tirar a Fase A (não contar) | Tabela zerada, o `while` nunca roda e o vetor fica **desordenado**. Resposta errada |
| Tirar o `counting_sort(vetor, N)` da `main` | O vetor fica desordenado. Resposta errada. Mostra que ordenar é essencial |

### 8.3 Índice e soma

| Mudança | O que acontece |
|---|---|
| Índice começa em 0 (`(long long)(i)` em vez de `(i + 1)`) | Primeiro termo vira 0. O exemplo 2 (`1 1`) passa a dar `0` em vez de `270369`, então ele **pega o erro** |
| Somar só `vetor[i]` sem multiplicar pela posição | A soma passa a **independer da ordenação**. A ordenação deixaria de importar |
| Multiplicar pela posição no vetor **antes** de ordenar | Resposta errada |

### 8.4 Declarações e memória

| Mudança | O que acontece |
|---|---|
| `vetor` declarado **dentro** da `main` | **Falha de segmentação mesmo com N = 1**: 60 MB não cabem na pilha |
| `MAXN` menor que o N da entrada | Funciona enquanto N ≤ `MAXN`. Acima disso, falha de segmentação |
| `#define MAX 1<<20` (sem parênteses) | `c % 1<<20` vira `(c % 1) << 20`, que é sempre 0. **Todos os valores viram 0** e a resposta sai `0` |
| `#define MAX 1048576;` (com ponto e vírgula) | O `;` entra na macro e quebra a compilação (raciocínio, testado em outra versão) |
| `#define MAX 1000` | Os valores passam a ir de 0 a 999. Resposta diferente (`32083` no exemplo 1), mas o código continua **correto** para o novo enunciado |
| `#define MAX 1048575` | Muda a faixa dos valores. Resposta diferente (`34971426` no exemplo 1) |
| `int vetor[MAXN]` vira `unsigned int vetor[MAXN]` | Sem efeito: os valores são menores que 2^20 (raciocínio) |

### 8.5 Mudanças de enunciado

| Mudança | Efeito |
|---|---|
| N = 1 | A Fase B escreve um único valor. A soma é `1 * x1` (exemplo 2) |
| Todos os valores iguais | Uma única casinha da tabela fica preenchida. Funciona normalmente |
| Faixa maior (ex.: `% 2^30`) | Tabela de cerca de 4 GB. **Counting sort inviável**: trocar por Merge Sort ou `qsort` |
| Faixa menor (ex.: `% 1000`) | Counting sort ainda mais rápido e leve. Basta mudar `MAX` |
| Valores **negativos** | Índice negativo na tabela. Solução: somar um deslocamento (offset) igual ao módulo do menor valor, e subtrair ao reescrever |
| Pedir ordem decrescente | Inverter o laço da Fase B |
| Pedir a soma só das posições pares | Filtrar com `if ((i + 1) % 2 == 0)` na soma. O resto não muda |
| Semente `S = 0` | O gerador xorshift com semente 0 fica **preso em 0** (`0 ^ 0 = 0`). Por isso o enunciado garante `S >= 1` |
| Ordenar por outra chave (ex.: ordenar pares `(chave, valor)`) | Aí a **estabilidade** importa, e é preciso a versão com soma acumulada (variante C) |

---

## 9. Variantes prontas (se o professor mudar a estrutura)

Todas conferem com os 7 testes da seção 2.

### Comparação

| Versão | Tempo (N = 1,5 x 10^7) | Memória medida | Quando usar |
|---|---|---|---|
| **Seu código** | cerca de 0,23 s | cerca de 63 MB | Apresentação: mais fácil de explicar |
| **A:** conta enquanto gera, soma direto | cerca de 0,10 s | cerca de 11 MB | Se pedirem menos memória |
| **B:** fórmula fechada da soma | cerca de 0,05 s | cerca de 11 MB | Se pedirem o mais rápido possível |
| **C:** counting sort estável | cerca de 0,46 s | cerca de 120 MB | Se a estabilidade importar |

### Variante A: sem guardar o vetor

```c
#include <stdio.h>

#define MAX 1048576
#define MOD 1000000007LL

int contagem[MAX];

int main() {
    int N;
    unsigned int S, c;
    long long resposta = 0;

    scanf("%d %u", &N, &S);

    /* gera e conta ao mesmo tempo (nao guarda o vetor) */
    c = S;
    for (int i = 0; i < N; i++) {
        c = c ^ (c << 13);
        c = c ^ (c >> 17);
        c = c ^ (c << 5);
        contagem[c % MAX]++;
    }

    /* percorre os valores em ordem e soma direto */
    long long posicao = 0;
    for (int valor = 0; valor < MAX; valor++) {
        for (int k = 0; k < contagem[valor]; k++) {
            posicao++;
            resposta = (resposta + posicao * valor) % MOD;
        }
    }

    printf("%lld\n", resposta);
    return 0;
}
```

**Como explicar:** o vetor ordenado só existe para calcular a soma. Como a tabela já diz quantas vezes cada valor aparece, dá para somar `posição * valor` direto, sem nunca montar o vetor.

### Variante B: fórmula fechada

```c
#include <stdio.h>

#define MAX 1048576
#define MOD 1000000007LL

int contagem[MAX];

int main() {
    int N;
    unsigned int S, c;
    long long resposta = 0;

    scanf("%d %u", &N, &S);

    c = S;
    for (int i = 0; i < N; i++) {
        c = c ^ (c << 13);
        c = c ^ (c >> 17);
        c = c ^ (c << 5);
        contagem[c % MAX]++;
    }

    /* um valor que aparece cnt vezes ocupa as posicoes pos+1 ... pos+cnt.
       soma dessas posicoes = cnt*pos + cnt*(cnt+1)/2 */
    long long pos = 0;
    for (int valor = 0; valor < MAX; valor++) {
        long long cnt = contagem[valor];
        if (cnt == 0) continue;
        long long soma_posicoes = (cnt * pos + cnt * (cnt + 1) / 2) % MOD;
        resposta = (resposta + soma_posicoes * valor) % MOD;
        pos += cnt;
    }

    printf("%lld\n", resposta);
    return 0;
}
```

**Como explicar:** se um valor aparece `cnt` vezes, ele ocupa `cnt` posições seguidas, de `pos + 1` até `pos + cnt`. A soma dessas posições é `cnt*pos + cnt*(cnt+1)/2` (soma de uma progressão aritmética). Multiplica-se pelo valor uma única vez.

**Cuidado com estouro (e por que está seguro):** os maiores termos são `cnt*pos` (até cerca de 2,25 x 10^14), `cnt*(cnt+1)/2` (até cerca de 1,1 x 10^14) e `soma_posicoes * valor` (até cerca de 10^15). Todos ficam abaixo do limite do `long long` (cerca de 9,2 x 10^18). O `% MOD` é aplicado **antes** de multiplicar pelo valor. Validei a fórmula contra a soma direta em 200 casos aleatórios.

### Variante C: counting sort estável (versão de livro)

```c
#include <stdio.h>

#define MAX 1048576
#define MAXN 15000000
#define MOD 1000000007LL

int vetor[MAXN];
int saida[MAXN];
int contagem[MAX];

/* counting sort ESTAVEL (versao de livro, com soma acumulada) */
void counting_sort_estavel(int v[], int out[], int n) {
    for (int i = 0; i < n; i++) {
        contagem[v[i]]++;
    }
    /* contagem[x] passa a ser: quantos elementos sao <= x */
    for (int valor = 1; valor < MAX; valor++) {
        contagem[valor] += contagem[valor - 1];
    }
    /* de tras para frente, para manter a ordem original dos iguais */
    for (int i = n - 1; i >= 0; i--) {
        contagem[v[i]]--;
        out[contagem[v[i]]] = v[i];
    }
}

int main() {
    int N;
    unsigned int S, c;
    long long resposta = 0;

    scanf("%d %u", &N, &S);

    c = S;
    for (int i = 0; i < N; i++) {
        c = c ^ (c << 13);
        c = c ^ (c >> 17);
        c = c ^ (c << 5);
        vetor[i] = c % MAX;
    }

    counting_sort_estavel(vetor, saida, N);

    for (int i = 0; i < N; i++) {
        resposta = (resposta + (long long)(i + 1) * saida[i]) % MOD;
    }
    printf("%lld\n", resposta);
    return 0;
}
```

**Como explicar:** depois da soma acumulada, `contagem[x]` diz **onde termina** o bloco do valor `x` no vetor ordenado. Percorrendo o vetor original de trás para frente e colocando cada elemento na última posição livre do seu bloco, os elementos iguais mantêm a ordem original (estabilidade). O custo é um vetor `saida` extra (mais 60 MB).

---

## 10. Comparação com os outros algoritmos

| | Bubble | Merge | Counting (seu código) |
|---|---|---|---|
| Tempo | O(N²) | O(N log N) | O(N + K) |
| Espaço extra | O(1) | O(N) | O(K) |
| Passa no N máximo? | Não | Sim (cerca de 2,4 s) | Sim (cerca de 0,23 s) |
| Depende da faixa de valores? | Não | Não | **Sim** |
| Facilidade de explicar | Muito fácil | Média (recursão) | Fácil |

(Os tempos do bubble sort e do merge sort foram medidos nas versões em C++ anteriores. A ordem de grandeza vale para C.)

**Frase para a apresentação:** "Bubble sort e merge sort ordenam comparando elementos, e comparar tem custo mínimo de N log N. O counting sort não compara: ele aproveita que os valores são pequenos e conhecidos."

---

## 11. Pontos de atenção no seu código (melhorias opcionais)

Nada disso afeta o resultado, mas o professor pode apontar:

| Ponto | Observação |
|---|---|
| `scanf` sem verificar o retorno | O compilador avisa com `-Wall -Wextra`. Se a entrada for inválida, `N` e `S` ficam sem valor definido. Versão defensiva: `if (scanf("%d %u", &N, &S) != 2) return 1;` |
| `int main()` | Em C, `()` significa "parâmetros não especificados". O mais correto é `int main(void)` |
| `unsigned int` para 32 bits | O padrão C só garante que `unsigned int` tem **pelo menos 16 bits**. Em PCs e servidores atuais tem 32, mas `uint32_t` (de `<stdint.h>`) **garante** exatamente 32 bits. Para o enunciado, `uint32_t` é a escolha mais rigorosa |
| `while` que zera a tabela | Funciona e é simples. Só vale lembrar que `contagem` termina toda zerada |
| `MAX` como nome de constante | Em alguns ambientes, bibliotecas definem `MAX(a, b)` como macro. Aqui não há conflito, mas `LIMITE` ou `VALORES` evitaria dúvidas |
| Arrays com tamanho fixo | Corretos para o enunciado. Se N passasse de `MAXN`, seria erro |

---

## 12. Checklist para a apresentação

- [ ] Saber explicar as 3 etapas da `main`: gerar, ordenar, somar.
- [ ] Saber explicar as duas fases do `counting_sort`: contar e reescrever.
- [ ] Fazer o exemplo `[3, 1, 3, 0, 1, 3]` no quadro (tabela `[1, 2, 0, 3]`, vetor `[0, 1, 1, 3, 3, 3]`, soma 50).
- [ ] Dizer a complexidade de cabeça: **tempo O(N + K), espaço O(N + K)**, com K = 2^20.
- [ ] Saber **por que funciona aqui** (faixa pequena) e **quando não funciona** (faixa enorme, ou K muito maior que N).
- [ ] Saber por que o `while` termina (o decremento) e por que `k` fica fora dos laços.
- [ ] Saber por que os arrays são globais (pilha de cerca de 8 MB).
- [ ] Decorar os erros clássicos: `int` no gerador (falha de segmentação), falta do `(long long)` (resposta negativa), índice em 0, `<=` no laço da tabela.
- [ ] Saber citar as variantes A (menos memória), B (mais rápida) e C (estável).
- [ ] Ter os exemplos à mão: `10 12345` dá `34851791` e `1 1` dá `270369`.

### Método para responder "o que mudou?"

1. Rodar mentalmente o exemplo `1 1` (N = 1). Se mudar, o erro está na geração, no índice ou no módulo.
2. Rodar o exemplo `10 12345`. Se mudar, o erro está na contagem, na reescrita ou no tipo.
3. Se os dois passam, mas N grande falha ou dá número **negativo**, suspeitar de **estouro** (falta de `(long long)` ou de `% MOD`).
4. Se der **falha de segmentação**, suspeitar de índice fora de um array: `int` no gerador, `valor <= MAX`, `contagem[valor]--` removido (laço infinito), vetor local, ou N maior que `MAXN`.
5. Se o resultado está certo mas o tempo ou a memória mudou, olhar se o vetor está sendo guardado, e o valor de `MAX`.
6. Se o resultado mudou de forma "limpa" (sem travar), olhar a direção do laço da Fase B (ordem crescente ou decrescente) e o valor de `MAX`.