# Gerenciamento de memória
_Como o SO gerencia a memória utilizada pelos processos? Como ele aloca e garante a segurança dos dados salvos no HD? Como o compilador organiza um programa e organiza ele para a execução? Como funciona o processo de execução de um binário executável?_

- Ler slides do capítulo 3 do Tanenbaum.

### Estratégias para não interferência dos programas com o SO considerando falta de abstração de memória:
- **Registrador de Base**: Um registrador que contém um valor que é somado em todas as referências de endereço dentro de um programa. Geralmente, esse registrador contém o endereço da última instrução do SO. Isso faz com que o código inserido na memória RAM jamais toque nas áreas de memória do SO.
	- Pensando que o SO está em 0 e o programa começa logo depois do SO.
- **Registrador de LImite**: Semelhante ao registrador de base, mas ao invés de somar nos endereços do programa para fazer eles saírem da área de memória do SO, o registrador de limite funciona para impedir que endereços acima daquele sejam acessados pelo programa.
	- Pensando que o programa começa em 0 e o SO está no final da RAM.

## Espaços de endereçamento
É o conjunto de endereços que um processo pode usar para endereçar memória. Cada programa possuí um espaço de endereçamento para ser executado e para armazenar dados necessários para seu funcionamento. Porém, o espaço de endereçamento de um programa é sempre um espaço virtual, o que levanta um problema: **como mapear este espaço virtual no espaço físico da memória?** (É aqui que entram os registradores de base e de limite).

## Swapping
Área de troca, alocada em disco. Utilizada para armazenar todos os programas em execução. Cada programa era levado em sua totalidade para a RAM, executado por uma fatia de tempo e depois trazido de volta para o Swap. Depois outro programa seria integralmente levado para RAM e assim por diante.

> A cada troca, salva-se o contexto de execução de cada programa.

É parte da evolução do gerenciamento de memória e processos.

![[Pasted image 20260422103353.png]]

## Quanta memória deve ser alocada para um processo quando ele é criado ou trocado?
Pensando na divisão de um processo em:
- **código**
- **variáveis globais**
- **heap**
- **pilha**

O código e as variáveis globais, são sempre constantes. Já a pilha e o heap podem ser alterados no meio do tempo de execução.
![[Pasted image 20260422104625.png|644]]

## Gerenciamento de memória contínua
### Gerenciamento de memória com mapas de bits (Bitmap)
A ideia é pensar no tamanho do bloco que desejamos representar, por exemplo X. Se dividirmos X em 8 partes iguais, nós teremos que:
- **X = \[X1, X2, X3, X4, X5, X6, X7, X8]**

Se X estiver totalmente desocupado: X = 00000000, se 3 partes de X estiverem ocupadas: X = 00111000.
E assim, temos como manter um controle maior de cada bloco menor de dados de um espaço maior.

A representação é simples, mas as operações de busca são mais complexas.

### Gerenciamento de memória com listas encadeadas.
Ainda pensando na ideia de dividir o espaço em pequenso blocos, podemos utilizar uma lista encadeada, em que cada nó revela informações sobre como a memória está mapeada.
- X = 00111000, nosso exemplo, a lista seria:
- \[H|0|2]->\[P|2|3]->\[H|5|3]-> NULL
	- H/P: Hole/Process | X: Starting block | Y: Length


### Imagem:
- a) memória contínua
- b) Bitmap
- c) Lista encadeada

![[Pasted image 20260422105122.png]]

## Memória virtual
Apesar do tamanho da memória crescer mais a cada ano, o tamanho dos programas estão crescendo bem mais rápido.
Então surge a necessidade de colocar em execução programas que são grandes demais para caberem integralmente na memória.

Então nasce o conceito da memória virtual:
- Cada programa tem seu espaço de endereçamento **completamente virtual**, dividido em _páginas._
- É uma generalização do registrador de base e de limite.

### Paginação
O espaço de endereçamento virtual de cada programa é quebrado em páginas, que são blocos de memória de tamanho fixo. As únicades correspondentes na memória física são chamadas de **page frames** (são os pequenos blocos em que a memória RAM foi dividida, que contém os dados que serão acessados pelo espaço de endereçamento virtual de cada programa). A memória RAM passa a carregar _páginas inteiras_ do disco, e não o _código inteiro_. Para que isso funcione, deve haver uma **MMU** no processador.

**MMU**: Memory Management Unit -> É quem faz a tradução de endereços da memória virtual para a página carregada na RAM.
![[Pasted image 20260422110724.png]]

- O número de cada página funciona como um índice na **tabela de páginas** resultando no número do page frame correspondente àquela página virtual.
- No hardware, existe um bit **presente/ausente** que indica se a página está presente fisicamente na memória.
- Se a página necessária não estiver presente, ocorre uma interrupção (**page fault**)
	- O SO escolhe um page frame pouco utilizado e escreve seu conteúdo de volta no disco.
	- Então ele carrega a página necessária da memória e para o page frame que acabou de ser descartado e reinicia a execução do código a partir da instrução que causou a interrupção.

### Tabela de páginas
- Mapeia as páginas virtuais em page frames
- É uma função que liga o número da página ao page frame contido na memória física.
- Cada lista contém o estado da página e o endereço efetivo da página (page frame).
![[Pasted image 20260422111244.png]]
![[Pasted image 20260422111258.png]]

Para evitar acessos desnecessários a memória (já que várias instruções podem resultar no acesso da mesma página) existe um cache para minimizar acessos, chamado TLB.

Um endereço da tabela de páginas contém 32 bits, porém os bits de 0 a 12 são completamente ignorados (já que cada página tem 4KB de tamanho) então os 20 bits restantes do endereço indicam qual página na tabela estamos nos referindo.

### TLB (Translation Lookaside Buffer)
É um cache de linhas da tabela de páginas, para minimizar acessos redundantes. Isso evita que várias instruções acessando a mesma página gerem vários acessos e processamentos da MMU, aliviando a carga de processamento dos hardwares associados. Contém informações sobre a presença ou não da página e o endereço efetivo dela.
![[Pasted image 20260422114327.png]]
