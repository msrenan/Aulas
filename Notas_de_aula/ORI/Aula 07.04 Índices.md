# Índice
Estrutura que mapeia um conjunto de dados em uma estrutura com uma chave identificadora e um ponteiro que aponta para o endereço do conjunto de dados completo (pode ser um offset caso os dados estejam em um arquivo de texto).

# Índice Linear
- Linear (como um vetor de hashmaps) 
- Muito menor que o arquivo de dados
- Pode ser replicado em memória
### Pesquisa de índice linear
Você primeiro procura a chave na estrutura do índice e depois pula o offset que a chave do índice armazena com si para chegar ao conjunto completo de dados.

> OBS: Ordenar as chaves pode ser uma boa estratégia para aumentar a velocidade de busca, utilizando busca binária

# Índice não linear
- Não linear (como uma árvore binária)
- Operações mais rápidas que no linear (menos a busca que é a mesma (se for busca binária no linear))
- Mais gasto de memória.

# Propriedades de índices

### Agrupado x Não agrupado
- Agrupado: mantém a ordem original do arquivo de dados
- Não agrupado: não mantém a ordem original do arquivo de dados

### Denso x Esparso
- Denso: Cada registro no arquivo de dados implica em um registro no índice
- Esparso: O índice contém um número menor de entradas em comparação com o arquivos de dados.
	- Um índice esparso **deve** ser agrupado.

### Primário x Secundário
- Primário: Identificador único, constituido sobre uma chave primária
- Secundário: Identificador que pode existir em múltiplos registros, constituido sobre uma chave secundária
	- Fortemente ligado: Armazena o endereço do registro.
	- Fracamente ligado: Armazena a chave primária do registro.
		- Isso significa que o Índice secundário deve apontar para o índicie primário que então aponta para um endereço no arquivo de registros. Um índice secundário fortemente ligado aponta direto para o arquivo de registros.

#### Repetição de chaves secundários
Algumas formas de lidar envolvem lidar com um índice que aponta para um vetor fixo de chaves primárias ou implementar uma lista encadeada invertida no índice das chaves primárias, que aponta para o próximo elemento de mesma chave secundária.