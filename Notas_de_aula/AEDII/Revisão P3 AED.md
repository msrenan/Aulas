
# Tópicos
- **GRAFOS**
	- Tipos
	- Implementação
	- Construção Aleatória
- **BUSCA EM PROFUNDIDADE EM GRAFOS**
	- Implementação
	- Ordenação topológica
	- DAGs aleatórios
	- Componentes fortemente conexos
	- Algoritmo de Kosaraju
	- Detecção de pontes
- **BUSCA EM LARGURA EM GRAFOS**
	- Implementação
	- Caminhos mínimos em grafos não ponderados
	- Caminhos mínimos ponderados em DAGs e grafos sem custos negativos
	- Algoritmo de Djikstra

---

# Grafos

Um grafo, pode ser definido, matematicamente como: `G = (V, E)` em que G é o grafo em questão, V é o conjunto de todos os vértices presentes no grafo e E é o conjunto de todas as **arestas** que ligam dois vértices.

Existem 2 tipos de grafos:
- **Direcionados**: Cada aresta possui um sentido de movimentação.
- **Não Direcionados**: Arestas não possuem sentido, sendo *bidirecionais*.
- **Ponderados**: Cada aresta possui um **peso** que interfere na movimentação, sendo assim, o que interfere na movimentação são as arestas e seus pesos.
- **Não ponderados**: Arestas não possuem peso e, portanto, a única coisa que interfere na movimentação são as próprias arestas.
- **Cíclicos**: Contém *pelo menos um* caminho que começa e termina no mesmo vértice (um loop).
- **Acíclicos**: Não possuem caminhos fechados (loops).
- **Conexo**: Em um grafo não-direcionado, é um grafo em que quaisquer 2 vértices podem se comunicar (independente do caminho utilizado).
- **Desconexo**: Quando existe uma "ilha" com um componente do grafo sem **qualquer** conexão com o resto do grafo.

> **OBS: O tipo de grafo mais famoso é o *DAG: Directed Acyclic Graph (Grafo Direcionado Acíclico)*!**

## Estruturas de Implementação

A escolha de como representar um grafo na memória impacta diretamente no desempenho do seu algoritmo. As 3 principais formas são:
- **Matriz de adjacência**:
	- Memória: *O(V<sup>2</sup>)*
	- Verificar se existe caminho entre 2 vértices: *O(1)*
	- Listar vizinhos de um vértice: *O(V)*
	- **Ideal para:** Grafos densos (*E ~ V<sup>2</sup>)
	- 
- **Lista de adjacências**:
	- Memória: *O(V + E)*
	- Verificar se existe caminho entre 2 vértices: *O(grau(u))*
	- Listar vizinhos de um vértice: *O(grau(u))*
	- **Ideal para:** Grafos esparsos (*E << V<sup>2</sup>)
- **Lista de arestas**:
	- Memória: *E*
	- Verificar se existe caminho entre 2 vértices: *O(E)*
	- Listar vizinhos de um vértice: *O(E)
	- **Ideal para:** Algoritmos de Kruskal e Prim.

## Construção Aleatória de Grafos

Gerar grafos de maneira é necessário para testar a eficiência de diferentes algoritmos. Para tanto, existem dois modelos clássicos:
- **G(n, p)**: Cria-se n vértices e para cada par de vértices possível, joga-se uma moeda inclinada com probabilidade p. Se der sucesso, adiciona-se a aresta entre o par de vértices. O número total de arestas é uma variável aleatória.
- **G(n, M)**: Cria-se n vértices e escolhe-se exatamente M arestas aleatórias, uniformemente, entre todas as combinações possíveis.

# Busca em Profundidade em Grafos

A Busca em Profundidade (DFS) é a busca que prioriza explorar um caminho até o final, antes de prosseguir para o próximo. Funciona a base de *backtracking* e tem como estrutura de dados principal, a **pilha**.

> Complexidade de tempo: O(V + E) -> Cada vértice entra na função exatamente uma vez devido à checagem do vetor de visitados. Por conta do laço `while` varrer todos os vizinhos do nó atual, varreremos exatamente por todas as E arestas do nosso grafo - 2E se for um grafo não direcionado.

> Complexidade de espaço: O(V) -> O vetor de visitados consome O(V) de memória alocada dinamicamente. Além disso, no pior cenário (um grafo no formato de linha reta), a pilha de recursão atingirá V camadas.

> Observações: A falta de desalocação ou stack overflow podem ocorrer em grafos extremamente massivos (V > 10<sup>5</sup>).

### Funcionamento

1. Marque o nó atual como visitado
2. Olhe para todos os vizinhos do nó atual
3. Para cada vizinho que não foi vizitado, chame a DFS recursivamente para ele
4. Quando todos os vizinhos de um nó forem explorados, a função encerra e o sistema retorna o nó anterior da pilha.

> Vale ressaltar que a DFS sozinha não acha caminho mínimo! Ela serve para verificar conectividade, encontrar ciclos e como base para outros algoritmos.
## Ordenação Topológica

É uma aplicação exclusiva de **DAGs (Directed Acyclic Graphs)**, sendo uma ordenação linear dos vértices, de modo que para cada aresta direcionada `u -> v` o vértice *u* apareça antes de *v* na ordenação.

**Algoritmo com DFS**: Executamos a DFS, quando um nó termina de explorar todos seus vizinhos - onde começaria o *backtracking* - empilha-se ele nem uma lista, ao final do algoritmo, a inversão dessa lista gera a **ordenação topológica** do grafo em questão.

## DAGs aleatórios

Para criar um **DAG** aleatório, não podemos fazer como descrito anteriormente, dependendo apenas da sorte para criar as arestas, pois isso pode levar à criação de **ciclos**. A estratégia, nesse caso, é:
1. Definir uma sequência linear de nós (uma ordenação topológica forçada)
2. Adicionar arestas apenas direcionadas **da esquerda para a direita**, ou seja de um nó de menor índice para um de maior índice - ainda com probabilidade p. Isso garante matematicamente a inexistência de ciclos, e que cada grafo, independente de ter os mesmos vértices, será diferente.

## Componentes Fortemente Conexos (SCC) e Kosaraju

Um **Componente Fortemente Conexo**, em um grafo direcionado, é um subgrafo em que para qualquer par de vértices *u* e *v*, existe caminho nos sentidos `u -> ... -> v` e  `v -> ... -> u`.

O algoritmo de **Kosaraju** é utilizado para encontrar *todos os componentes fortemente conexos de um grafo em **tempo linear*** (O(V + E)), usando duas passagens de DFS:
1. Execute a DFS no grafo original para calcular os tempos de término de cada vértice, inserindo os vértices terminados em um apilha - análogo à ordenação topológica.
2. Crie o **Grafo Transposto** - que consiste no mesmo grafo, mas com o sentido das arestas invertido.
3. Desempilhe os vértices da pilha criada no *passo 1*, e para cada vértice desempilhado que **ainda não foi visitado**, execute uma nova DFS no grafo transposto. Cada árvote gerada a partir desse ponto é um Componente Fortemente Conexo completo.

> **Por que inverter as setas (usando o grafo transposto)?** Porque se existe um caminho de ida e volta entre A e B, inverter as setas não muda isso, mas impede que a segunda DFS "vaze" para fora do componente.

## Detecção de Pontes

Uma **ponte** é uma aresta cuja remoção aumenta o número de componentes conexos do grafo (ou seja, desconecta uma parte do grafo).

Para detectá-las, em O(V + E), usamos a DFS mantendo 2 listas de controle:
- ***tin\[u]***: Tempo de descoberta do vértice u.
- ***low\[u]***: Menor tempo de descoberta alcancável a partir de u, usando **no máximo** uma aresta que não seja da árvore de busca (uma back-edge).

Durante a DFS, para cada aresta visitada de *u* para um vizinho *v*:
- Se v é o pai de u, ignoramos.
- Se v já foi visitado, atualizamos `low[u] = min(low[u], tin[v])`.
- Se v não foi visitado, chamamos a DFS para v e depois atualizamos: `low[u] = min(low[u], low[v])`.
- **Condição da Ponte**: Se após a linha acima, descobrirmos que **`low[v] > tin[u]`** então significa que v e seus descendentes não possuem caminhos alternativos para subir na árvore além da aresta (v, u). Logo, ***(v, u)*** é uma ponte.

> Se `low[v] > tin[u]`, isso significa que o ponto mais alto que *v* consegue alcançar na árvore ainda está abaixo de *u*. Logo, se você cortar a aresta entre eles, *v* e seus descendentes ficam isolados.

# Busca em Largura e Caminhos Mínimos em Grafos não Ponderados

A Busca em Largura (BFS) explora o grafo camada por camada, começando a partir da origem, ela explora todos os vizinhos da origem (camada 1), para então explorar todos os vizinhos de todos os nós da camada 1 (camada 2), e assim sucessivamente até chegar em uma camada n em que nenhum nó pertencente a essa camada possui vizinhos. Sua complexidade de tempo também é linear O(V + E), e possui como sua principal estrutura de dados a **Fila**.

A propriedade fundamental da BFS é que, por explorar o grafo em camadas radiais, a primeira vez q ela alcança um nó, ela alcança **sempre** pelo menor caminho possível.
### Funcionamento

1. Inicialize as distâncias de todos os nós como infinito e a do nó inicial como 0, marque o nó inicial como visitado.
2. Insira o nó inicial na Fila
3. Enquanto a fila não estiver vazia
	1. Remova o nó da frente (atual)
	2. Para cada vizinho do atual que **não foi visitado**:
		1. Marque-o como visitado
		2. Defina dist\[vizinho] = dist\[atual] + 1
		3. Defina pai\[vizinho] = atual
		4. Insira o vizinho no final da Fila

> A BFS **garante** o menor caminho *se o grafo não tiver pesos* (ou se todas as arestas tiverem peso igual a 1) porque ela esgota todas as possibilidades de comprimento *k*, antes de avançar para *k+1*.

## Caminhos mínimos Ponderados em DAGs

Se o grafo possui pesos, a BFS tradicional vai falhar. Porém, se for um DAG, existe uma maneira de funcionar mesmo com pesos negativos: **Ordenação Topológica + Programação Dinâmica**.
1. Encontre a ordenação topológica do DAG
2. Inicialize as distâncias como infinito, exceto a origem (que vale 0)
3. Processe cada vértice **estritamente** na ordem topológica. Para o vértice atual, atualize a distância de todos os seus vizinhos (*relaxamento*).
`se dist[v] > dist[u] + peso(u, v) => dist[v] = dist[u] + peso(u, v)`

Como processamos em ordem de dependência, calculamos o caminho mínimo global em O(V + E).

> **Por que funciona rápido e aceita pesos negativos?** Porque como o grafo não tem ciclo (é um *DAG*) e você processa na **ordem de dependência**, quando você chega em um nó, é matematicamente impossível que apareça outro caminho posterior para alterar a distância dele. Você só passar por cada nó e aresta, **uma vez.**

## Djikstra

Para grafos gerais (com ou sem ciclos) ponderados, **desde que não existam pesos negativos**, é o melhor algoritmo para encontrar caminhos mínimos. Adota uma estratégia gulosa usando uma **fila de prioridade (min heap).** Complexidade de tempo com Matriz/Vetor (grafos densos): **O(V<sup>2</sup>)**. Complexidade de tempo com min-heap (grafos esparsos): **O((V + E)log V)**.

Funcionamento:
1. Definimos a distância da origem como 0 e de todos os outros nós como infinito, inserimos a origem no min-heap.
2. Enquanto o min-heap não estiver vazio:
	1. Extraímos o nó u com a menor distância calculada
	2. Se essa distância extraída for maior do que a que já temos registrada para u, ignore.
	3. Para cada vizinho v de u, tente relaxar a aresta (`dist[u] + peso(u, v) < dist[v]`), atualize a distância de v (dist\[v]) e isira (dist\[v], v) no min-heap

> **Por que o Djikstra falha com pesos negativos?** Por que ele assume que uma vez que um nó foi extraído da min-heap, o menor caminho possível para ele já foi solidificado e nunca mais será reduzido. Se houver uma aresta negativa mais à frente no grafo, ela poderia reduzir mais o custo do caminho, o que quebraria a lógica gulosa do algoritmo

