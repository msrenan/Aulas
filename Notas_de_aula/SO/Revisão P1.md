# Como o SO realiza o gerenciamento de memória? \[10/10]

Sabemos que a RAM, a memória primária que é onde os programas em execução ficam, é limitada em termos de capacidade. Dito isso, vamos discorrer sobre como o SO faz o gerenciamento de memória, para conseguir fazer vários programas que juntos ultrapassam o limite máximo da RAM, ou então, para colocar vários programas individualmente maiores do que a capacidade da RAM em execução, todos ao mesmo tempo. 

Primeiro, é importante pensar que dentro de um programa, ele não está em execução integralmente ao mesmo tempo, então, teoricamente, o RAM poderia carregar apenas as linhas que estão em execução no momento. Na prática, a ideia é boa, mas a execução supracitada é ineficiente, pois exigiria muitos acessos à memória secundária, o que aumentaria substancialmente o tempo dos programas e, portanto, não é a estratégia utilizada pelo SO. A estratégia real do SO, é dividir a memória RAM em **page frames**: pedaços da memória com tamanho fixo de X bytes, que dividem a capacidade total Y da RAM em partes iguais - ou seja: Y/X = número de page frames com X bytes de tamanho somado ao fato de fazer todo o programa ser inicialmente carregado utilizando um espaço de endereçamento virtual.
Esses page frames são um mecanismo de "modularização da RAM", o que irá permitir carregar partes de um programa na RAM, e não o programa inteiro. Para fins de exemplificação, vamos assumir que cada page frame tem 4KB de tamanho.

Então, sabemos que a RAM é subdividida em vários page frames, mas como o SO sabe o que colocar em cada um deles e como acessar a eles? Para isso ele usa a **tabela de páginas**, uma tabela, também armazenada na RAM, que mapeia "todas" as áreas de memória virtuais de um programa nos page frames físicos na RAM. Então cada página da tabela que representa uma página da memória virtual, possuí uma moldura física. Sendo assim, cada programa tem uma tabela de páginas que mapeia suas áreas de código na RAM, mas nem todas as áreas de código estão mapeadas ao mesmo tempo. Para iniciar a execução de um programa, o SO pode carregar apenas as áreas de dados integralmente e depois carregar somente as duas primeiras páginas da área de código, deixando **para que as próximas páginas sejam carregadas sob demanda.**

Como o SO sabe qual página carregar? Para entender isso, devemos entender sobre a estrutura virtual da tabela de páginas:
```
--------------------------------------------------------------------------------------------------------
| presença |   perms   | acesso | dirty |                        endereço físico                       |
--------------------------------------------------------------------------------------------------------
                                        0                                                              32
```

Cada linha da tabela de páginas segue o modelo supracitado, com 1 bit de presença, bits para as permissões, um bit indicando acesso, um bit indicando modificação (dirty) e 
32 bits para o endereço físico da tabela na RAM. A tabela de páginas traduz um endereço virtual em um endereço físico (todo programa é mapeado usando um espaço de endereçamento virtual, e por isso a tabela de páginas se faz necessária). Em hardware, o que processa a tabela de páginas é uma parte do processador chamada **MMU (Memory Management Unit)**, que é a unidade que acessa a tabela de páginas de cada programa, **checa se a página está presente na memória física**, e se estiver, acessa ela. Contudo, se o bit de presença marcar 0 para alguma página, acontece um **page fault**. Existem "três" tipos de page fault:
- **TLB Miss**: Não é bem um page fault, mas ocorre quando a MMU vai tentar acessar a tabela de páginas pelo cache (**TLB** *Tablepage Lookaside Buffer* - um cache que contém os endereços já acessados de uma tabela de página) e não vê a página requisitada em cache, aí ela faz um acesso a memória RAM para acessar efetivamente a tabela de páginas, e se a página estiver presente na RAM, coloca ela no TLB para acesso otimizado posteriormente e acessa o page frame.
- **Soft page fault**: É quando a MMU tenta acessar uma página na tabela de páginas mas o bit de presença marca 0. Então a interrupção do page fault é gerada. Contudo, quando o SO começa a olhar para a RAM e para a tabela de página, ele percebe que a página requisitada já está na RAM, mas por algum motivo, não está constando como presente na tabela de páginas. Então o SO atualiza a tabela de página (bit de presença e endereço, se precisar), e tudo se resolve.
-  **Hard page fault**: Também ocorre quando a MMU tenta acessar uma página que consta como não-presente na tabela de páginas. Contudo, desta vez, a página também não está carregada na RAM. Então MMU gera uma interrupção *page fault*, o SO verifica que não é um *Soft Page Fault*  e então fica responsável por carregar a página requisitada na memória e atualizar a tabela de páginas. Ao fim do tratamento dessa exceção, o SO deixa o processo pronto para ser re-executado.

Entendemos então como o SO gerencia a memória dos processos, mas está faltando um ponto essencial: **Como o SO sabe quais partes de um código são quais áreas de um programa?**

Isso é tudo feito pelo **compilador**, que ao gerar um arquivo ELF, já mapeia as áreas de memória do arquivo em memória secundária no espaço de endereçamento virtual, e assim um programa é dividido em seções: **.data, .text, .bss**, etc. Então quando um programa entra em execução, as partes de dados e a stack (que é alocada somente no início da execução de um programa) são todas carregadas integralmente, mas a parte de código (.text) não é carregada integralmente, carrega-se uma ou duas partes da área de código, e o resto vem tudo por demanda. Contudo, algumas partes como .bss não são carregados com os dados, o que acontece é que o SO realiza um **lazy allocation**, em que ele preenche várias páginas apenas com bits 0, e vai fornecendo elas para o código preencher com os dados reais conforme a demanda, o mesmo acontece com a stack, que também é alocada usando o **lazy allocation**.

Um detalhe importante: Como a MMU interpreta o endereço virtual nas tabelas de páginas? Um endereço comum tem 32bits, se pensarmos em cada page frame com 4KB, os 12 primeiros bits de um endereço podem ser interpretados apenas como um **off-set** dentro de uma mesma página. Assim, os 20 bits restantes são os índices de cada tabela de páginas.

---

# Como o SO realiza o gerenciamento de processos? \[10/10]

Antes de tudo, vamos diferenciar uma tarefa de um processo. Uma tarefa é tudo que pode ser executado, como um processo e suas threads. Um processo é "um programa em atividade", ou seja, é um programa em execução, possuindo espaço de memória próprio, um *task_struct* e alguns recursos alocados. O motivo por trás da necessidade do SO de gerenciar quais processos estão consumindo os recursos da CPU é para conseguir executar vários processos de maneira eficiente, em um curto espaço de tempo, para que assim nenhum processo monopolize o uso da CPU, para tanto, vamos mergulhar nas nuâncias do gerenciamento de processos.

Em primeiro lugar, é importante destacar como o escalonamento de tarefas funciona, já que é por meio dessa rotina que os processos entram em execução. Então o escalonador é acionado sempre que um processo é interrompido, e para não deixar a CPU a toa, outro deve imediatamente entrar em seu lugar. O SO é acionado através de uma interrupção, e ele salva o contexto de uma aplicação atualmente em execução, e depois chama o escalonador para que outra tarefa seja colocada em execução. Veja que se o SO não salvar o contexto de execução atual do processo, não há como restaurá-lo depois, ou seja, sem isso, cada processo não sairia de cena, e sim seria reiniciado. Os processos alternam entre 4 estados: 
- **Prontos**: Processos que não estão em execução, nem bloqueados e estão prontos para serem executados pelo processador, seja a primeira execução ou uma continuação da execução. Os processos neste estado chegaram aqui ou por estarem em execução e terem sido removidos, ou por estarem bloqueados e agora estarem prontos.
- **Em execução**: Eram processos prontos e agora estão efetivamente consumindo recursos da CPU e sendo executados. Vale observar que somente um processo fica em execução por ver na CPU.
- **Finalizados**: Eram processos em execução que chegaram ao fim efetivo de sua execução, sendo descartados nesta fase de pós-execução.
- **Bloqueados**: Eram processos em execução, que por algum motivo, se depararam com alguma condição que forçou a espera (I/O, espera de dados do disco, espera de dados da rede), e para não ficar ocupando a CPU com tempo de espera estes processos são bloqueados, e não podem ser re-executados enquanto a condição de espera deles não for satisfeita. Em suma, processos bloqueados são processos que estavam em execução mas precisaram aguardar alguma condição de espera, para voltarem a execução. Vale ressaltar, que os processos bloqueados não voltam imediatamente à execução, eles são primeiramente colocados junto com os processos prontos para então voltarem a ser executados.

Então, um processo em execução pode ser movido para a fase dos processos prontos pelo SO, e depois o escalonador, usando alguma política de escalonamento, vai selecionar a próxima tarefa entre as prontas para ser executada pela CPU, então o contexto dessa tarefa selecionada é restaurado e ela continua sua execução de onde parou antes do SO entrar em ação.

Agora se pensarmos em como o escalonador seleciona a próxima tarefa, devemos entender as quais os principais pontos o escalonador avalia para definir sua política de escalonamento:
- **Throughput**: Caso o processador precise diminuir o número de tarefas que está processando, ele prioriza tarefas menores.
- **Latência**: Para casos onde a tarefa submete uma informação e aguarda uma resposta, ele pode priorizar a tarefa com o menor tempo de resposta.
- **Fairness**: Prioriza um uso balanceado do processador, seja atribuindo tempos de execução iguais para todas as tarefas (fatias de tempo) ou atribuindo uma faixa de processamento para todos os processos (todos os processos devem usar X% da CPU).
- **... outros**

Sabendo que esses pontos podem ser levados em conta pelo escalonador, agora podemos definir algumas políticas de escalonamento:
- **FIFO**: A tarefa é executada até acabar ou ser bloqueada.
- **Shortest First**: A tarefa mais curta é executada primeiro. Contudo, requer total conhecimento sobre a lista de tarefas a ser executada.
- **Shortest Remaining Time**: A tarefa com menor tempo restante de execução é priorizada e executada primeiro. É uma política preemptiva, ou seja, a CPU pode comparar a tarefa atual com outras tarefas prontas para decidir o que fazer.
- **Round Robin**: Todas as tarefas recebem uma fatia de tempo enquanto prontas, e só deverão ser executadas pela CPU por, no máximo, essa fatia de tempo. Assim, fica alternando entre todas as tarefas da lista até que elas acabem, independente de quantas rodadas de fatia de tempo isso levou. É um exemplo de política justa, pois todos os programas ocupam a CPU por no máximo o mesmo período de tempo, por rodada de execução.
- **Prioridades**: As tarefas com maior prioridade são executadas primeiro.
- **Completely Fair Scheduler (CFS)**: É a estratégia utilizada pelo Linux, a qual atribui uma fatia equivalente de uso do processador para cada tarefa baseado em uma expectativa total de tempo de execução (tempo de vida da tarefa), tempo de execução corrente.

Usando essas políticas o escalonador é capaz de selecionar uma tarefa ótima para ocupar o processador em todos os momentos, o que mantém a CPU sempre ocupada com alguma tarefa.

Outro aspecto que deve ser levado em conta é **como o SO armazena e interpreta uma tarefa?** Isso ocorre por meio de uma estrutura: o **task struct**

```C
struct task_struct {

	// Aqui contém identificadores (PID, PPID, entre outros)
	// Aqui contém também endereço do arquivo no HD
	// Aqui contém também todo o mapeamento virtual do programa.

} 

```

A task_struct, contém todas as informações necessárias para que um processo possa funcionar corretamente, é nela que as variáveis de contexto ficam armazenadas, que a memória virtual fica mapeada e que todas as informações úteis de um processo ficam.

Finalmente, o último aspecto a ser pensado é que, falamos que o SO faz inúmeras ações durante o gerenciamento de tarefas, mas sabemos que a CPU executa um código por vez. Isso significa que *o código do SO não pode ficar rodando em background para conseguir identificar quanto tem que agir.* Então **como o SO retoma o controle?**

A reposta é simples: **através das interrupções**. Uma interrupção é gerada pelo processador, e faz o fluxo de execução desviar para o código do SO. Uma interrupção pode ocorrer de maneira involuntária, quando um erro acontece, por exemplo. Neste caso, é gerada uma interrupção específica, uma **exceção**, que deverá ser tratada pelo SO. As interrupções podem ser programadas, como por exemplo a interrupção do timer, utilizada para comunicar o fim de uma fatia de tempo para o SO, que irá entrar em cena para salvar o contexto e chamar o escalonador de tarefas. Podem ser geradas de forma assíncrona, também, pelos drivers e controladores de dispositivos de entrada e saída e de disco, que precisam avisar para o SO quando uma informação chega ou está pronta (para que assim, ele não tenha que paralisar o uso da CPU esperando por elas.). Por fim, as interrupções podem ser injetadas em um código via **instrução INT** ou a função **SYSCALL** da `libc`.

Por fim, é válido mencionar que todo processo nasce a partir da syscall `fork()`, que cria um processo a partir de outro, sendo o processo criado um **processo filho**. O processo filho herda o vetor de arquivos abertos e algumas outras informações do processo pai. A syscall `fork()` returna 0 dentro do processo filho e retorna o *PID do processo filho* dentro do processo pai. Isso acontece para que o processo pai tenha como gerenciar e verificar o estado dos processos filhos (e ele precisa do PID de cada processo filho para isso), e os processos filhos recebem 0, pois eles tem apenas um pai.

---

# Como o SO pode retomar o controle? \[10/10]

Como a CPU não executa dois programas ao mesmo tempo, o SO fica dormente até que ele seja acionado. O problema é: **Como o SO pode ser acionado pelo programa**?

Isso ocorre por meio das **interrupções**, geradas pelo processador, que fazem o fluxo de execução desviar para o código do SO. Uma interrupção pode ser gerada de maneira involuntária, quando um erro ocorre durante a execução, sendo assim uma **exceção**. Elas podem ocorrer de maneira programada, **como a interrupção do timer**. Por fim, elas podem ser acionadas intencionalmente, **via instrução INT, ou a chamada `syscall()`**.

Primeiro, vamos falar sobre *exceções*. Quando um programa vai executar um trecho de código que pode, por exemplo, fazer uma divisão por zero ou fazer um acesso a uma memória não alocada pelo programa, o processador gera uma interrupção específica. Essa interrupção será tratada pelo vetor de interrupções, que por sua vez contém o endereço das rotinas de tratamento de exceções do SO.

Em segundo lugar, vamos falar sobre a interrupção do timer, ela ocorre como parte da estratégia de escalonamento do SO, o qual atribui a cada processo uma fatia de tempo, quem fica responsável por manter o controle sobre a fatia de tempo de cada processo é o **timer**. Quando o timer percebe que um processo está excedendo sua fatia de tempo ele gera uma interrupção específica, que vai desviar o fluxo de execução para apontar para o vetor de interrupções, que por sua vez irá apontar para o trecho de código que contém a rotina que fará o SO retomar o controle, tirar a determinada tarefa de cena e chamar o escalonador. Como a interrupção do timer é feita integralmente via hardware, é uma interrupção assíncrona, sendo **perfeita** para o uso de gerenciamento de fatias de tempo.

Por fim, interrupções intencionais ocorrem quando o programador insere a instrução *int* ou utiliza a chamada *syscall* em seu código. Ambas geram uma interrupção, mas de maneiras diferentes. A instrução INT, recebe um parâmetro inteiro, que é um índice para o vetor de interrupções. O caminho percorrido é o seguinte: A instrução INT força o processador a gerar uma interrupção, então o fluxo de execução se volta - mais uma vez - para o vetor de tratamento de interrupções, mais especificamente, usando o número passado como parâmetro na instrução. Se estivéssemos usando essa instrução para fazer uma **chamada de sistema**, o índice passado como parâmetro deveria ser o índice do endereço da rotina de entrada genérica do kernel para as syscalls (`entry_SYSCALL_64`) que interage com a tabela de syscalls, e está dentro do vetor de interrupções. Na prática, para fazer uma syscall, a instrução *int* faz um passo a mais. Agora, pensando na chamada *syscall*, ela pode ser utilizada em C, para forçar uma interrupção. Contudo, ela não requer que você passe pelo vetor de interrupções, já que o processador mantém um registrador apenas apontando diretamente para a rotina de entrada genéria do kernel para as syscalls (`entry_SYSCALL_64`). Na prática, o caminho completo é o seguinte: O processador é forçado a gerar uma interrupção específica, a interrupção da syscall, e imediatamente, o fluxo de código aponta para o `syscall_reg` (o registrador contendo o endereço da entrada genérica do kernel), que aponta para a rotina genérica do kernel `entry_SYSCALL_64`, que por sua vez, irá interagir com a tabela de chamadas de sistema. A rotina genérica de entrada olha para um registrador que armazena, como parâmetro da função `syscall` utilizada, o número da syscall que o usuário deseja chamar.

Sobre o vetor de interrupções, é um vetor que fica sempre carregado na memória, e contém endereços para rotinas de tratamento de interrupções, podendo apontar tanto para trechos de código, como no caso de exceções, quanto para outras estruturas, como o manipulador que interage com a syscall table. Ele é essencial para o bom funcionamento do SO, já que toda exceção gerada pelo processador só pode ser tratada a partir dele.

O SO, só retoma o controle através das interrupções, independente de como elas foram geradas. Seja para trocar o processo em execução, impedir um acesso indevido à memória ou até mesmo ler dispositivos de entrada e saída externos (como mouse, teclado, placa de rede, entre outros), o SO **depende** da geração de uma interrupção para cada uma dessas situações, e também **depende** do vetor de interrupções contendo o endereço para as "instruções sobre o que fazer" para lidar com cada interrupção.

Porém ainda falta uma parte importante: **a troca de privilégios**. Existem alguns anéis de privilégio para o uso do processador:
- **Anel 0** (Kernel mode): É o anel com todos os privilégios destravados. Não existe restrição aqui.
- **Anel 3** (User mode): É o anel que possuí restrições mais relevantes, já que é o anel em que o usuário interage com a CPU e, por questões de segurança, deve ser limitado.

Quando um processo (executando em *user mode*) chama algum serviço do SO, a principal ação que deve acontecer para que o SO recupere integralmente o controle do processador é a **troca de privilégios**. Quando um serviço do SO é executado e a interrupção é gerada, op próprio processador, antes mesmo de começar a execução do código do SO, altera o registrador responsável por controlar qual anel de privilégio as instruções que serão processadas por ele devem estar, e só após alterar o privilégio que ele executa o código do SO. Feito isso, agora sim, o SO retomou **completamente**, o controle da CPU e está pronto para executar qualquer um de seus serviços.

---

# Qual a necessidade de dois processos se comunicarem entre si? Explique como isso funciona. \[10/10]

Suponha que eu preciso printar números que são obtidos através de um cálculo extremamente custoso. Fazer tudo isso em um processo, funciona, mas tem um jeito melhor. Se separamos o problema em duas partes: a geração dos números e a exibição na tela, podemos fazer dois processos: **um que só faz o cálculo, e outro que só mostra na tela**. Nós dividimos as funções e agora cada processo está focado em apenas uma ação específica. O problema é que um processo precisa pegar o resultado de outro processo, mas como?

É aqui que entra a comunicação inter processos (IPC). A IPC compõe uma série de estruturas e estratégias para garantir que dois processos possam se comunicar.

Primeiro, vamos focar em **como essa comunicação ocorre?**. A resposta é simples: *áreas de memória compartilhada*. Basicamente, adaptando ao nosso exemplo, poderíamos definir uma área comum, onde o processo que cuida da exibição na tela fica esperando um dado chegar, e o processo que faz o cálculo coloca o resultado nesta área compartilhada. Basicamente, em alguns casos, a comunicação entre processos envolve uma área de memória compartilhada, pois é por ela que as informações são trocadas. Em outros casos, a IPC ocorre apenas para uma troca de mensagens, onde dados são enviados e recebidos através do kernel, em que um processo tem seus dados copiados em alguma estrutura do kernel, e outro processo depois, lê os dados e os copia para si.

Agora, vamos entrar mais detalhadamente em como essas áreas de memória são adquiridas. Existem 3 formas:
- **Arquivo no disco**: Um jeito bastante simples de criar uma área de memória comum é manipular um mesmo arquivo do disco. Um processo escreve, o outro lê o arquivo. Toda a troca de informações fica armazenada em memória secundária, e utilizamos a API do sistema de arquivos do SO para manipular os dados com segurança. O único problema, é justamente o fato de ser a memória secundária: **acessos a disco são extremamente custosos**.
- **Estruturas do Kernel**: Uma alternativa, buscando evitar os custosos acessos à memória secundária, é utilizarmos de algumas estruturas mantidas pelo próprio kernel como formas para compartilhar memória. **Pipes, UNIX Sockets, Message Queues (msgqs), entre outros**, são exemplos das estruturas mantidas pelo kernel que podem ser utilizadas por processos que necessitam se comunicar para realizar a troca de informações. Essas estruturas são gerenciadas e mantidas pelo kernel, então o gerenciamento e manutenção da segurança delas também é dever do kernel. Mais especificamente, essas estruturas representam mecanismos de troca de mensagens, em quem um processo envia uma informação, e outro recebe.
- **Área de memória gerenciada pelo usuário**: A última alternativa é que nós como programadores aloquemos, nós mesmos, uma área de memória compartilhada e **nós devemos gerenciar ela**. Isso implica que somos nós que devemos cuidar da sincronização de leitura e escrita e outras nuânceas de IPC. Em contrapartida, nós não dependemos mais das "amarras do SO" para manipular essa área de memória, isso significa que temos total liberdade para deixar a interação entre processos do nosso jeito. Além disso, vale ressaltar que este método final é um método que deixa a informação sempre na mão dos processos, quase como se isso fizesse a área compartilhada de memória ser parte da área interna de memória de ambos os processos, sendo o menos custoso dos 3 no quesito acesso à memória. Isso pode ser feito usando a API de C *POSIX Shared Memory*, em que pedimos para o SO alocar um espaço de memória e entregá-los para nós. Segue uma implementação:
```C 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

// 1. Criar o objeto de memória compartilhada
int shm_fd = shm_open("/minha_memoria", O_CREAT | O_RDWR, 0666);

// 2. Configurar o tamanho (inicialmente é zero)
ftruncate(shm_fd, 4096); // Define 4KB

// 3. Mapear o objeto na memória do processo (obter ponteiro)
void *ptr = mmap(0, 4096, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
```

O terceiro método pode ser feito tanto utilizando dois processos separados, **quanto utilizando threads**. Uma thread, é uma linha de execução de um programa, que funciona executando uma função específica, paralelamente à execução da linha principal de execução de um programa. Todo programa, é executado na *thread principal*, e pode ou não criar mais threads dependendo da necessidade do programador. Acontece, que as threads, surgem a partir da syscall `clone()` (que é executada dentro da chamada `pthread_create()`, que clona todas as informações do processo corrente para a thread, fazendo com que as threads **naturalmente compartilhem memória dentro da área de memória de um processo**. A comunicação entre threads dentro de um processo é um caso de IPC, já que cada thread funciona como se fosse um processo separado, quando na verdade são apenas linhas de execução diferentes dentro de um mesmo processo.

Certo, abordamos o motivo que nos faz querer utilizar processos que se comunicam e como essa comunicação ocorre, mas também é necessário falar sobre alguns **problemas de IPC**. Veja bem, como garantir que um processo não vai consumir uma informação na área comum ao mesmo tempo que outro está escrevendo nela? Isso poderia corromper todo o fluxo de execução de um programa. O SO abstraí essa parte de sincronização para nós, caso utilizemos um arquivo ou uma área de memória mapeada por ele como nossa área comum. Mas no terceiro caso, onde nós mapeamos nossa própria área comum de memória, **somos nós quem devemos garantir a segurança de memória do nosso programa**. 

Nós dizemos que **a região crítica de um processo** é a região do código em que a manipulação de dados compartilhados ocorre. No âmbito da segurança de memória, o que nós devemos garantir, na prática, é que dois processos nunca executem sua região crítica ao mesmo tempo, o que causaria erros de sincronização e muito provavelmente erros graves em tempo de execução, podendo envolver acesso a áreas de memória que não são do programas (devido a endereços corrompidos), dados capturados pela metade ou até mesmo dados corrompidos na área compartilhada de memória. Sanitizar o acesso a área de memória compartilhada é o trabalho mais importante para que a comunicação inter processos funcione de maneira eficiente e segura.

Para isso, existem algumas bibliotecas de C que podem nos ajudar. Vamos ver as mais relevantes:
- **Semáforos**: Uma estrutura de controle que permite você produz algo (`sem_post`) ou consumir por algo (`sem_wait`). Ele funciona mantendo um **contador** e uma fila de processos. O contador é iniciado de acordo com a vontade do programador, pode iniciar com 0, 1, ou n, desde que sejam números positivos. Cada vez que alguma thread consome algo, o contador é decrementado. Cada vez que uma thread produz algo, o contador é incrementado. Se uma thread tentar consumir algo, mas o contador estiver em 0, essa thread é bloqueada e colocada na fila de processos, e ela fica na fila até que alguma outra thread produza algo e incremente o contador. Isso garante que nenhuma thread tentará consumir uma informação que não existe ou que está sendo produzida. Ou você consome uma informação pronta, ou você espera uma ser completamente produzida.
- **Mutexes**: Uma estrutura de controle parecida com o semáforo, mas estruturalmente diferente. Você pode travar (`mutex_lock`) ou destravar (`mutex_unlock`) um mutex. Quando você trava um mutex, **todas** as threads que também tentarem travar o mutex são bloqueadas até que o mutex seja destravado, e aí uma das tarefas bloqueadas é acordada e então pode travar o mutex (e assim o ciclo se repete). Não existe contador, nem fila de processos. Existe um processo que travou o mutex, e consequentemente todas as outras threads, e o mesmo processo deve destravar o mutex para que todos os outros fluxos de execução possam continuar. Isso garante que apenas uma thread está executando sua região crítica.

A principal diferença entre semáforos e mutexes é que se uma thread A deu `sem_post`, uma thread B pode dar `sem_wait`. Com o mutex, **o mesmo processo que deu `lock` é o mesmo que deve dar `unlock`**.

---

# O que são sinais? Como eles funcionam? \[10/10]

Sinais, a grosso modo, são como um mecanismo de notificação para processos em execução. É como se o SO enviasse uma mensagem, por exemplo "pare agora", ou "acorde". Essas mensagens são transmitidas na forma de sinais.Sinais podem ser enviados a partir de comandos específicos do teclado (em situações específicas), por outros processos e até mesmo podem ser forçados pelo programador usando a syscall `kill()` tanto em um processo quanto pelo **terminal do linux**.

Os sinais *unreliable*, os mais comuns, são associados a um número no intervalo de 1 a 31, sendo cada um transmitindo uma mensagem específica. Os sinais podem ser usados para terminar a execução de programas, paralisar e bloquear processos e até mesmo acordar processos bloqueados, tornando-os prontos. Tudo depende da situação e da funcionalidade desejada pelo programador. O SO mantém algumas estruturas de tratamento de sinais dentro do *task_struct* de cada processo, e essas estruturas permitem a um processo: **ignorar um sinal**, **instalar uma rotina de tratamento para um sinal** e **bloquear um recebimento de um sinal**. Vale ressaltar que de todos os 31 sinais, somente o **SIGKILL e o SIGSTOP**, não podem ser tratados, ignorados ou bloqueados por um processo. 

Os sinais podem estar desbloqueados, bloqueados ou pendentes. Um sinal desbloqueado é recebido e tratado normalmente por um processo. Um sinal bloqueado não é recebido por um processo, mas também não é ignorado, ficando então como um sinal **pendente**, até que o processo o desbloqueie. Por fim, sinais podem ser ignorados, em que não são nem recebidos, nem tratados e também não ficam pendentes.

Os sinais devem ser tratados, pelo SO ou pelo programador. Por exemplo, se um processo envia um sinal para acordar outro, como esse processo de **acordar outra tarefa** deve ocorrer? Tudo isso deve ser especificado na **rotina de tratamento de sinais**, que pode ser tanto a *padrão* do SO quanto um personalizada, criada pelo programador. Se o programador não instalar uma rotina de tratamento personalizada para o sinal, é executada a rotina padrão do SO para o tratamento daquele sinal. As rotinas de tratamento de um sinal - tanto do usuário quanto do SO - envolvem a *finalização, a paralisação ou a continuação* da execução de um processo.

No caso de se tratar de um processo com múltiplas threads, as threads de um processo compartilham o tratamento de sinais do processo ao qual estão associadas, mas cada uma pode ter sua própria **máscara de recebimento de sinais** - caso decida sobrescrever à herdada pela thread principal. Essa máscara é basicamente um conjunto de bits informando se a thread está bloqueando ou não o recebimento de sinais específicos. Se um sinal é bloqueado por uma thread, ele é listado como **pendente** até que a thread atualize sua máscara para desbloqueá-lo. É possível enviar sinais para um grupo inteiro de processos ou para todas as threads de um mesmo processo, assim como também é possível enviar sinais para apenas uma thread específica. Também é possível que um processo ou thread se envie um sinal - através da syscall `raise()`.

É possível também fazer um processo ficar bloqueado enquanto não receber um determinado sinal, usando a syscall `pause()`.

**Os sinais só são entregues a um processo se o PID do processo for encontrado e se o emissor do sinal tiver permissões suficientes para tal.**

O recebimento **efetivo** de um sinal só ocorre durante a restauração de contexto, mais especificamente na troca dos anéis de privilégio: Depois do contexto de um programa já ter sido restaurado, o processador sai do anel 0 e entra no anel 3 para retomar a execução normal, e somente quando ele volta para o anel 3 o sinal é entregue efetivamente ao processo. Isso ocorre porque é necessário ajustar o contexto do processo alvo, restaurando sua tabela de páginas e ponteiros para a área de código e a stack, além de retornar o processador para o **user mode** (anel de privilégio nível 3) para poder executar a rotina de tratamento instalada para aquele sinal. **Isso implica que o tratamento de um sinal só ocorre quando o contexto do processo que recebeu o sinal é restaurado**. E isso acontece pois o SO "captura" a rotina de tratamento do sinal instalada enquanto está no kernel mode e força sua execução assim que entra no user mode (ele faz isso fazendo com que o PC passe a apontar para a rotina de tratamento instalada e insere ela artificialmente no stack do usuário, como se ele tivesse chamado a função de tratamento. Isso garante que ao retornar a função, o código do usuário siga sendo executado de onde parou).

**As únicas exceções para o processo mencionado acima são o SIGKILL e o SIGSTOP, que são tratados imediatamente no envio, ou seja, o processo não tem que receber eles para que eles sejam tratados (e isso significa que quem trata eles é o kernel, *ainda no anel de privilégio 0* - e por isso eles não precisam ser entregues, já que quando o SO volta para o user mode - momento em que os sinais seriam entregues ao programa - eles já foram processados.**




---
