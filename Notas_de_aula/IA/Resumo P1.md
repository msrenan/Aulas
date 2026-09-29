
---

# Sumário

1. **[[#Introdução à IA]]**
2. **[[#Busca desinformada]]**
3. **[[#Busca Informada]]**
4. **Algoritmos evolutivos**

---
# Introdução à IA

## Resolução de problemas por Busca

Consiste em modelar um problema usando uma representação do mundo baseada em **estados** e aplicar um algoritmo de busca sobre esses estados para encontrar uma solução para o problema.

Para estudar a resolução de problemas por busca (RPB), devemos entender dois conceitos:
- **Representação dos problemas**: Como mapear os elementos e caminhos de uma situação real para os estados.
- **Algoritmos de busca**: de propósito geral, pois independem do domínio, e nos ajudam a encontrar uma solução.

### Representação do Problema
As etapas para se representar um problema são:
1. **Formulação do Objetivo**:
	1. Definir qual problema se deseja resolver
	2. O objetivo é definido como <u>um conjunto de estados do mundo nos quais esse objetivo é satisfeito</u>.
2. **Formulação do Problema**:
	1. Processo de decidir quais ações e estados considerar, dado um objetivo.

A representação do problema envolve a definição de:
- **Estados** que representam o mundo
	- Ex: Dado um conjunto de cidades, percorrer todas elas sem passar duas vezes por nenhuma delas
	- **Estado:** quais cidades foram percorridas até o momento
- **Ações** que provocam a alteração de um estado para outro
	- **Ação:** transitar de uma cidade para outra

### Busca
É o processo de encontrar uma solução para um problema como uma **sequência de passos** entre um estado *inicial* e um estado *final* (objetivo). Inicialmente, assumimos que o ambiente (mundo) é:
- **Observável**: O estado atual é conhecido
- **Discreto**: A cada estado, apenas um número finito de ações podem ser tomadas
- **Conhecido**: O estado atingível por cada ação é conhecido
- **Determinístico**: Cada ação leva a **apenas um estado**.

### Definição de um problema
Um problema pode ser definido formalmente por 5 componentes:
- **Estado inicial:** Representa a situação a qual a busca se inicia.
- **Ações ou Operadores:** Descrição das possíveis ações, aplicáveis a cada estado.
- **Modelo de Transição:** Descrição do resultado de cada ação, especificado por uma função `RESULTADO(s, a)` que retorna o estado resultante da aplicação da ação `a` no estado `s`.
- **Teste final:** Condições que determinam se um estado é o objetivo
- **Custo do Caminho:** Função que atribui um custo ao caminho, geralmente a soma dos custos de cada passo - serve para medir a qualidade da solução.

#### Conceitos relevantes sobre solução de problemas
1. **Caminho:** Sequência de ações que levam de um estado a outro
2. **Espaço de Estados:** Conjunto de **todos** os estados, ações e modelo de transição. Conjunto de estados atingíveis do estado inicial por qualquer sequência de ações.
3. **Solução:** **Caminho** do estado inicial a um estado objetivo
4. **Solução ótima:** Solução que tem o menor **custo** de caminho entre todas as soluções

## Importante:
1. **A definição do problema *independe* do algoritmo de busca que será utilizado, e portanto, pode ser utilizada para diferentes algoritmos de busca.**


## Visão geral de Algoritmos de busca
 A ideia básica de um algoritmo de busca que será utilizado para resolver um problema formulado é expandir uma sequência de ações até que esta leve ao resultado esperado.
 - **Árvore de Busca:** Árvore gerada pelo processo de busca, com o estado inicial na **raíz**.
	 - Nós -> Estados
	 - Arcos -> Operadores
	 - Solução -> Caminho do nó inicial ao nó final.

A ideia principal do algoritmo de busca é manter e estender um conjunto de soluções parciais (sequência de ações). A busca utiliza operações de **expansão e geração:**
1. **Expansão de um nó:** aplicação de *todos* os operadores permitidos nesse nó.
2. **Geração de um conjunto de nós:** criação dos nós resultantes da *expansão* de um nó.

O processo geral dos algoritmos de busca será:
1. Raíz corresponde ao estado inicial - e é aqui que tudo começa.
2. Testa-se se o estado atual é um objetivo.
3. Expande-se o estado corrente, gerando um novo conjunto de estados.
4. Adiciona-se ramos saindo do nó **expandido** e criar novos nós, um para cada estado gerado.
5. **Escolhe-se** um nó da folha e repete-se o processo até que a solução seja encontrada ou até que não existem mais nós a serem expandidos.

O conjunto de nós **folha** da árvore de busca é chamado de **fronteira ou lista de nós *abertos*.**

Os algoritmos de busca possuem uma mesma estrutura básica e variam de acordo com a estratégia utilizada.

- **Estratégia de busca:** determina o critério utilizado para selecionar o *próximo nó a ser expandido* no algoritmo.
	- As estratégias são implementadas por meio da forma de tratamento da **lista de nós abertos.**

São elas:

![[Pasted image 20260928185644.png]]

---

# Busca desinformada
São as estratégias de busca que **não** possuem informação adicional sobre o problema, além de sua própria definição. Podendo apenas gerar sucessores a partir de um dado estado e testar se um estado é o objetivo ou não. 

> Todas as estratégias dessa categoria são determinadas pela **ordem** em que os nós são expandidos.

## Busca em Largura
Explora o espaço nível por nível: *primeiro, o nó inicial é expandido, depois seus sucessores, depois os sucessores de seus sucessores, e assim por diante.* Dessa forma, todos os nós de um determinado nível são expandidos antes de iniciar a expansão dos nós do nível seguinte.

- **Lista de nós abertos é tratada como FILA (FIFO)!**

> Encontra sempre o **caminho mais curto** para a solução.
> > Caso existam caminhos alternativos para atingir um nó da fronteira, esse caminho deve ser no mínimo tão longo quanto o que já foi encontrado antes.

> O caminho **mais curto** será o caminho **ótimo** se todos os movimentos tiverem o mesmo custo!

**Exemplo de algoritmo**:
```
Open = [Start]; // Fila de nós gerados mas NÃO expandidos (FIFO)
Closed = []; // Lista de nós já expandidos
While Open != [] {

	X = pop(Open); // Remove o primeiro estado de Open -> X (mais a esquerda)
	if X == objetivo return Sucess;
	gere todos os filhos de X;
	push(Closed, X); // Marca X como explorado
	elimina todos os filhos de X que já estejam em Open ou Closed;
	coloque os outros descendentes de X, na ordem em que foram gerados, ao fim de Open. (mais a direita)
}
```

> Para problemas em que todos os movimentos tenham o mesmo custo, o teste do nó objetivo pode ser feito **no momento em que o nó é *gerado*, o que possuí uma série de vantagens e economiza diversos ciclos de processamento**.

## Busca em Profundidade
Explora o espaço ramo por ramo: expande o nó no nível mais interno dos nós da fronteira até que o nó desse ramo não tenha mais sucessores. Então, retrocede a busca ao próximo nó mais profundo que ainda tenha sucessores não explorados.

- **Lista de nós abertos é tratada como PILHA (LIFO)**

> **Não garante** o caminho mais curto, nem a solução ótima, mesmo se as ações tiverem o mesmo custo.

**Exemplo de algoritmo:**
```
Open = [Start]; // Pilha de nós gerados mas NÃO expandidos (LIFO)
Closed = []; // Lista de nós já expandidos

While Open != [] {

	X = pop(Open); // Remove o primeiro estado de Open -> X (mais a esquerda)
	if X == objetivo return Sucess;
	gere todos os filhos de X;
	push(Closed, X); // Marca X como explorado
	elimina todos os filhos de X que já estejam em Open ou Closed;
	coloque os outros descendentes de X, na ordem em que foram gerados, no início de Open (mais a esquerda)
}
```

> Para problemas em que todos os movimentos tenham o mesmo custo, o teste do nó objetivo pode ser feito **no momento em que o nó é *gerado*, o que possuí uma série de vantagens e economiza diversos ciclos de processamento**.

### Variações da Busca em Profundidade
A busca em profundidade possuí algumas limitações e desvantagens bastante marcantes. Então são propostas algumas variações para torná-la mais eficiente.

- **Busca em Profundidade Limitada**: Define previamente um limitante de nível *lim* para a expansão dos nós, mesmo que ainda existam sucessores a serem expandidos.
	- Nós do nível *lim* são tratados como se **não tivessem sucessores**.
	- **Problema:** se o objetivo estiver em um nível mais profundo que *lim*, ele **não será encontrado.**
- **Busca em Profundidade Limitada Iterativa**: Busca em profundidade limitada que encontra o melhor limite.
	- Varia o valor de *lim*, começando com 0, e repete o processo para cada valor, desde o início, até encontrar a solução.
- **Backtracking**: Armazena apenas o caminho sendo explorado.
	- Não armazena irmãos do nó gerado nem caminhos já percorridos.
	- Os filhos de cada nó são gerados um por vez, e não todos ao mesmo tempo como na Busca em Profundidade padrão.

## Algoritmo de Busca de Custo Uniforme
Utilza a função de custo `g` definida como parte da formulação do problema:
- `g(n)` é o custo do caminho do nó inicial até o nó n.
- `g(n)` é calculada pela **soma** dos custos da aplicação de cada uma das ações do caminho

Expande o primeiro nó n que tenha o menor custo de caminho `g(n)`.
- A fronteira é armazenada como uma **lista de prioridades ordenada por `g`.**

O teste de objetivo só é aplicado quando o nó é **selecionado para expansão**, já que o primeiro nó objetivo gerado pode estar em um caminho subótimo.

Um teste deve ser adicionado para verificar se um novo caminho de um nó que já estava na fronteira é melhor do que o anterior.

Na busca de custo uniforme, não importa o tamanho da solução, e sim **seu custo.**

> Encontra **sempre** a solução ótima.

Exemplo de Código:

```
Open = [Start]; // Lista de prioridade de nós abertos.
Closed = []; // Lista de nós já explorados

While Open != [] {

	X = pop(Open); // Remove o estado com maior prioridade (no início) de Open -> X (mais a esquerda)
	if X == objetivo return Sucess;
	gere todos os filhos de X;
	push(Closed, X); // Marca X como explorado
	
	for filho : X {
		if filho not in Open and not in Closed {
			atribua valor de avaliação a este estado;
			adiciona filho a Open
		} else if filho in Open {
			if filho foi atingido com um valor de custo menor {
				dar filho este valor menor.
			}
		} else if filho in Closed {
			descarte filho
		}
		
		reordenar os estados em Open de acordo com valor de custo (prioridade)
	}
	
	if Open == [] return erro.

}

```

---

# Busca Informada

Também conhecida como Busca Heurística, é a estratégia de busca que considera informação *específica* sobre o problema, **além da definição do problema em si.**

- Essa informação considerada vem na forma de **heurísticas.**
- Estados são avaliados em função do seu conteúdo, considerando a situação **específica** que representam.
- A informação sobre o problema é usada no momento de selecionar qual o próximo nó a ser expandido.

> Ao contrário da busca desinformada, na busca heurística, evita-se a busca exaustiva.

### Heurísticas

São regras simples ou ("dicas") utilizadas para avaliar rapidamente uma situação específica. Nos métodos de busca informada, são utilizadas para escolher os caminhos em um espaço de estados que tem **mais chance de levar a uma solução**, evitando a busca exaustiva.

> Devem ser expressas na forma de função, que vai ser aplicada a cada estado.

É um conceito importante em IA, pois é uma forma simples de representar **conhecimento.**

**Situações em que heurísticas são utilizadas em IA**:
- Quando um problema não tem solução exata (exemplo: diagnóstico, visão computacional)
- Um problema tem uma solução exata mas o custo computacional é proibitivo (muito alto)

As heurísticas possuem diversas limitações que precisam ser evitadas na sua utilização:
1. Busca sujeita a falhas (a heurística **por si só**, <u>não traz garantia que a busca que está sendo feita é a melhor.</u>)
2. É uma **tentativa de adivinhar** o melhor caminho (por isso as falhas ocorrem)
3. Baseada em experiência e intuição (não exise um mecanismo ou procedimento exato para a construção da heurística)
4. **Pode levar a uma solução sub-ótima ou pode não encontrar a solução.**

## Busca pela Melhor Escolha

- Utiliza conhecimento específico do problema para selecionar o próximo nó a ser expandido.
- Esse conhecimento é expresso através de uma **Função de Avaliação**.
- Este algoritmo pode ser entendido como um *modelo que representa vários algoritmos* e, ao definir o tipo específico de função de avaliação que será utilizada, **tem-se um algoritmo específico.**

### Função de Avaliação
É uma função que tenta exprimir o quanto é desejável expandir um nó (retornando um número). Tipicamente, essa é gerada usando uma **medida estimada do custo da solução.**

No algoritmo de busca, é aplicada a cada nó no momento em que ele é gerado. Em alguns algoritmos, um nó pode ter valores de avaliação diferentes, dependendo do caminho utilizado para chegar até ele no processo de busca.

> Nó que tiver o menor valor de avaliação é considerado o melhor

### O algoritmo

Utiliza uma lista de prioridade para `Open` (nós abertos mas não expandidos), e uma lista para `Closed` (nós já expandidos), de maneira que `Open` deve sempre ser reordenada (em ordem **crescente**) a cada iteração.

A ideia é que o próximo nó a ser expandido **sempre é escolhido com base no seu valor de avaliação**, *independente do seu **nível ou do ramo em que se encontra!***

**Exemplo de código:**

```
Open = [Start];
Closed = [];
While Open != [] {

	X = próximo estado retirado de Open
	if X == objetivo return Solução;
	processe X, gerando seus filhos;
	for filho de X {
	
		if fiho not in Open and filho not in Closed {
			atribua valor de avaliação a este estado;
			adicione a Open;
		} else if filho in Open {
			if estado foi atingido com um valor de avaliação menor {
				dar a esse estado em Open este valor menor;
			}
		} else if filho in Closed {
			if estado foi atingido por um valor de avaliação menor {
				dê ao estado em Closed esse valor menor;
				mover esse estado de Closed para Open;
			}
		}
		
		coloque X em Closed;
		reordenar estados de Open;
	}
	
	if Open == [] {
		return erro;
	}
 
}

```

> Ao atribuir um valor menor a um nó que é reatingido (e já estava em `Open`), é preciso **alterar o caminho** que chega àquele nó.

## Função de Avaliação na Busca Best-First (Melhor escolha)

Na busca por melhor escolha, envolve duas medidas:
1. Função de custo (conhecida a cada passo):
	1. `g(n)`: *custo do caminho da raíz até o nó **n***
2. Função Heurística (estimada):
	1. `h(n)`: *estimativa de custo do caminho do nó **n** até o objetivo.*
	2. **Restrição:** `h(n) = 0` quando **n** é um objetivo.

### Função de Custo `g`
- Calcula um valor com base nos custos dos movimentos do caminho já percorrido durante o processo de busca, desde a raíz até o nó.
- Um nó *n* pode ter valores diferentes de `g` em situações diferentes **no mesmo processo de busca**, uma vez que `g` leva em conta o *caminho utilizado para chegar até n*.

### Função Heurística `h`
- Forma mais comum de aplicar conhecimento adicional do problema ao algoritmo de busca.
- Definida para cada problema
- `h(n)` é o custo **estimado** do caminho mais econômico do nó *n* até um nó objetivo.
- **Restrição:** se *n* for um objetivo: `h(n) = 0`.

> Observações importantes:
> 	1. A função heurística é baseada nas informações do **estado em que está sendo aplicada**, não considerando nenhum tipo de informação relacionada ao **custo** das operações.
> 	2. Na definição de uma função heurística, é necessário considerar a eficácia da função e o custo para seu cálculo.
> 	3. Uma função heurística mais complexa pode avaliar o estado com mais precisão, mas se seu cálculo for muito custoso, sua utilização pode ser inviável.


## Busca Gulosa

- Minimiza o custo estimado para atingir um objetivo
- Expande o primeiro nó considerado **mais perto do objetivo** (heurística)
- **Função de Avaliação = Função Heurística**
	- Onde: `h(n)` é a estimativa do custo do caminho do nó *n* até o objetivo

## Algoritmo A*

- Minimiza o custo estimado que passa por um determinado nó
- **Função de Avaliação = Função de Custo + Função Heurística**
	- Onde *n* representa qualquer estado
	- `g(n)` é o custo do caminho inicial até o nó *n*
	- `h(n)` é a estimativa heurística do custo do caminho do nó *n* até o objetivo.

## Busca Local