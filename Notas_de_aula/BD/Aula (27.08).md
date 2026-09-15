
# Modelo Entidade-Relacionamento

## Entidade

Elemento tangível ou não que é relevante ao escopo de análise do projeto. Possuí atributos - propriedades específicas que descrevem/compõem a entidade.

Os **atributos** podem ser:
- **Monovalorados**
	- **Simples:** É um atributo que já está em sua versão final.
	- **Compostos**: Um atributo que é repartido em dois sub-atributos, que juntos compõem a versão final.
- **Multivalorados**
	- **Simples**: Conjunto de atributos simples.
	- **Compostos**: Conjunto de atributos compostos.

![[Pasted image 20260827110806.png]]

- **Derivados**: São gerados a partir de outros dados, que podem ser armazenados ou não.

O **tipo de entidade** descreve a estrutura de um conjunto de entidades que compartilham a mesma estrutura.

A extensão de um tipo de entidade é uma **instância** daquele tipo de entidade com dados preenchidos.

## Relacionamento

Um relacionamento descreve uma associação entre duas entidades. O grau de um relacionamento se refere ao número de **tipos de entidade** que participam deste relacionamento. Um papel descreve a ação que cada tipo de entidade exerce em cada instância de relacionamento.

![[Pasted image 20260827113936.png]]

Relacionamentos que não são de grau 1, são de grau N.

## Entidade Fraca x Forte
- **Fraca**: Não possuem atributos chave capazes de diferenciar instâncias.
- **Fortes**: Possuem atributos chave capazes de diferenciar instâncias.

![[Pasted image 20260827114518.png]]

Em entidades Fracas, alguns atributos podem funcionar como identificadores parciais.

# Diagrama de ER

![[Pasted image 20260827114747.png]]

# Dúvidas
- Como escolhas na hora de montar o esquema do banco afetam a eficiência dele?
- Como definir valores compostos e derivados usando ORM - alto nível.