# Gerenciamento de Arquivos

Sabemos que um computador precisa gerenciar tanto a memória volátil (primária) para garantir o bom funcionamento das aplicações em execução, **mas também é preciso gerenciar a memória não-volátil (secundária) para garantir o armazenamento *persistente* de informações.** 

Para isso, vamos entender primeiro como **são constituídos os dispositivos de memória secundária** para, só depois, entendermos **como o SO faz a ponte entre o hardware e o software para a manipulação de informações na memória secundária**. Depois de entender como funciona a memória persistente e como ocorre essa troca de informações entre o SO e o hardware, finalmente, vamos entender **como o SO manipula e gerencia arquivos**.

## Dispositivos de memória Secundária

Um dispositivo de memória secundária é um HDD/SSD que serve para armazenar arquivos **persistentemente**. Um armazenamento é considerado *persistente* se, ao desligar a máquina, a informação não se perde. Isso é o que diferencia os dispositivos de memória persistente da memória RAM, enquanto a RAM é mais rápida e volátil, a memória secundária é mais lenta e persistente. Essa diferença na velocidade está intrinsecamente ligada a questão da **volatilidade**: a memória secundária é mais lenta **justamente** por permitir a persistência dos dados em memória.

### HDDs como funcionam?

Um HD ou Hard Disk é um dispositivo que armazena memória em discos magnéticos. Para interagir fisicamente com os discos, o HD possuí cabeças de leitura/escrita que são posicionadas na posição exata de escrita do disco. O Disco magnético em si é dividido de maneira **geométrica** (física) em:
- **Discos**: Os discos magnéticos em si.
- **Trilhas**: Anéis concêntricos.
- **Setores**: É a menor unidade de armazenamento endereçável dentro de uma trilha, geralmente 4KB.
- **Cilindros**: Conjunto de trilhas alinhadas verticalmente através de vários discos.

Essa divisão é o que proporciona a tradução do mapeamento físico no lógico. Cada HD exporta sua geometria para o SO através de seu **device driver**, para que assim o SO saiba como traduzir o endereço lógico no físico.

Agora, pensando no SO, o Disco é dividido de maneira **lógica** em:
- **Bloco**: Grupo de setores, que somados totalizam um tamanho X, sendo esse tamanho X a menor unidade utilizada pelo SO para ler e gravar arquivos. Geralmente, um bloco tem o tamanho de uma **página virtual de memória**, para que seja facilitada o carregamento dos dados lidos da memória secundária para a memória primária.
- **Partições**: Divisão primária do Disco, delimitando onde começa e termina cada sistema de arquivos gravado e definido para aquele hardware, permitindo o isolamento de dados e sistemas inteiros.

> OBS: existe duas cabeça de leitura/gravação **por disco**, e todas se movem **juntas** para a mesma posição. Não é possível mover apenas uma cabeça de maneira independente.

#### Fluxo básico de leitura/escrita
```mermaid
flowchart LR

start(("Início do fluxo"))
driver["Driver: traduz a requisição do SO"]
heads["Ajusta as cabeças de leitura"]
action{"Leitura/Escrita dos dados no setor e trilha indicados"}
fim(("Fim do fluxo"))

start --> driver
driver --> heads
heads --> action
action -- "Envia a informação lida para que o SO carregue ela em memória primária" --> fim
```

### SSDs como funcionam?

Um SSD ou Solid State Driver, diferentemente dos HDDs, não possuí nenhuma parte que se move, sendo **100% eletrônico**. Isso proporcionou um aumento **exponencial** na velocidade de utilização em comparação com os HDDS, mas o custo por hardware também aumentou e a capacidade total de armazenamento, muitas vezes, é menor que a de um HDD.

O SSD é construído em cima da **Memória NAND Flash**. Dentro de cada chip desse de memória, existem incontáveis *transistores de Porta Flutuante* - ou, em tecnologias mais modernas: *Charge Trap Flash* - que funcionam, básicamente como uma **gaiola de elétrons** que é **fortemente isolada** por uma camada de *óxido* (excelente isolante elétrico). Sendo assim, se a gaiola está com um elétron capturado, seu valor é **0** e se estiver vazia é **1**. Então, em cada chip, as milhares de gaiolas existentes formam o código binário da informação. E como a gaiola **mantém o elétron capturado, mesmo sem energia**, graças ao isolamento proveniente da camada de óxido, a informação não se perde.

> É isso esse isolamento dos transistores é a principal diferença entre o SSD e um chip de memória RAM.

O SSD organiza a informação em uma grade estrita e definida, sendo essa a sua forma de divisão física, em:
- **Célula**: Cada transistor individual.
- **Página**: Agrupamento de milhares de Células, sendo essa a menor unidade que o SSD consegue ler e escrever dados.
- **Bloco**: Agrupamento de milhares de Páginas, sendo essa a menor unidade que o SSD consegue *apagar*.

Assim como no HDD, é essa divisão que proporciona a tradução dos endereços lógicos em endereços físicos, feita pelo **device driver**, do SSD.

Agora, pensando no SO, o SSD é dividido de maneira **lógica** em:
- **Bloco**: Grupo de blocos do SSD, que somados totalizam um tamanho X, sendo esse tamanho X a menor unidade utilizada pelo SO para ler e gravar arquivos. Geralmente, um bloco tem o tamanho de uma **página virtual de memória**, para que seja facilitada o carregamento dos dados lidos da memória secundária para a memória primária.
- **Partições**: Divisão primária do Disco, delimitando onde começa e termina cada sistema de arquivos gravado e definido para aquele hardware, permitindo o isolamento de dados e sistemas inteiros.

> A divisão lógica do SSD é extremamente semelhante (quando não exatamente igual) à do SSD, justamente pois independente do tipo de hardware utilizado, o funcionamento do SO é o mesmo.

#### Fluxo de leitura e gravação em um SSD
- A **gravação** em um SSD envolve a aplicação de uma voltagem específica sobre uma célula, forçando um elétron a atravessar a camada isolante e entrar na gaiola - por meio de um fenômeno da física quântica chamado de **tunelamento**.
- Já a **leitura** em um SSD envolve a passagem de uma corrente baixa pela célula. Se a corrente passar sem dificuldades, a gaiola está vazia e seu valor é **1**, se a corrente encontrar dificuldades para passar por aquela célula, significa que os elétrons presos dentro da gaiola estão gerando um campo magnético que está gerando uma resistência para a corrente, e, portanto, o SSD sabe que aquela célula está preenchida, e se valor é **0**.

```mermaid
flowchart LR
	inicio(("Início"))
	d1{"Leitura ou escrita?"}
	
	inicio --> d1
	d1 -- "Leitura" --> L
	d1 -- "Escrita" --> E
	
	subgraph L["leitura"]
		direction LR
		loop("Início do loop de leitura")
		corr(("Aplica baixa corrente"))
		cort{"Testa a célula"}
		pos["Valor = 1"]
		neg["Valor = 0"]
		arm["Armazena o valor da célular em um buffer"]
		loopf(("Fim do loop de leitura"))
		
		loop -- "O processo ocorre até que todas as células do SSD tenham sido testadas" --> corr
		corr --> cort
		cort -- "Corrente passa sem dificuldades" --> pos
		cort -- "Corrente passa com dificuldades" --> neg
		pos --> arm
		neg --> arm
		arm -- "Ajeita o buffer para receber próximos resultados" --> loop
		arm -- "Caso todas as células tenham sido testadas, fecha o buffer e entrega ele para o SO" --> loopf
	end
	
	subgraph E["escrita"]
		direction LR
		volt(("Aplica a voltagem específica na célula"))
		tunel["Elétron atravessa a camada isolante forçadamente"]
		gai["Elétron é capturado pela gaiola"]
		fim(("Fim do fluxo de escrita"))
		
		volt --> tunel
		tunel -- "Tunelamento" --> gai
		gai -- "Por conta do isolamento, nada mais precisa ser feito, a informação já está salva." --> fim
	end
```

Agora que sabemos como o **hardware** se comporta, vamos entender como o **SO se comporta para armazenar memória persistentemente**

## Interação do SO com o hardware de memória secundária

O que nós precisamos entender sobre a manipulação de memória persistente pelo SO é:
- **Como o SO pede as informações para hardware?**
- **O que o SO faz com as informações recebidas pelo hardware?**

Essas perguntas são essenciais para entendermos como o SO lê e grava informações em memória secundária. O SO não tem acesso direto ao disco, ele precisa pedir as informações para o hardware, que deve aceitar o pedido e devolver as informações solicitadas. Essa comunicação pelo SO e o hardware é feita via **device driver**. Não vamos nos aprofundar muito sobre device drivers (agora, já que é nosso próximo tópico de estudo), mas o driver do nosso hardware espera uma requisição de leitura ou uma requisição de escrita.  Em resumo, ambas requisições devem ter:
- **onde** na organização lógica do SO está a informação requisitada/deseja-se gravar a informação
- número de bytes (**tamanho**) que se deseja ler/escrever.
- onde na **RAM** serão carregados os dados lidos/estão os dados a serem gravados.

Então, os SO mandam a requisição para os drivers de disco, que traduzem o endereço lógico em físico e manipulam seus respectivos hardwares, baseados na requisição feita pelo SO, executam a operação e retornam para o SO, ou uma confirmação - no caso da escrita - ou os dados requisitados - no caso da leitura.

> OBS: Os controladores de disco pedem o **endereço na RAM** na requisição pois são capazes de realizar *Direct Acess to Memory* (DMA), na memória RAM, podendo escrever ou ler conteúdos da **RAM** sem o intermédio do SO ou da CPU.

Isso responde nossa primeira pergunta: `'Como o SO pede as informações para o hardware?'`: O SO envia uma requisição contendo o endereço de memória lógico dos dados que serão manipulados, indicando qual operação será realizada (leitura ou escrita), indicando quantos bytes serão manipulados e o onde na RAM devem ser colocados os dados, em caso de leitura, ou, de onde na RAM devem ser retirados os dados, em caso de gravação. Uma vez enviada a requisição, o driver verifica a memória RAM da máquina, para ajeitar o espaço para inserir informações em caso de leitura ou para checar as informações já existentes,em caso de escrita, depois realiza a tradução do endereço lógico do **sistema de arquivos** em um endereço físico para o hardware de memória secundária, realiza os procedimentos necessários pré-operação (que divergem dependendo do tipo de hardware SSD ou HDD) e, finalmente, realiza a operação da requisição. Uma vez finalizada a operação, o driver sinaliza para a CPU que finalizou a operação requisitada pelo SO e uma interrupção é gerada para que o SO lide com a finalização da operação.

> Vale ressaltar que em caso de leitura, no momento em que a confirmação é enviada pelo driver, os dados lidos já estão em RAM para que o SO manipule eles. Isso implica que, uma vez acionado, o SO deve inserir esses dados na tabela de páginas do processo que requisitou eles. Em caso de leitura, a confirmação indica o fim da gravação dos dados em memória persistente.

Agora, uma vez que a operação requisitada é realizada, o SO precisa lidar com a resposta do driver, que pode ser - em caso de leitura - uma confirmação que significa que os dados já estão carregados em RAM, ou - em caso de escrita - que os dados já foram armazenados de maneira persistente, o que significa que, dependendo do intuito da aplicação, o SO já pode descarregá-los da RAM.

Pensando na leitura, ao receber a confirmação, o SO precisa realizar todo o processo de inserção dos dados na tabela de páginas do programa que requisitou essas informações, para que assim, finalmente, os dados possam ser utilizados de maneira efetiva pelo processo que utilizou a syscall `read()`. 

Isso responde nossa segunda pergunta: `O que o SO faz com as informações recebidas pelo hardware?`.

Pois bem, agora nós entendemos todo o fluxo da transferência de informações entre o SO e os dispositivos de memória secundária. Dito isso, agora precisamos entender como isso tudo é utilizado pelo SO para o **gerenciamento de arquivos**.

## Como o SO gerencia arquivos?

Essa é a pergunta principal que desejamos responder. Agora que somos capazes de entender o contexto por trás de conceitos fundamentais do gerenciamento de arquivos - **leitura e escrita de dados em memória secundária** - podemos subir um nível e começar a entender toda a organização lógica que um SO faz para manipular esses arquivos. Mas antes de entendermos o que é um **sistema de arquivos**, vamos primeiro entender **o que é um arquivo para o SO**.

### O que é um arquivo para o SO?
Um arquivo é uma unidade lógica de informação criada por um processo. Um dispositivo de memória secundária normalmente conterá milhões de arquivos em si, todos diferentes. Um arquivo deve ser armazenado **sempre** de maneira persistente, o que significa que caso o processo que o criou acabe, o arquivo ainda permanece, independente de estar atrelado a um processo ou não.

Um arquivo fornece uma maneira para o SO abstrair região de informações de um dispositivo de memória secundária, isolando o usuário dos detalhes de como e onde essas informações estão armazenadas fisicamente. Todo arquivo criado por um processo recebe um **nome**, que facilita o acesso pelo sistema de arquivos, posteriormente. O nome de um arquivo, comumente é separado em duas partes - por um '.' - sendo a segunda parte, a **extensão** do arquivo. A extensão, é uma indicação visual do **tipo** do arquivo. É visual, pois todo arquivo possuí, logo em seu início, um **magic number** que indica para quem estiver lendo o seu tipo, independentemente da extensão utilizada. O tipo de arquivo é importante pois processos podem estar esperando apenas tipos específicos de arquivos, e a extensão facilita a filtragem de arquivos por parte do sistema de arquivos.

Arquivos, internamente, podem ser organizados em:
- **Sequência de bytes**: o SO não sabe e não se importa com o que está escrito neste tipo de organização de arquivo. Seu significado é inteiramente atribuído por outra aplicação em UserSpace. Os bytes são colocados todos crus dentro do arquivo. É uma organização extremamente flexível, já que o SO se preocupa em ler bytes e carregar bytes apenas, enquanto so programas em UserSpace se preocupam com a interpretação.
- **Sequência de registros**: nesta organização, o arquivo é dividido em *registros de tamanho fixo*, o que significa que, por exemplo, cada linha representa um registro. Para que este tipo de organização funcione, é necessário que a operação de escrita **adicione ou sobreponha um registro**, e a de leitura **retorne um registro em uma posição especificada**. É uma organização muito usada em banco de dados pois facilita a manipulação de uma estrutura fixa de dados.
- **Árvore de registros**: nesta organização, o arquivo em si representa uma *árvore de registro de tamanhos variados*, o que significa que toda a manipulação de dados deste tipo de arquivo é feita utilizando a árvore que ele representa. Este, também é muito utilizado em bancos de dados, já que essa organização acelera muito a busca, inserção e remoção de registros dentro do arquivo.

Existem alguns tipos comuns de arquivo - *tipos esses diferentes do tipo que é sinalizado pela extensão de um arquivo, relacionados à estruturação interna* - vamos entendê-los:
- **Arquivo comum/executável**: Contém um cabeçalho que possuí: o magic number, algumas métricas e o um ponteiro para o início da seção de dados, além de algumas flags e fora do cabeçalho, existem a seção de texto, de dados, entre outras.
- **Diretório**: Contém vários cabeçalhos contendo o nome dos arquivos contidos em si, suas datas de criação, proprietários, permissões e por fim, logo após o fim do cabeçalho de cada arquivo, um ponteiro que aponta para o endereço do primeiro bloco do arquivo em si.

Entendemos, então, o que é um arquivo para o SO, e sabemos que os arquivos são unidades lógicas de informação que mapeiam blocos físicos do disco, mas como funciona esse mapeamento? 

#### Tipos de alocação

Dependendo do SO, o jeito que se decide quais blocos de disco vão ser alocados para cada arquivo pode variar, vamos entender os principais:
- **Alocação Contígua**: Consiste em armazenar cada arquivo como uma execução contígua de blocos de disco, ou seja *um do lado do outro*. É o tipo de alocação mais simples de ser implementado, pois basta saber o endereço do primeiro bloco e o número de blocos daquele arquivo para se obter o arquivo completo, e sabendo o número do primeiro bloco é possível descobrir qualquer outro bloco utilizando simples adições. É uma alocação extremamente eficiente para a **leitura**, pois basta buscar o bloco **uma vez** para lê-lo completamente. A desvantagem dessa alocação, é que o disco pode ficar **fragmentado** - isto é, com lacunas entre dois blocos - caso um arquivo que se encontra no meio de outros 2 (no ponto de vista dos blocos no disco) seja deletado.
- **Alocação por lista encadeada**: Consiste em manter cada arquivo como uma lista encadeada de blocos de disco. Para tanto, o início de cada bloco de disco é usado como um ponteiro para o próximo bloco. Diferente da alocação contígua, não existe fragmentação de disco nesse caso, pois sempre que um espaço ficar vago, outro arquivo pode preenchê-lo com um de seus blocos. Contudo, a facilidade para se encontrar qualquer bloco é perdida aqui, já que é preciso buscar no disco todo pelo primeiro bloco do arquivo desejado.
- **Alocação por lista encadeada utilizando uma tabela na memória**: Consiste em uma otimização da *alocação por lista encadeada*. visando eliminar o acesso aleatório de extrema lentidão, a ideia aqui é colocar o ponteiro de cada bloco em uma **tabela de ponteiros** armazenada em memória RAM, tornando o acesso aleatório é facilitado. Essa tabela, chamada de **FAT (File Allocation Table)**, é uma otimização considerável, mas apresenta uma desvantagem principal: *a tabela inteira deve estar carregada em memória RAM, a **todo momento**, para que este método funcione*.
- **I-nodes**: Um método semelhante ao supracitado, porém resolvendo sua principal desvantagem. Falaremos dele quando estivermos estudando a **estrutura do sistema de arquivos.**

Certo, agora que entendemos o que é um arquivo e como um arquivo se associa com o disco, vamos entender **o que é um sistema de arquivos**.

## Sistema de arquivos

Vamos destrinchar a fundo qual é o propósito do sistema de arquivos para um SO e vamos entender como é essa tecnologia que **torna possível a manipulação e gerenciamento de arquivos do jeito que conhecemos.**

Para começar, vamos discutir sobre a função do sistema de arquivos. Sua função é ser uma **abstração** para as interações do SO com o disco. Essa interação que o SO vai fazer pode ser apenas de 2 tipos:
- **Leitura**: O SO deseja ler o conteúdo de um arquivo armazenado no hardware de memória secundária. Cabe ao sistema de arquivos receber esse pedido, e interagir com o driver do hardware em nome do SO.
- **Escrita**: O SO deseja gravar uma informação da memória primária para a memória persistente. Cabe ao sistema de arquivos receber esse pedido, e interagir com o driver do hardware em nome do SO.

Beleza, sabemos então que o Sistema de Arquivos vai abstrair o pedido do SO para o device driver. É só isso que ele faz? **Não.** Enquanto entendíamos como os dispositivos de hardware (HDD ou SSD) funcionavam, nós aprendemos que *o hardware possuí uma divisão física que proporcionava a **tradução de endereços lógicos** em endereços físicos*, mas até então, nunca havíamos parado pra pensar "De onde vêm esses endereços lógicos?", bem a resposta é simples: **do sistema de arquivos**. Para entender como isso funciona, devemos entender primeiro a **organização do sistema de arquivos**.

### Estrutura Interna do SA (Sistema de Arquivos)

Sabemos que um disco pode ser **particionado**, e como vimos anteriormente, cada partição permite o **isolamento** de dados e **sistemas de arquivos inteiros**. Vamos começar deste ponto. Um disco, contém, obrigatoriamente, *MBR - Master Boot Record* - correspondendo ao **Setor 0 do disco**, com exatos 512 bytes, contendo um gerenciador de boot do próprio disco e uma **tabela de partições** - para que o disco saiba, assim que inicializar, quais blocos estão dentro de cada partição. 

Então, logo após a tabela de partições, começam as partições normais de um disco, em que, cada uma representa **obrigatoriamente**, um sistema de arquivos diferente - **independente desse SA estar associado a um SO diferente ou não.**  As partições normais, são subdividias, na grande maioria das vezes em:
- **Bloco de inicialização**: Contém o código do **bootloader**, que é responsável por carregar o sistema operacional.
- **Superbloco:** Contém parâmetros-chave a respeito do sistema de arquivos e é lido para a memória RAM assim que o computador é inciado, ou quando o sistema de arquivos é **montado**.
- **Gerenciamento de espaço livre**: Contém informações a respeito dos blocos de disco mapeados no sistema de arquivos na forma de *bitmaps* ou uma *lista de ponteiros*.
- **I-nodes**: Contém uma lista com **todos** os **I-nodes** registrados no SA. (Daremos ênfase em **o que é um i-node e qual sua importância**, em breve).
- **Diretório-raiz**: Contém o ponteiro para o **i-node** do diretório raiz do sistema de arquivos (no universo UNIX, é o famoso: "/")
- **Arquivos e diretórios**: Contém toda a árvore de **i-nodes** que mapeiam a estrutura hierárquica de pastas e arquivos. *Árvore essa cuja raiz é o diretório "/".*

É chegada a hora então de entendermos, finalmente, **o que é um I-node**.

#### I-nodes

É o **tipo de alocação** de arquivo mais otimizado atualmente, em que cada bloco de um arquivo é associado a um *i-node (index-node)*, que lista os atributos e os endereços de disco dos blocos referentes ao arquivo desejado. Se você possuí o i-node de um arquivo, é possível encontrar **todos os blocos daquele arquivo**. A vantagem do esquema de i-nodes sobre a *lista encadeada tabelada em RAM* é que o **i-node de um arquivo só precisa estar carregado em memória quando o arquivo *está aberto***. Um problema envolvendo os i-nodes, é que o número de endereços que ele comporta é fixo, e caso um arquivo ultrapasse esse número de blocos de disco, a solução é reservar o último espaço de endereço de disco para que ele armazene o endereço de um bloco que contém o resto dos endereços do arquivo que não couberam no i-node.

Certo, agora que sabemos o que é um **i-node** e entendemos qual a relação dessa estrutura com **arquivos**, precisamos entender qual é a relação dessa estrutura com **diretórios**.

Normalmente, se não utilizarmos i-node, um diretório precisaria manter os **atributos** de cada arquivo dentro dele no **cabeçalho do arquivo** presente no ponto de entrada do diretório. Utilizando i-nodes, basta que o diretório guarde o **nome do arquivo** e o **ponteiro para o i-node do arquivo**, que conterá todas as informações de atributos e endereços de blocos de disco. A pergunta natural a se fazer agora é *como é armazenado esse ponto de entrada de um diretório?*, a resposta você já sabe: **em um i-node**. Ao invés de conter os atributos do arquivo e um ponteiro para os blocos de disco do arquivo, um i-node que representa um **diretório**, contém atributos do diretório e **ponteiros apontando para cada i-node dos arquivos de dentro daquele diretório.**

Isso faz com que a estrutura do sistema de arquivos se pareça com esse diagrama:
```mermaid
flowchart TB

root["/"]
A["User A"]
B["User B"]
C["User C"]

P1["Pasta 1"]
A1["Arquivo 1"]
A2["Arquivo 2"]

root --> A
root --> B
root --> C

B --> P1
P1 --> A1
P1 --> A2
```
 Em que cada nó do diagrama representa um **i-node**.

Dessa forma, entendemos como se estrutura um **Sistema de Arquivos**. Entendemos também como ele organiza a **hierarquia de pastas e diretórios** usando i-nodes. Mas ainda faltam responder algumas perguntas.

## Se existem diferentes tipos de sistema de arquivos, como o SO consegue executar a mesma operação em todos eles?

Os diferentes tipos de sistema de arquivos variam na ordem que as informações são armazenadas, nos atributos dos i-nodes, na hierarquia de pastas e entre muitas outras coisas. **Como o SO sabe se comunicar com todos os diferentes tipos de SA?** A resposta é bem simples: **Virtual File System - VFS.**

O SO utiliza um Sistema de Arquivos Virtual para fazer os seus pedidos de *leitura e escrita*. O que ele faz, é pedir a leitura/escrita para o VFS, que nada mais é do que uma **API** que padroniza funções padrões de **read()** e **write()**, que devem existir para **TODOS** os sistemas de arquivos. Na prática, o que o SO faz é algo parecido com: `ext4.vfs.read()` ou `vfat.vfs.read()`.

## Os endereços armazenados em um i-node, são os endereços físicos dos blocos no disco?

A resposta é **NÃO!** Os endereços armazenados em um sistema de arquivos são **endereços lógicos** que são traduzidos para **endereços físicos** pelo **controlador de disco**. Quanto o SO pede para abrir um arquivo, ele passa o **caminho do arquivo** (lista com o nome de todos os diretórios necessários para chegar no i-node do arquivo, desde a **raiz do sistema de arquivos**) para o SA, que por sua vez vai acessar todos os diretórios - seguindo o caminho fornecido - até chegar no arquivo (caso seja o caminho errado, retorna erro), então consulta os i-nodes para pegar os **endereços LÓGICOS dos blocos de disco** que contenham a informação que o SO quer acessar, envia esses endereços obtidos para a **camada de blocos**, que organiza os endereços para rearranjar a ordem de leitura para uma leitura otimizada e, finalmente, envia os endereços rearranjados para o driver de disco realizar a interação física.

## O que significa "montar um sistema de arquivos"?

Quando você cria uma partição de disco e insere um sistema de arquivos nela, o SA não fica **imediatamente** pronto para uso. É preciso montá-lo primeiro. A montagem de um sistema de arquivos acontece durante a inicialização da máquina, também.

Ao iniciar o processo de montagem, o kernel lê o superbloco daquela partição para salvar em memória RAM atributos essenciais do sistema de arquivos que está sendo montado. Depois disso, o kernel guarda o endereço do diretório-raiz do SA - e em caso de montagem de um SA fora da inicialização, define o diretório raiz do SA2 como sendo a pasta do SA1 que você indicou ao iniciar o processo de montagem - e assim a montagem se dá por concluída.

**"E a árvore de i-nodes? Não é iniciada na memória RAM também durante a montagem?"**
**NÃO!** Isso por que seria um desperdício de tempo preencher **TODA** a árvore de i-nodes direto na montagem do SA. O que ocorre é um preenchimento gradual **sob demanda**. A árvore só é preenchida na primeira vez que tentam acessar aquele caminho. Caso contrário, os **i-nodes não visitados NUNCA entram na subdivisão de Arquivos e Diretórios.** Por isso que acessar um arquivo na segunda vez é mais rápido que na primeira: quando você acessa pela primeira vez, você está pavimentando a estrada. Na segunda vez, com a estrada já pavimentada, é só ir direto no arquivo.

A árvore de todos os i-nodes **sempre** existe em memória secundária. Contudo, o **cache de i-nodes**, não é carregado totalmente preenchido na memória RAM durante a montagem, e é preenchido gradualmente.

Agora sim, podemos dizer que **entendemos** como o Linux gerencia arquivos. Mas você pode estar se perguntando: "E aquela comunicação entre o SO e os device drivers, como funcionam?", esse é nosso próximo tópico de estudo.

---

# Entrada e Saída de Dados

Você já se perguntou **como o SO se comunica com a rede?**, ou então **como o SO se comunica com hardwares externos ao processador, como placa de vídeo, placa de wi-fi, hardwares de memória secundária, mouse, teclado, entre outros?**

Pois bem, isso tudo é possível graças ao **gerenciamento de entrada e saída** realizado pelo SO. Agora que entendemos bem como funciona um sistema de arquivos, podemos entender profundamente como a E/S (Entrada e Saída) funciona, já que **tudo é feito a partir das duas operações fundamentais da manipulação de arquivos: read() (leitura) e write() (escrita)**. Sim, você não entendeu errado, *o SO utiliza o SA para gerenciar a entrada e saída de dados*, já que para receber dados que estão **entrando**, por exemplo, ele utiliza a syscall `read()` para receber os dados.

Antes de entendermos esse gerenciamento, vamos entender **com quem o SO vai trocar dados**.

## Dispositivos de E/S

Um dispositivo de E/S é todo hardware externo ao processador que pode, através de uma conexão que nós já iremos detalhar, tentar se comunicar com o SO (que está sendo executado pela CPU). Os dispositivos de E/S são categorizados, simplificadamente em dispositivos de **bloco** ou dispositivos de **caractere**, em função dos tamanhos das transferência de dados (de 512B a 64KB) e do tipo de acesso (sequencia ou aleatório).

De maneira geral, dispositivos de E/S são conectados ao controlador de E/S apropriado para seu uso, através de interfaces como *SATA, SCSI, USB, Thunderbolt, FireWire, Serial ou Paralela*.

 Uma unidade de E/S, genericamente, é composto por um componente mecânico (onde o dispositivo de E/S se acopla) e um componente eletrônico chamado de **controlador do dispositivo ou adaptador**.

Isso significa que a porta USB-A do seu notebook ou computador, possuí um controlador próprio para ela, que gerencia as informações que entram - de um teclado, por exemplo. Contudo, hardwares extremamente específicos como um HDD/SSD podem possuir controladores internos para interações internas dentro daquele hardware -  por exemplo, o controlador interno do HDD é quem reposiciona as cabeças de leitura.

As funcionalidades desse controlador são comumente providas por circuitos na placa mãe ou em alguma placa adaptadora conectada a algum **barramento** do computador, como o *PCIe*, por exemplo. Controladores possuem registradores de controle internos, que servem para a passagem de comandos e para a verificação do **status** de suas operações. Alguns controladores também possuem **buffers** de dados que podem ser **usados para comunicação**.

> A comunicação entre o SO (sendo executado pela CPU) e os controladores ocorre com o envio de comandos e de dados pelo **barramento ao qual o controlador está conectado.** 

Certo, agora que entendemos o que são os dispositivos com os quais o SO vai trocar dados, vamos entender mais afundo **como essa comunicação ocorre**.

## Como o SO se comunica com os dispositivos de E/S?

Essa comunicação pode ocorrer de duas formas:
- **Usando instruções (IN e OUT) do processador para leitura e escrita no barramento**
- **Usando E/S mapeada em memória (Memory Mapped I/O)**

A técnica de *Memory Mapped I/O (MMIO)* é mais eficiente e simplificada de usar pelo SO já que as interações vão ocorrer simplesmente lendo e escrevendo dados em áreas da memória RAM **reservadas para cada controlador** de dispositivo. Para tanto, algumas faixas de endereços são excluídas, sendo que essa **exclusão** é definida por um acordo de parâmetros negociados via protocole de controle dos dispositivos conectados ao barramento PCI/PCIe. Uma vez reservadas, **essas faixas de endereços passam a ser filtradas no chipset de controle de acsso do processador à memória, deixando sob responsabilidade dos dispositivos o tratamento das suas respectivas faixas de endereço reservadas.** Isso significa que, quando um controlador reserva um endereço, toda vez que o processador executar uma instrução para **salvar um dado em um endereço reservado**, o chipset que controla o acesso da CPU na memória irá desviar o fluxo de dados diretamente para o controlador do dispositivo que fez a reserva daquele endereço, para que os dados sejam armazenados no buffer do controlador e possam ser interpretados, posteriormente, pelo dispositivo de E/S.

Dentro dos controladores de dispositivos existem processadores dedicados que são responsáveis por traduzir os comandos recebidos da CPU em instruções específicas para o dispositivo de E/S conectado à unidade de E/S. Na prática, isso significa que o SO não precisa conhecer os detalhes específicos de **todos** os dispositivos de E/S que estão conectados na máquina, basta que ele saiba como interagir com os controladores.

> Isso significa também que o SO pode enviar comandos para que os controladores tratem de forma **autônoma** a entrada e saída de dados dos dispositivos, sejam eles de bloco ou de caractere.

A parte do SO que fica responsável por **interagir com um controlador de E/S** é chamada de **device driver**. Esse código é escrito pela **fabricante** do dispositivo e **instalado** na máquina pelo próprio **usuário**, para que o SO se torne capaz de se comunicar com o controlador do dispositivo. Caso você tente utilizar um componente externo, sem instalar o driver apropriado para sua utilização, muitas vezes o dispositivo **nem será reconhecido pelo SO**.

Como os controladores de E/S possuem processadores próprios, a comunicação entre os diferentes núcleos de CPU e os processadores dos controladores ocorrem através de algum **barramento**. Por ser um barramento, qualquer controlador conectado àquele barramento vai ter acesso às informações que forem propagadas ali. Portanto, é preciso utilizar algum mecanismo para **endereçar** para qual controlador deseja-se realizar a comunicação. Para isso, existem as **portas de entrada e saída (I/O ports)**, endereços pré-definidos pela indústria em faixas para cada tipo de dispositivo. Existe alguma liberdade de configuração para cada controlador de dispositivo dentro dessas faixas.

> Para controladores conectados ao barramento PCI, por exemplo, há até mesmo um protocolo para a negociação dos endereços que serão utilizados pelos controladores.

As instruções de escrita e leitura diretamente no barramento são feitas com **instruções privilegiadas do tipo IN e OUT**.

Agora que sabemos o que é **MMIO** e como funcionam as **I/O ports**, devemos entender um importante conceito para a comunicação entre CPU e os controladores: **Direct Access to Memory (DMA)**

### DMA

O Acesso Direto à memória ocorre quando um dispositivo de E/S coloca um dado na memória RAM **sem que esse dado passe pela CPU**, o que significa que o dado vai parar **direto no barramento de memória**.

Vamos entender como isso funciona mais detalhadamente. Para que o DMA funcione, existem **4 registradores** internos que a CPU acessa via MMIO para dar ordens:
- **Registrador de Endereço de Memória**: Guarda o endereço de *destino* na RAM
- **Registrador de Contador de Bytes**: Guarda o tamanho do arquivo/bloco que precisa ser movido
- **Registrador de Controle**: Define a direção (Leitura ou Escrita)
- **Registrador de Status**: Indica se o DMA está ocupado, livre ou se terminou a tarefa

Primeiro, a CPU manda a ordem, ajeitando os valores dos 3 primeiros registradores e enviando um sinal para que o controlador comece a preparar os dados. Após isso, a CPU volta ao fluxo normal de execução. Depois, o controlador do dispositivo busca os dados na mídia física e os coloca em seu buffer interno de comunicação. Quando esse buffer fica cheio, o controlador ativa uma linha física de sinal elétrico ligada **diretamente ao DMA** chamada *DREQ (DMA Request)*, na prática significa que o controlador está avisando que os dados estão prontos em seu buffer interno.

Então, após receber o aviso, o DMA envia um sinal **HOLD ou Bus Request** para a CPU, que faz a CPU terminar o ciclo atual do que estava fazendo e desligar temporariamente as saídas elétricas daquele barramento, respondendo com um **HLDA Hold Acknowledge**, o que significa que *a CPU está bloqueando o barramento para que apenas o DMA envie informações*.

Com o barramento inteiro para o DMA, os dados do buffer interno do controlador são liberados no barramento, sendo interceptados pelo DMA que injeta esses dados **diretamente no barramento de memória**, fazendo com que eles fiquem salvos na memória física da RAM, **sem ter que passar pelo processador.**

Finalmente, quando o DMA confirmar que todos os dados foram transferidos, ele pede para que a CPU desative o **HOLD**, liberando o barramento e dispara uma **interrupção física (IRQ)** para a CPU. Por conta da interrupção, o SO assume o controle, vê que os dados estão carregados na memória e avisa o programa que pediu os dados que eles ficaram prontos.

> Não existe, atualmente, um chip específico para realizar o DMA, mas existem, nos próprios controladores de dispositivos rápidos e em alguns canais internos do PCH, componentes que juntos operam para que o DMA funcione corretamente.

> Uma curiosidade, é que os dados circulam em um barramento, tal qual um pacote circula na rede. O que significa que o PCH envia um "**pacote**" pelo barramento. Esse pacote contém um cabeçalho com diversas informações, entre elas uma flag que informa se o pacote deve ser entregue utilizando DMA. É assim que os componentes dispersos pelos barramentos e controladores sabem quando uma informação deve ser entregue via DMA e quando deve ser entregue normalmente pelo barramento.

Beleza, agora entendemos **como o SO se comunica com os dispositivos de E/S**, mas uma coisa que não ficou totalmente clara é **como o SO, os controladores, e os dispositivos de E/S estão fisicamente conectados.**

## Conexões físicas

A primeira coisa que precisamos entender é que a CPU, a memória RAM e os controladores de dispositivos estão **interligados** na placa mãe, e podem ocorrer interações entre eles.

No mundo dos processadores intel, existe o **PCH (Platform Controller Hub)**, sendo esse o chip **principal** da maioria das placas mães, funcionando como um grande concentrador e tradutor de todos os periféricos e controladores de E/S mais lentos, *servindo como ponte entre eles e a CPU*.

Fisicamente, a divisão pode ser resumida pelo diagrama a seguir:

```mermaid
flowchart TB

subgraph CPU
	c["Cores"]
	cnt["Controlador de Memória"]
	
	c --> cnt
end

gpu["GPU/NVMe"]

c -- "PCIe Direto (extrema velocidade)" --> gpu

ram["Memória RAM"]

cnt -- "Barramento de memória DDR4/DDR5" --> ram

c -- "Barramento DMI" --> p

subgraph p["PCH"]
	x["Contém os controladores para: - Portas USB - Portas SATA - Wifi / Rede / Áudio - Linhas PCIe secundárias"]
end

```




Vamos destrinchar esse diagrama:
- **Barramento de Memória**: Liga a CPU diretamente na memória RAM, é controlado pelo **Integrated Memory Controller (IMC)** que fica dentro da CPU. É um barramento paralelo de altíssima velocidade e baixíssima latência. Serve **exclusivamente** para buscar instruções de código e dados que a CPU precisa processar imediatamente.
- **Linhas PCIe Diretas**: Linhas de barramento que ignoram o PCH e vão direto para os componentes, geralmente que não podem sofrer gargalos, e por isso transmite informações em **alta velocidade**. É usado pela GPU principal e por **SSDs M.2 NVMe**.
- **Barramento DMI**: Barramento que conecta a CPU no **PCH**, para que o processador consiga interagir com as demais interfaces (ex: USB) e com os demais dispositivos de E/S.

**Como essa disposição de barramentos é utilizada na prática?**
- **MMIO**: Quando a CPU executa uma instrução para um endereço que está marcado para MMIO, um circuito interno da CPU olha o endereço, percebe que é um endereço que foi reservado por um dispositivo de E/S, e então **ao invés de enviar o sinal elétrico da CPU para o barramento de memória (o que resultaria em um acesso comum à memória) ele envia o sinal pelo barramento DMI, em direção ao PCH**, para que assim, o dispositivo de E/S receba uma instrução da CPU e possa realizar a operação solicitada.
- **Trabalho do PCH**: O PCH recebe esse sinal do processador vindo do DMI, reconhece que ele é **destinado ao controlador de E/S USB interno** e então propaga esse sinal na linha física da porta USB-A, onde está o seu teclado, por exemplo.
- **DMA**: Se o seu SSD está fazendo uma transferência via **DMA**, para a RAM, o controlador de dispositivo do SSD joga os dados nas linhas **PCIe do PCH**. O PCH empacota isso e propaga no barramento **DMI**, a CPU recebe esse pacote e, através do seu controlador de memória interno, joga o dado **diretamente** no barramento de memória, salvando tudo na RAM física. Tudo isso **sem incomodar os núcleos da CPU**.

Agora, nós já sabemos bastante sobre entrada e saída. Mas algo importante a ser comentado é: **em que momentos ocorre uma operação de E/S**, já que ainda não sabemos **quando**, isso que estamos estudando vai ocorrer.

## Operações de Entrada e Saída

Bom, nós já sabemos como a E/S funciona, mas **quando exatamente** isso vai ocorrer?
- **E/S programada:** o SO interage com um dispositivo de E/S e fica esperando a conclusão da operação em **busy waiting** - quando ele fica esperando a conclusão da operação sem liberar a CPU para que ela faça outras tarefas.
- **E/S orientada à interrupção**: O SO interage com um dispositivo de E/S e espera a conclusão da operação requisitada, **mas sem monopolizar a CPU.** O que acontece aqui, é que é esperada que **uma interrupção seja gerada quando a operação pedida pelo SO chegue ao fim**, o que fara com que o SO retome o controle e conclua o que quer que ele estivesse fazendo que precisava dessa informação de E/S.
- **DMA**: Uma transferência direta entre o dispositivo de E/S e a memória, com uma interrupção sendo gerada ao fim da transferência para informar que os dados estão prontos.

Agora que entendemos mais sobre as operações de entrada e saída, podemos dizer que compreendemos profundamente os aspectos que permeiam essa seção do SO. Contudo, ainda assim, existem algumas perguntas que não foram respondiads:

## O que contém um Device Driver?

Sabemos que os **drivers** são códigos desenvolvidos pela fabricante do dispositivo de E/S que, uma vez instalados, "ensinam" o controlador de E/S a se comunicar com determinado dispositivo conectado a uma interface. O código de um driver inclui tratamento de interrupções do dispositivos. como operar na ativação do dispositivo e como operar após a ocorrência de uma interrupção, como tratar endereços, entre muitas outras coisas. Um device driver é como se fosse o **dicionário** de um controlador para se comunicar na língua do respectivo dispositivo de E/S.

## Como o SO reconhece dispositivos de E/S?

Essa é uma pergunta muito pertinente, pois o SO precisa conhecer **TODOS** os dispositivos conectados e precisa saber **TODOS** os *barramentos* com os quais eles estão conectados. Para descobrir isso, o SO acessa informações mantidas pela BIOS do sistema computacional (**SMBIOS**), ou então, pode testar cada barramento em busca de dispositivos presentes. No LINUX, ele pode consultar essas informações utilizando o **dmidecode.** Dessa maneira, o SO pode descobrir quais dispositivos de E/S estão conectados à máquina e também em quais barramentos eles estão atuando.

Caso o SO precise consultar informações sobre **quais dispositivos estão utilizando quais I/O ports**, no linux, a relação de endereços é guardada em `/proc/iomem`

Por fim, falta um último e crucial detalhe: **no começo, afirmamos que o sistema de arquivos é utilizado pelo SO para gerenciar a entrada e saída**, e agora, é o momento de entendermos isso.

## Como o SA se relaciona com E/S

Dentro do sistema de arquivos, existem alguns diretórios que **não possuem relação com o disco físico**. Esses diretórios são especiais, e existem somente no **cache de i-nodes**, para que o SO consiga dar um **endereço lógico**, para elementos que são manipulados a partir *das chamadas de sistema de gerenciamento de arquivos*: `open()`, `read()` e `write()`.

Um desses diretórios é o **`/dev`**, que é utilizado pelo SO para dar um endereço lógico para **TODOS** os controladores de dispositivos presentes na máquina.

Quando conectamos um pendrive - por exemplo - em uma porta USB-A da nossa máquina, é criado um i-node dentro do `/dev` que corresponde ao controlador de disco que interage com o pendrive. Dentro desse i-node, são gravados 2 números mágicos:
- **Major Number**: Identifica qual é o **Driver** responsável por aquele hardware.
- **Minor Number**: Identifica qual a *instância* do dispositivo (ex: é o segundo pendrive plugado).

Isso significa que caso um programa tente **escrever** dados no *i-node* do disco da sua máquina, quando o SO executar a syscall `write()` o VFS vai identificar que o i-node passado como parâmetro não se refere a um arquivo, **mas sim a um controlador de dispositivo**, e então irá desviar a requisição direto para **as funções de escrita do driver**, que por sua vez irão acionar o **MMIO/DMA** para executar a operação.

> Tudo isso só é possível graças ao VFS que aumenta ainda mais a camada de abstração provida pelo sistema de arquivos, fazendo com que a máxima de que no mundo Unix **tudo é um arquivo**, seja verdadeira.

---
