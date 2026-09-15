# Introdução Rápida

## 1. O que é uma snapshot de um processo?

Uma snapshot é uma foto instantânea de algo, muito utilizada no processo de salvamento e recuperação de dados e sistemas. É uma estratégia diferente, porém análoga ao *backup*. Ambos funcionam muito bem antes.

Uma snapshot de um processo, envolve gravar **tudo relacionado ao estado do processo atual**, ou seja: variáveis de controle do processador, estado do sistema operacional no momento da foto, estado do disco no momento da foto e muito mais coisas.

## 2. Nosso seminário

Sendo assim, iremos apresentar sobre a **snapshot de processos realizada pelo Incus.** O Incus é um gerenciador de máquinas virtuais e containers LXC, que funciona - via REST API - criando e gerenciando diversas VMs. O que nós vamos abordar, especificamente, é como o Incus realiza snapshot de processos, para conseguir salvar o estado de **containers e VMs inteiras!**

---
# Alto nível

## 3. Como o Incus faz a snapshot?

Quando você faz o comando para realizar a snapshot, o cluster do Incus recebe a requisição `instanceSnapshotPost` e então ele segue dois caminhos, dependendo do tipo de snapshot:
1. **Container LXC**: Para fazer uma snapshot de um container, o incus congela toda a instância, chama o **CRIU (Checkpoint-restore in Userspace** para realizar o **dump** do container (tirar a snapshot) e então usa o dump para criar a snapshot.
2. **VM com QEMU**: Para fazer uma snapshot de uma VM inteira, o Incus utiliza um artifício do próprio *QEMU* para fazer um **state dump**, depois despausa a snapshot e usa o dump para criar a snapshot.

Para ambos os casos, após obter o **dump**, a função `snapshotCommon()` é chamada e é ela quem cria a snapshot em si. Nessa função são realizadas operações do cópia do storage pool e sincronização com o banco de dados interno do Incus. Em seguida cria a snapshot na lógica interna do Incus, adicionando no banco de dados de instâncias.

Em seguida, é realizada a lógica de copiar as áreas de memória secundária (chamadas de **pool**). O jeito como isso é realizado depende **MUITO** do storage backend implementado. No geral, ele copia o storage da instância original.

## 4. Como o Incus restaura uma snapshot?

Quando você faz o comando para restaurar uma snapshot, o Incus recebe a requisição `instancePut` e `instanceSnapshotRestore` e então segue dois caminhos, dependendo do tipo da snapshot:
1. **Container LXC:** Para restaurar uma snapshot de um container, primeiro a instância do LXC é pausada, depois desmonta-se o **rootfs** do container e a partir do rootfs desmontado, restaura-se o **rootfs da snapshot**. Depois, chama o **CRIU** para que ele finalize a restauração. E finalmente, reinicia a instância.
2. **VM com QEMU:** Para restaurar uma snapshot de uma VM inteira, primeiro a instâncai da VM é pausada, então é chamada a função `pool.RestoreInstanceSnapshot` que irá restaurar completamente a snapshot na VM pausada. Por fim, reinicia-se a instância.

## 5. Como o Incus faz uma cópia a partir de uma snapshot?

Quando você faz o comando para cópia a partir de uma snapshot, o Incus recebe a requisição `instancesPost` e então chama a função `MigrateReceive` dentro de `createFromCopy().CreateInstanceFromMigration()`, que vai analisar o tipo de snapshot, e decidir:
1. Se for **LXC**, chama o **CRIU Migrate**.
2. Se for **VM com QEMU**, utiliza os próprios artifícios internos do QEMU.

---

# O CRIU

## 6. O que é o CRIU?

O CRIU (Checkpoint-Restore in Userspace) é uma ferramenta do Linux que permite **congelar uma árvore de processos em execução** e salvaer o seu **estado no disco**. Esses metadados guardados pod3em ser usados para restaurar a aplicação permitindo que ela retoma a execução **no exato momento em que o estado foi paralisado**.

### Ciclo de vida do CRIU
1. **Mapeamento e Congelamento (Checkpoint/Freeze)**
2. **Extração de Dados (Dump)**
3. **Reconstrução (Restore)**

### a) Mecânica do checkpoint e freeze
Para realizar o congelamento de cada tarefa, o CRIU delega as chamadas de sistema para uma biblkioteca interna especializada chamada **COMPEL (CRIU Parasite Engine for Linux)**.

### b) Extração de dados

Com a árvore de tarefas paralisada, o CRIU precisa coletar todas as informações relevantes e guardá-las em arquivos dump para que elas possam ser utilizadas na restauração.

São extraídas informações como:
- File descriptors, através de `/proc/$PID/fd/`
- VMA inspecionado `/proc/$PID/smaps` e `/proc/$PID/map_files/l`
- Os metadados da tarefa e o estado dos registradores originais via `ptrace(PTRACE_GETREGS)` e `/proc/$PID/stat`

Além disso o CRIU **injeta um código parasita no espaço de endereçamento de cada tarefa**, cujo trabalho é **coletar informações adicionais restritas**, como as credenciais de segurança e o conteúdo **real da RAM**.

### c) Reconstrução do processo

O processo de reconstrução ocorre com o CRIU se transformando na tarefa que será restaurada. Em alto nível, isso ocorre em 4 etapas:
1. Resolução de recursos compartilhados
2. Recriação da árvore de processos
3. Restauração de recursos básicos
4. Troca de contexto e execução final

---

# Syscalls utilizadas no processo

## 7. Cenários possíveis

Primeiro, antes de entendermos como o Kernel reage à todo esse processo de snapshot de um processo, devemos compreender os dois cenários possíveis:
- **Container LXC**: Um container não é uma máquina isolada, e sim apenas uma pasta cheia de arquivos. Seu armazenamento não é um disco virtual, mas sim uma árvore de diretórios raiz (**rootfs**) atrelada ao **VFS** do host. Assim, para fazer o snapshot, o Incus precisa **entrar nessa pasta e copiar arquivo por arquivo**.
- **VM**: O kernel enxerga a máquina virtual como um grande arquivo contíguo (geralmente uma imagem `root.img` ou `qcow2`) gerenciado por um **hypervisor**. O Incus não "olha" dentro desse arquivo, ele apenas **coordena a cópia do disco.**

Como consequência, para realizar uma snapshot **stateless** (sem preservar o estado da RAM), o kernel não tem a premissa de "clonar um dispositivo de bloco", na verdade, ele é forçado a realizar um *path walking* através do VFS, manipulando **i-nodes**, transições de **dentries** e páginas de memória para reconstruir o estado do sistema de arquivos em um diretório de backup.

## 8. Strace: Quantidade x Latência

Ao analisar um snapshot em um storage backend não otimizado (diretório sobre ext4 ou xfs), o `strace` revelou que aproximadamente **233.000** chamadas foram feitas. O cruzamento entre o volume das chamadas e o tempo percentual gasto, mostra uma inversão no comportamento do escalonador:

- **Alta frequência e Baixa Latência (O VFS e o dcache):** Chamadas como `newfstatat`, `getxattr` e `openat` dominam em volume, mas representam menos de 5% do tempo total de execução. O Linux mitiga a latência de I/O através do subsistema de cache do VFS. Durante a varredura do **rootfs do container**, o pathwalk aciona o **dentry cache (dcache)** e  **i-node cache (icache)**. Na maioria das chamadas `newfstatat`, o kernel utiliza primitivas de sincronização RCU (Read-Copy-Update) para ler metadados já carregados na memória (estrutura `struct inode`), evitando o custo altíssimo de um Page Fault maior ou de buscar dados no disco.
- **Baixa frequência e Alta Latência (Espera Bloqueante):** As chamadas `wait4` e `waitid`, invocadas pouco mais de 10 vezes, consumiram aproximadamente 92% do tempo total de execução. Aqui, o daemon do Incus realizou a criação de processos filhos (via `clone` com flags específicas ou `fork`) para delegar operações massivas (como cópia usando `rsync`). O kernel, ao receber o `wait4`, altera o estado do processo pai de `TASK_RUNNING` para `TASK_INTERRUPTIBLE`. O processo sofre uma troca de contexto voluntária e é retirado da fila de execução da CPU, sendo inserido em uma fila de espera do processo filho. O tempo médio aqui não representa uso do processador, mas sim **ociosidade bloqueante** até que o kernel receba a interrupção `SIGCHLD` atestando a conclusão do filho.

## 9. Analisando as syscalls para LXC

![[Pasted image 20260712195716.png|772]]

### a) Resolução e Metadados

A syscall `openat` inicia a resolução de caminho (`path_lookup`) alocando **file descriptors** na `fdtable`. A `newfstatat` invoca a função interna `vfs_statx()`, preenchendo a estrutura `kstat`. O Incus precisa extrair os bits de modo de acesso, a propriedade (**UID/GID** sob namespaces) e os ponteiros de tempo (`mtime`), garantindo que o **i-node original seja clonado fielmente.**

### b) Atributos Estendidos e Segurança

Cruciais para a segurança em containeres. O VFS repassa essas chamadas (`getxattr` e `llistxattr`) aos drivers do sistema de arquivos utilizado para extrair os **extended attributes (EA)**. É nestes atributos que residem as **Linux Capabilities** (`cap_permitted` e `cap_effective`) e os perfis do *LSM* (AppArmor/SELinux). Sem essa cópia bit a bit, o container seria restaurado **com seu isolamento corrompido.**

### c) Consolidação Atômica

Após a cópia para um diretório temporário, o Incus invoca `rename` **mais de 12.000 vezes**. Internamente, `vfs_rename()`, o kernel aplica bloqueios (`inode_lock()`) em ambos os diretórios para garantir exclusão mútua. Em sistemas com *journaling*, a transação é atômica (**ACID**), prevenindo que um *kernel panic* corrompa a árvore do container no meio da operação.

## 10. E em VMs?

Enquanto para o container LXC, cerca de **233.000** chamadas foram feitas, o mesmo snapshot aplicado em uma VM gera cerca de **1.200** chamadas, apenas.

Como os arquivos da VM são invisíveis para o host, as syscalls não focam no **VFS (`openat`)** e sim no **subsistema de rede (`sendmsg`/`recvmsg`)**. O Incus comunica-se via Sockets Unix com o daemon QEMU (usando o **QEMU Machine Protocol - QMP**), solicitando a pausa das threads do **KVM (vCPUs)**.

Com a VM suspensa, a cópia do arquivo de imagem ocorre a nível de blocos. O processo utiliza, geralmente, sinalizadores `O_DIRECT`, que instruem o subsistema de I/O do kernel a realizar **DMA** diretamente do disco para os buffers de usuário, **contornando o Page Cache da RAM**. O gargalo sai da CPU processando **dentries** para o **throughput do controlador SATA/NVMe**.

## 11. Por que a arquitetura do storage backend importa?

É claro notar que esse número avassalador de syscalls ocorre devido às limitações do storage backend padrão (`dir`). Como o VFS é cego para os blocos físicos do disco, ele é forçado a operar no nível lógico dos **i-nodes**.

Se o host utilizasse um sistema de arquivos **Copy-on-Write (CoW)**, como ZFS ou BtrFS, o log do `strace` seria muito diferente. O snapshot seria acionado via uma única chamada de controle (**`ioctl`**). O Driver do ZFS no kernel não copiaria dados ou metadados, ele simplesmente bifurcaria os ponteiros lógicos e incrementaria os **reference counts** dos blocos. A complexidade algorítmica cairia de **O(N)** para **O(1)** - para **N** sendo o número de i-nodes - abstraindo totalmente o usuário do peso do I/O.

## 12. Snapshot stateful: Tem como salvar o estado da RAM?

A análise acima foca em uma snapshot *Stateless*, que ignora o estado da RAM na hora de gerar o **dump**, lidando com a imutabilidade em disco. Contudo, como preservar a memória RAM e a **execução ativa de um container?**

Para VMs, o Hypervisor gerencia e congela a memória alocada ao convidado facilmente. Contudo, em um LXC, os processos **compartilham a RAM do host diretamente**. O kernel Linux **não possui** uma syscall nativa para salvar a memória de um container.

Para contornar essa limitação, o Incus delega ao **CRIU**, que realiza um malabarismo utilizando syscalls para **serializar o estado da máquina em arquivos**:
- `ptrace`: Iterrompe e assume o controle dos processos (`TASK_TRACED`), extraindo até os valores dos registradores físicos da CPU
- **Inspeção de memória:** O CRIU analisa as estruturas do VMA (**Virtuam Memory Area**), em `/proc/$PID/maps` para ler ativamente as páginas de memória de cada process (Heap, Stack).
- **Rede (`sock_diag` via Netlink):** Extrai as máquinas de estado de sockets TCP (sequence numbers, buffers, etc), permitindo que conexões de rede em andamento sejam congeladas e posteriormente ressuscitadas **sem derrumar a conexão.**

---

# Snapshot Stateful: CRIU Internals

## 13. Como o CRIU age por baixo dos panos?

Para realizar uma snapshot stateful em um LXC, o Incus delega essa tarefa ao CRIU, uma ferramenta que opera em userspace comunicando-se com o kernel via syscalls! É ele quem lida com os processos internos para fazer o salvamento do estado atual da tarefa (o container, no caso). Ele permite que o processo seja congelado no meio de uma requisição, salvando em disco e depois (ou em outra máquina) seja restaurado no exato momento em que o processo parou.

O CRIU segue os seguintes passos:

### a) Checkpoint (Dump):

Para gerar o **dump** de um processo, o que o CRIU faz, genéricamente, é andar recursivamente pela árvore `/proc/` do processo.
1. **Coleta a árvore de processos e congela (Freezing):** O PID do processo líder do container é encontrado. Com isso, via `/proc/$PID/task` são coletadas as threads do processo, e via `/proc/$PID/task/tid/children` os processos filhos, recursivamente. Durante essa caminhada, as tasks vão sendo paradas através da syscall `ptrace`.
2. **Coleta os recursos da tarefa e faz o Dump (Dump):** Neste momento, o CRIU lê todas as informações coletadas das tarefas congeladas e grava essas informações em arquivos de imagem (.img). Os recursos obtidos são:
	- VMAs (via `/proc/$PID/smaps`) e os arquivos mapeados (via `/proc/$PID/map_files` - links simbólicos).
	- File descriptors (via `/proc/$PID/fd` e `/proc/$PID/fdinfo`).
	- Parâmetros principais de uma tarefa (como registradores e outros metadados) são exibidos por meio da interface da `ptrace` e pela entrada `/proc/$PID/stat`
3. **Injeção do Código Parasita:** Para extrair memória de uma forma eficiente, o CRIU injeta um binário no **espaço de endereçamento da tarefa**, assim, contorna as limitações do `ptrace`. Iste é feito em três passos:
	1. Primeiro, o CRIU salva o estado atual dos registradores do processo (incluindo o RIP) e em seguida, usa o `ptrace` para forçar o processo a executar o `mmap` e alocar uma nova página de memória
	2. Segundo, o código parasita é copiado para essa nova área alocada. Então o CRIU troca o registrador RIP para apontar para o parasita
	3. A partir desse ponto, o processo alvo acorda e executa o código parasita do CRIU por dentro do seu próprio contexto. Isso permite fazer a coleta das credenciais e varrer o conteúdo completo de sua memória anônima (Heap/Stack).
4. **Cleanup:** Depois que o dump foi feito completamente para o disco (incluindo páginas de memória, que só podem ser gravadas dentro do espaço de endereço do dump), o CRIU usa a `ptrace` novamente para remover o código parasita e restaurar o código original do processo. Então o processo é deixado suspenso e o CRIU **finaliza o checkpoint**.

### b) Restore
O processo de restor consiste no CRIU assumir a identidade (PIDs) das tasks que serão restauradas e as reconstruas, consistindo em 4 passos:
1. **Resolve Shared Resources:** O CRIU lê os arquivos de imagem (.img) para descobrir quais processos estão compartilhando quais recursos. Se vários processos estavam compartilhando um mesmo arquivo ou segmento de memória, o CRIU elege um destes processos para ser o líder e, a partir dele, restaurar os recursos, garantindo que os outros processos herdem **hierarquicamente** os recursos.
2. **Process Tree Fork**: É quando o CRIU chama `fork`(ou `clone3()`) diversas vezes para re-criar os processos. As threads de cada processo são recriadas mais tarde.
3. **Restauração de recursos básicos:** Agora, o CRIU precisa restaurar recursos básicos como:
	- Local exato do memory mapping
	- Timers
	- Credenciais
	- Threads
	Nesse passo, o CRIU abre os arquivos, prepara namespaces, mapeia áreas de memória privada (e as preenche com dados), cria sockets locais, chama as rotinas como `chrdir()` e `chroot()`. Porém, o restauramento dos recursos cruciais (timers e threads), são atrasados - para o quarto passo.
4. **Finalização**: Como os processos criados na Fase 2 estão rodando o binário do próprio CRIU, a memória deles precisa ser apagada (`munmap`) e substituída pela memória original do container lida do disco. Para não apagar a si próprio enquanto faz isso, o CRIU insere um código "trampolim" (restorer blob). Esse blob é um pequeno pedaço de código executável que fica num endereço de memória que não colide com o mapa de memória do container. O processo salta para este trampolim, que realiza o `munmap` do ambiente do CRIU e utiliza o `mmap` final para sobrepor a memória original (Heap/Stack). É neste mesmo salto final que as credenciais voltam para o nível do usuário do container, os timers são reativados e as threads são recriadas a partir dos PIDs principais para evitar inconsistências no layout virtual que acabou de ser injetado.

## 14. Processo de Dump destrinchado

### a) Entry Point: `cr_dump_tasks()`
Essa função é chamada quando o user faz um comando de ump. ela parte de uma inicialização de alto nível e delega o dump (de verdade) para a função **`cr_dump_finish()`**.
Responsabilidades chaves:
- Inicializa o conjunto de imagem global (globa_imgset)
- Abre os file descriptors do diretório de destino da snapshot.
- Valida a compatibilidade entre a arquitetura e as flags da CPU do host
- Chama **`cr_dump_finish()`** para fazer o dump real
- Gerencia o modo de coordenação entre processos antes do congelamento definitivo

Fluxo de chamadas:

`main()` -> `cr_dump_tasks(opts.tree_id)` -> `cr_dump_finish()`

### b) Process Tree Collection and Freezing
Antes do dump começar, é preciso identificar todos os processos na árvore do alvo e **congelá-los**. Isso é feito por muitos componentes trabalhando em conjunto:
1. **Process Tree Discovery (`collect_pstree()`):** a função `collect_pstree` percorre a hierarquia de processos começando pelo PID do processo root:
	1. Lê `/proc/<PID>/task/`para descobrir as threads
	2. Lê `/proc/<PID>/task/<TID>/children` para encontrar processos filhos
	3. Constroi uma árvore hierárquica (`struct pstree_item`)
	4. Valida a sessão e relações do grupo de processos
2. **Process Seizure (`seize_catch_task()`):** Assim que a árvore é identificada, cada processo precisa ser congelado usando `ptrace`:
	![[Pasted image 20260712225511.png]]
	O processo de apreensão usa a flag `PTRACE_SEIZE` para evitar que o kernel mande `SIGSTOP` (pois poderia manchar o estado ao executar rotinas de tratamento). Depois de apreender o processo, a flag `PTRACE_INTERRUPT` é enviada para parar o processo.
	A struct `dmp_info` atrelada a cada `pstree_item` contém estados específicos do **dump**, como: **controlador do parasita, estruturas de controle das threads e estado da memória coletada**

### c) Injeção do Parasita
Assim que as tarefas são congeladas, o CRIU injeta um código parasita (um blob de código independente da posição) diretamente no **espaço de endereçamento do processo alvo.** 
Como a leitura externa via `ptrace` é custosa, muitos recursos só podem ser lidos de maneira eficiente **por dentro do próprio processo**, como:
	- Valores de armazenamento local da thread
	- Estado completo dos registradores (como FPU/SIMD)
	- Máscara e manipuladores de sinais
	- Credenciais do processo
	- Conteúdos da memória (é mais rápido que via `ptrace`)

Fluxo de Injeção do código parasita:
![[Pasted image 20260712230151.png]]

A função `parasite_infect_seized` coordena esse processo usando a biblioteca COMPEL.

#### Comandos do parasita

Uma vez que o código parasita está rodando dentro do contexto do container, o daemon do CRIU passa a atuar como um **cliente**, enviando **comandos remotos (RPC)** para o parasita:
![[Pasted image 20260712230342.png|1123]]

Esses comandos são emitidos via `compel_rpc_call()` e `compel_rpc_sync()`.

O COMPEL é um utilitário para executar um código arbitrário em um contexto de processo **estrangeiro**. Ele faz parte do CRIU.

O código arbitrário a ser executado **é o parasita**. Uma vez compilado com as flags do COMPEL e empacotado, ele pode ser executado **dentro do contexto de outra task**. *Note que o código está em um ambiente sem as funções da libc*.

A execução do parasita sempre começa com a função do COMPEL `std plugin` que foi linkada ao binário do parasita durante o estágio de **empacotamento.**

#### Rodando o parasita

No momento de infectar uma tarefa com o código parasita, é preciso realizar os seguintes passos:
- Parar a task, chamando `compel_stop_task(int pid)`. O valor retornado deve ser salvo, caso dê certo, pois contém o estado da task.
- Preparar o gerenciador de infecção, chamando `compel_prepare(int pid)`. O valor retornado é um ponteiro para `struct parasite_ctl`.
- Rodar o código remoto:
	- Executa uma syscall com `compel_syscall(ctl, int syscall_nr, long* ret, int arg ...)`
	- Infecta o alvo com o blob parasita chamando `compel_infect(ctl, nr_thread, size_of_args_area)
- Curar o alvo chamando `compel_cure(ctl)`
- Continuar a tarefa chamando `compel_resume_task(pid, orig_state, state)` com o valor do estado salvo no primeiro momento.

### d) Orquestração do dump per-task

Com o parasita hospedado, **para cada task na árvore de processos, o mecanismo de dump chama `dump_one_task()`**, que orquestra a coleta de estado por task.
**Sequência de task dump:**
![[Pasted image 20260712231130.png]]

Principais funções **per-task dump**:
- `collect_mappings()`
	1. Processa `/proc/$PID/maps` para pegar informações de Áreas de Memória Virtuais (VMA)
	2. Chama `parse_smaps()` para ler estatísticas detalhadas de memória
	3. Constrói um `struct vm_area_lis` com todas as VMAs
- `dump_task_core_all()`
	1. Coleta o estado da tarefa (personality, credenciais, etc.)
	2. Chama `parasite_dump_thread_leader_seized()` para ter o estado da thread líder.
- `dump_task_mm()`
	1. Escreve um memory map de **metadados (MmEntry)** para as imagens
	2. Inclui uma lista de **VMAs**, endereços de code/data/stack, auxv, etc.
	3. Ainda **não** escreve o conteúdo da página de memória atual (isso é feito mais tarde)
- `dump_task_files_seized()`
	1. Itera sobre `/proc/$PID/fd/`
	2. Para cada **fd**, identifica o tipo (regular file, socket, pipe, etc.)
	3. Chama un handler específico de dump fia (`fdtype_ops->dump()`)
- `parasite_dump_pages_seized()`
	1. Extrai o conteúdo da página de memória
	2. Usa o parasita para, eficientemente, transferir as páginas via pipes
	3. Lida com soft-dirty tracking para um dump incremental
	4. Escreve no arquivo `pages-*.img`

### e) Memory Dumping (`parasite_dump_pages_seized()`)

Dump de memória é uma das partes mais complexas e críticas em relação à performance do mecanismo de dump. Essa função (`parasite_dump_pages_seized()`) é responsável por **extrair o conteúdo atual da página de memória**.

**Arquitetura do Memory Dump:**
![[Pasted image 20260712232045.png]]

Passos chave:
1. **Scanning de VMAs** - `generate_iovs()`: Percorre a lista de VMAs. Para cada página, lê `/proc/$PID/pagemap` para decidir como ela deve sofrer o **dump**. A regra é a seguinte:
	- A página só é candidata ao **dump** se estiver presente na RAM e não for uma página zerada (ainda não tocada)
	- Se a VMA é **file-backed** (mapeada de um arquivo/biblioteca, o próprio executável ou um **mmap** de arquivo) e a página não foi modificada (não sofreu copy-on-write), ela não sofre **dump de conteúdo** - no restore, é simplesmente remapeada a partir do arquivo original em disco.
	- Só entram de fato em **pages.img**: páginas anônimas (heap, Stack `mmap(MAP_ANONYMOUS)`) e páginas file-backed que sofreram **CoW**.
	- Isso significa que a distinção relevante é se o conteúdo ainda bate com o que está no arquivo em disco, ou já divergiu. Um mapeamento read-only (como o .text de um ELF) nunca diverge, ou seja, **nunca sofre dump**. Um mapeamento read-write `MAP_PRIVATE` só sofre dump se a página específica foi de fato modificada.
2. **Page Transfer via Parasite** - `drain_pages()`:
	- Envia `PARASITE_CMD_DUMPPAGES` ao parasita
	- O parasita chama `vmsplice()` para enviar as páginas de memória para dentro de pipes do kernel
	- Ocorre uma transferência zero-copy da memória do processo direto par ao buffer do pipe, sem passar pelo espaço de endereçamento do próprio CRIU.
	- O CRIU lê do pipe e escreve nos arquivos de imagem
3. **Suporte a Dump Incremental:**
	- Usa o tracking de **soft-dirty** (via `/proc/$PID/clear_refs`)
	- Só faz o **dump** de páginas modificadas desde o último dump (útil no **pré-dump para live migrations**)
	- Cria cadeias de imagens "pai -> filho" para eficiência.

### f) Thread Dumping

Cada thread de um processo multi-threaded deve ter seus respectivos estados capturados separadamente. A função `dump_task_threads()`é quem lida com isso:
![[Pasted image 20260712233255.png]]

**Estado por thread:** Cada thread recebe sua própria mensages *protobuf CoreEntry* contendo:
![[Pasted image 20260712234711.png|858]]

A thread líder (a primeira) recebe estado adicional no seu *CoreEntry*:
- Credenciais da Tarefa (CredsEntry)
- Limites de recurso (TaskRlimitsEntry)
- Timers (TaskTimersEntry)
- Personality
- Código de Saída (se for um zombie)

### g) Estado de Namespace e Cgroup

O mecanismo de **dump** precisa capturar a ssociação de namespace e cgroups de cada processo, necessária para restaurá-lo no contexto **correto** de execução.

#### Namespace Dumping:
![[Pasted image 20260712235240.png]]

- **Dump de Namespace:** A função `dump_task_ns_ids()` captura essa informação a partir dos links simbólicos `/proc/$PID/ns/*`, o número de i-node de cada link identifica **unicamente** a instância daquele namespace.
- **Dump de cgroup:** a função `dump_thread_cgroup()`:
	- Lê `/proc/$PID/cgroup` para obter os caminhos de cgroup
	- Para cada controlador, determina o **caminho do cgroup**
	- Opcionalmente, faz dump das propriedades do cgroup (CPU shares, limites de memória, etc.)
	- Escreve um **CgroupEntry nas imagens**.

### h) Shared Kernel Objects

O CRIU rastreia quais objetos do kernel são compartilhados entree processos usando comparações via chamada `kcmp`

```c
// In get_task_ids() [criu/cr-dump.c:696-723]
ids->vm_id = kid_generate_gen(&vm_tree, &elem, &new); // Memory space
ids->fs_id = kid_generate_gen(&fs_tree, &elem, &new); // FS info (cwd/root)
ids->files_id = kid_generate_gen(&files_tree, &elem, &new); // FD table
ids->sighand_id = kid_generate_gen(&sighand_tree, &elem, &new); // Signal handlers
```

Isso permite ao CRIU:
- Detectar memória compartilhada entre processos (ex: após `clone(CLONE_VM)`)
- Detectar tabelas de file descriptors compartilhadas (ex: após `clone(CLONE_FILES)`)
- Evitar realizar o dump do mesmo dado várias vezes.

### i) Coleta de Estado de Sinais

O Estado de Sinais do processo precisa ser capturado para restaurar sinais pendentes. A função `dump_task_signals()`é quem cuida disso.
- **Dump da fila de sinais**: O CRIU captura tanto filas de sinal compartilhadas (por processo) quanto privadas (por thread), usando `ptrace(PTRACE_PEEKSIGINFO, ...)`:
	```c
	// From dump_signal_queue() [criu/cr-dump.c:1042-1122]

	struct ptrace_peeksiginfo_args arg;
	arg.nr = SI_BATCH; // 32 signals at a time
	arg.flags = group ? PTRACE_PEEKSIGINFO_SHARED : 0;
	arg.off = 0;
	
	while (1) {
		siginfo_t si[SI_BATCH];
		int nr = ptrace(PTRACE_PEEKSIGINFO, tid, &arg, si);
		if (nr <= 0) break;
		// Store signals in SignalQueueEntry
		for (int i = 0; i < nr; i++) {
		queue->signals[queue->n_signals++] = encode_siginfo(&si[i]);
		}
		arg.off += nr;
	}
	```
- **Informação armazenada por sinal**: Número do sinal, PID/UID do remetente, dados específicos do sinal (para `SIGCHLD`, `SIGPOLL`, etc.), código do sinal (`SI_USER`, `SI_KERNEL`, `SI_TIMER`, etc.) são informações guardadas no *CoreEntry* de cada thread: `thread_core->signals_p`(sinais privados pendentes por thread) e `thread_core->signals_s` (sinais compartilhados por processo).

### j) Image Writing and Finalization:

Depois de todo o estado coletado, o mecanismo de **dump** escreve **TUDO** em um arquivo de imagem e faz a limpeza.
- **Gerenciamento do conjunto de imagens:** Todas as imagens são gerenciadas por uma `struct cr_imgset` (o `glob_imgset` global), que fornece uma interface unificada para escrever mensagens *protobuf* em cada tipo de imagem:
	```c
	// From write_img_inventory() [criu/crtools.c]
	struct cr_imgset *glob_imgset;
	
	// Open image set
	glob_imgset = cr_imgset_open(O_DUMP);
	
	// Write to specific image type
	pb_write_one(img_from_set(glob_imgset, CR_FD_PSTREE), &pstree_entry, PB_PSTREE);
	pb_write_one(img_from_set(glob_imgset, CR_FD_CORE), &core_entry, PB_CORE);
	```

#### Tipos de imagens escritos durante o dump
![[Pasted image 20260713000542.png|684]]

### k) Cleanup and Process Termination:

Depois do dump, o mecanismo de dump faz a limpeza via `parasite_cure_seized()` e, então, faz uma das três opções:
1. **Matar os processos (padrão)**: Envia `SIGKILL` para todas as tasks capturadas
2. **Deixar rodando (--leave-running)**: destaca o `ptrace` e deixa os processos continuarem
3. **Deixar parado (--leave-stopped)**: destaca o `ptrace`, mas deixa os processos em estado parado.

---

