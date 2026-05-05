# Processos / Task

#### Estrutura de representação
BCP -> Bloco de controle de processo
No linux é conhecido como task_struct, é uma lista que carrega todos os processos criados, podendo estar ou não em execução dependendo do estado de cada processo. Funciona, na prática, como uma grande lista encadeada.


#### Estados de um projeto
Um processo pode existir em 3 estados:
- Pronto: Aparece quando um processo é criado, a partir de uma syscall. Também pode aparecer quando um processo em execução atinge o fim de sua fatia de tempo. Ou quando um processo mais prioritário está pronto para ser executado, então um processo já em execução volta para o estado de Pronto.
- Bloqueado: Acontece quando um processo em execução esbarra em uma condição de execução não satisfeita. Quando essa condição é satisfeita, o processo bloqueado volta para o estado de Pronto.
- Em execução: Acontece quando o escalonador (scheduler) seleciona um processo que está **pronto** e coloca ele em execução.
- Terminado: _exit_ / _fatal error_  (Fim de execução normal de um processo / Sinais fatais que podem aparecer em processos Bloqueados ou Prontos)

#### Troca de Contexto
Sempre que um processo entra/sai de execução, o contexto em que ela estava deve ser salvo e o contexto do precesso que está entrando deve ser restaurado. Um **contexto**, neste caso, refere-se ao estado de execução dos registradores do hardware no momento que um processo vai sair de execução - ex: Flags, PC, etc. Geralmente, isso é feito dando um _push_ na pilha corrente de execução do processo. Quando um processo tem seu contexto restaurado, o processador volta o PC para o PC salvo na pilha de execução corrente do processo e descarrega todas as coisas salvas na pilha.

"Entender os estados e as transições, entender que a troca de contexto é necessária e entender como uma troca de contexto funciona na prática."

O salvamento de contexto ocorre, praticamente, na pilha do task_struct do processo.

# Chamadas de Sistema
São os serviços que o SO oferece para os programas de usuário, para que os programas do usuário consigam interagir com o SO e consequentemente com o Hardware, sem sacrificar a segurança que somente o SO é capaz de prover.

Os códigos das Syscalls não são ligados no código do usuário durante a fase de compilação do código, o que acontece é que o compilador insere as chamadas de sistemas no código, ou seja, as chamadas de sistema só são acessadas pelo programa em tempo de execução.

Existe uma API em C para que possamos executar syscalls em "alto nível", essa API/Interface existe na forma de uma wrapper function que irá executar a chamada de sistema efetivamente, pegando os parâmetros que você passou na função, colocando-os nos registradores necessários e posteriormente executando um código de baixo nível para executar a syscall de fato.

Essas wrapper functions são as funções normais de C.

#### Acesso / Desvio do SO
- instrução INT: **Salva o contexto e faz um JUMP para o endereço correspondente ao número passado como parâmetro no vetor de interrupções.**
- instrução SYSCALL

 A diferença das duas formas de entrada nas syscalls é uma pequena diferença no ponto de entrada que cada instrução utiliza.

OBS: A troca do userspace para o kernelspace é sempre muito custosa.

Vale ressaltar que o SO mantém informações que não representam uma ameaça ao bom funcionamento do hardware e dos programas em locais mapeados pelo próprio programa, para que assim a syscall não precise efetuar uma troca de contexto completa, e sim apenas uma leitura simples de dados.


