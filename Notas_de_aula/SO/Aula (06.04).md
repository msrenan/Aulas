# IPC
**_Inter-process comunnication_**

## O problema produtor/consumidor
"Há um produtor que produz algo X, mas não necessáriamente ele vai tratar X. Ele encaminha X diretamente para o consumidor final. Enquanto isso, há um consumidor que aguarda um item X, e que processa esse item no lugar do produtor. Podem ser mais de um produtor e consumidor, seus números podem variar. Imagine que exista um buffer circular para o uso tanto do produtor quanto do consumidor. Imagine, agora, que o produtor colocou X em uma posição do buffer para que o consumidor possa pegar sem que ele precise ficar esperando o consumidor ir buscar X. Agora imagine se houvessem dois consumidores? Como garantir que dois consumidores não irão buscar o mesmo X?"

No mundo dos processos, existe um problema parecido, em que 2 processos podem tentar acessar recursos compartilhados simultâneamente (o que quebraria, obrigatóriamente pelo menos 1 dos processos).

A comunicação entre processos serve para passagem de informação, controle de gerenciamento de tarefas e sincronização de acesso à áreas de memória compartilhadas.

## Comunicação entre 2 processos.
Suponha 2 processos P1 e P2, com duas áreas de memória, separadas, mapeadas. Como fazê-los se comunicar? 

Se um processo P3, com sua área de memória mapeada, possuí 3 threads, as threads já nascem intercomunicadas, pois as threads teriam acesso a uma "área de memória global" dentro da área de memória de P3. Ou seja, comunicação inter-threads é nativamente mais fácil de ser implementada.

Já com a comunicação inter-processos, onde as áreas de memória não se intersectam, o SO precisa ajudar na integração entre P1 e P2, utilizando de mecanismo e estruturas de comunicação/sincronização para realizar a IPC.

## Programação Concorrente
É uma programação que envolve múltiplas tarefas. É aquela que há uma idependência entre as atividades, e usamos isso para explorar a existência de múltiplos processadores de uma mesma máquina, em que as multiplas tarefas disputam recursos de uma mesma máquina.

Na programação concorrente, um dos grandes problemas está relacionado à sincronização entre processos para acesso de recursos. A má sincronização de processos pode resultar em **deadlocks** (quando 2 processos que esperam por um mesmo dado ficam esperando infinitamente, sem condição de saída, ocorrendo, por exemplo, quando outro processo utiliza o recurso que eles estavam esperando errôneamente, fazendo com que os 2 processos fiquem presos em um loop.) ou **livelocks** (semelhante ao deadlock, mas um processo tira a informação do outro para prosseguir, prendendo ambos em um loop de "roubo de dados").

> A esses problemas de sincronização, dá-se o nome de **race-conditions**.

Uma possível solução para as race-conditions é a **exclusão mútua**: um mecanismo que deve **garantir** que apenas um processo esteja acessando o recurso por vez.

Em uma tarefa, o trecho de seu código que envolve a manipulação de algum recurso compartilhado é conhecido como **critical section** (região crítica). Esse trecho de código, deve sofrer tratamento de race-conditions, e esse tratamento deve durar o **menor tempo possível**, já que enquanto um processo está na critical section, outros processos ficam bloqueados até que o processo em questão libere o recurso compartilhado para que os processos bloqueados possam acessá-la também.

## Estratégias para exclusão mútua
```
           Tempo --->
Processo A ----------|======================|----------------------  
                     .                      .
                     . A: na região crítica .
                     .                      .
Processo B -----------------|xxxxxxxxxxxxxxx|==============|-------  
                     .      .               .              .
                     .      . B: bloqueado  .              .  
                     .      .               . B: na região . 
                     .      .               .    crítica   .
                    (1) A entra na RC       .              .
                            .               .              .
                           (2) B tenta entrar na RC e é bloqueado
                                            .              .
                                           (3) A libera a RC; B é desbloqueado e                                                entra na RC  
                                                           .  
                                                          (4) B sai da RC,                                                              deixando o recurso livre
```
 Olhando esse diagrama, fica claro que provavelmente o SO fornecerá algum serviço para testar se a algum processo está em sua região crítica para bloquear outros processos que precisam do mesmo recurso.

Se vários processos ficam bloqueados enquanto tentam acessar algum recurso compartilhado, usualmente, eles são desbloqueados por "ordem de chegada" na fila dos processos bloqueados. Contudo, podem existir mecanismos de gerenciamento de race conditions que levam em conta alguma prioridade na hora de definir a ordem de desbloqueio dos processos. Usualmente, por padrão, leva-se em conta a ordem de chegada mesmo.

- Um dos primeiros métodos para implementar uma solução para as race conditions era o uso de uma variável global a todos os processos. Os processos entravam em um `ẁhile (lock == 1);` enquanto lock estivesse ativo, e quando não estivesse ativo, eles saiam do loop e executavam a região crítica. Contudo essa estratégia apresentava um problema, se houvesse uma interrupção entre a saída do loop (que é imediatemente seguida por um `lock = 1`), antes de setar o lock para 1, outro processo poderia entrar na região critica e acessar a informação. Supondo que este segundo processo seja interrompido no meio da sua região crítica, e o primeiro processo volte a execução, depois de já ter saído do loop, ocorrerá uma race condition.

Como impedir que uma interrupção estrague toda a lógica de bloqueio?
- Um método para coordenar as instruções com a variável global lock é a instrução **TSL**, essa instrução torna possível a busca e a alteração de uma variável em um único ciclo de clock. Ou seja, no exemplo acima, o primeiro processo seria interrompido depois de ter trocado o lock para 1, pois a instrução de verificação e alteração da variável lock ocorreria no mesmo ciclo. Logo, o segundo processo nunca teria passado pelo loop enquanto o primeiro estava interrompido.

Mas ainda assim, surge um problema de otimização: Será que não há como dar um uso melhor para o processo do que apenas deixar ele preso em um while loop que não faz nada além de verificar o valor de lock? O que fazer com essa tarefa que não pode entrar em sua região crítica enquanto está bloqueada?
- A solução idealizada foi utilizar um **sleep/wakeup**, se o lock estiver ativo, a função `sleep()` era chamada e o processo dormia até que uma condição fosse satisfeita (e não por um período de tempo). Quando a condição era cumprida, acontecia um `wakeup()` que acordava a thread de volta ao loop de verificação do lock.
```c
 /* Processo A */             /* Processo B */                 
 1.   ...                          ...
 2.   extern int lock=0;           extern int lock=0;
 3.   ...                          ...                             
 4.   while (cond) {               while (outra_cond) { 
 5.     ...                           ...               
 6.     if (lock == 0)                if (lock == 0)  
 7.       lock = 1;                     lock = 1;  
 8.     else                          else
 9.       sleep();                      sleep();
            
        região_crítica();         outra_região_crítica();
1.     if (sleeping(B))              if (sleeping(A))  
2.       wakeup(B);                    wakeup(A);  
3.     else                          else
   4.       lock = 0;                    lock = 0; 
5.    ...                            ...
6.   }
```

Mas essa estratégia também tem um problema grave, existe a chance de A liberar o recurso e B ficar dormindo eternamente.

## Semáforo
As soluções para race conditions pareciam não chegar em um acerto total, até que Djikstra em 1963 chega com a ideia do **semáforo**.

O semáforo e uma estrutura que depende da ajuda do SO para gerenciar mutual exclusion, fazendo isso por meio das funções lock e unlock.

```
// lock()
se (semáforo.contador > 0)
  semáforo.contador --
senão {
  salva estado da tarefa chamadora
  insere descritor desta tarefa na fila do semáforo (é um auto-bloqueio!)
  ativa escalonador (para selecionar outra tarefa e colocá-la em execução)
}
```

```
// unlock()
se (semáforo.fila != NULL)     // há tarefa bloqueada na fila
   acorda tarefa da fila       // recoloca tarefa anteriormente bloqueada na                                       fila de prontos
   chama escalonador           // é preciso decidir qual tarefa será colocada em                                   execução
senão                          // não há tarefa(s) na fila
   semáforo.contador ++        // apenas indica que recurso agora está livre
```

Isso resolveu o problema das race conditions e ainda otimizou o uso do processador, pois quando uma tarefa entra na região crítica e é bloqueada, o semáforo chama o escalonado e tira a tarefa de execução, permitindo que outras tarefas sejam executadas. Quando o processo que estava executando a região crítica termina a manipulação dos recursos compartilhados, o semáforo acorda a próxima tarefa na fila das tarefas bloqueadas e chama o escalonador para decidir se ela é a próxima tarefa a ser executada ou se outra tarefa será executada em seu lugar.

O semáforo utiliza um contador para saber se há tarefas bloqueadas na fila aguardando liberação dos recursos compartilhados para prosseguirem para a execução de suas áreas críticas.

O semáforo pode ser utilizado para verificação de recursos diversos e não só para manipulação de regiões compartilhadas de memória.

## Mutex
Um mutex funciona de maneira semelhante ao semáforo. Uma diferença é que enquanto o semáforo faz o controle com base no contador do semáforo, o mutex guarda o estado de liberação da região compartilhada de memória. Em resumo: **O semáforo utiliza um contador que pode indicar quantos processos estão em espera, quanto espaço ainda há na fila, etc. o valor inicial do semáforo depende do que valor que você leva em consideração para o controle do semáforo. Já o Mutex utiliza 0 se a região compartilhada de memória estiver sendo acessada e 1 se ela estiver liberada para que algum processo entre em sua região crítica."

```
mutex_lock:
    if (mutex.contador == 1)        // operações devem ocorrer
       mutex.contador = 0           // de maneira atômica
   else {
       salva contexto da tarefa atual // o recurso já está ocupado, então o processo deve ser bloqueado.
       bloqueia esta tarefa na fila do mutex
       chama escalonador
   }

mutex_unlock:
   if (há tarefa na fila do mutex) {
      acorda tarefa
      chama escalonador
   } else
       mutex.contador=1 // recurso está disponível
```

Ambos o semáforo e o mutex verificam se a área compartilhada de dados está sendo utilizada, mas enquanto o contador do mutex reflete o uso ou não dessa área compartilhada, o do semáforo indica quantos recursos estão disponíveis/ já foram utilizados. Outra diferença é que o semáforo pode ser usado para verificar uma estrutura X e manipular uma estrutura Y, enquanto mutex deve ser utilizado tanto para verificar e manipular uma mesma estrutura X.