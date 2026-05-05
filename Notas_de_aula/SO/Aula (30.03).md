
# Threads
"Linhas de execução que compartilham as áreas de memória do task_struct."

Uma thread é uma **linha de execução** que permite partes de um código sejam executadas mesmo enquanto existe uma operção que ainda está aguardando uma condição. Essa linha de execução normal mente está associada a um código dentro de um programa, ou seja: todas as threads compartilham uma mesma área de dados.

As threads estão associadas a um processo e podem ser criadas a partir de uma função. A primeira API de threads foi criada totalmente em UserSpace, mas era ineficiente demais e foi incorporada, com alterações, em KernelSpace posteriormente.

> Sinais, tratamentos de sinais e mapeamento de memória são elementos do task_struct que são compartilhados entre as _threads_.

Algumas funções não podem ser executadas simultâneamentes por diversas threads, essas funções são chamadas de não-reentrantes.

#### Diagrama do funcionamento de uma thread
```
+-----------------------------------------------------------------------+   
| Process                                                               |   
|   +-------+     +-------------+  +-------------+  +-------------+     |   
|   | Files |     | Thread      |  | Thread      |  | Thread      |     |   
|   +-------+     |+-----------+|  |+-----------+|  |+-----------+|     |   
|                 || Registers ||  || Registers ||  || Registers ||     |   
|                 |+-----------+|  |+-----------+|  |+-----------+|     |   
| ....................................................................  |   
| . Memory        |             |  |             |  |             |  .  |   
| .               | +---------+ |  | +---------+ |  | +---------+ |  .  |   
| . +--------+    | |  Stack  | |  | |  Stack  | |  | |  Stack  | |  .  |   
| . |  Heap  |    | |         | |  | |         | |  | |         | |  .  |   
| . +--------+    | |         | |  | |         | |  | |         | |  .  |   
| .               | |         | |  | |         | |  | |         | |  .  |   
| . +--------+    | |         | |  | |         | |  | |         | |  .  |   
| . | Static |    | |         | |  | |         | |  | |         | |  .  |   
| . +--------+    | |         | |  | |         | |  | |         | |  .  |   
| .               | |         | |  | |         | |  | |         | |  .  |   
| . +--------+    | |         | |  | |         | |  | |         | |  .  |   
| . |  Code  |    | +---------+ |  | +---------+ |  | +---------+ |  .  |   
| . +--------+    +-------------+  +-------------+  +-------------+  .  |   
| ....................................................................  |   
+-----------------------------------------------------------------------+
```


- Cada thread armazena uma área de contexto próprio, para poder salvar o estado de sua execução em alguma eventual interrupção.
- Cada thread também tem uma área de pilha própria para cada uma, para que elas não quebrem as suas informações utilizando uma mesma pilha conjunta.

### Modelos de programação com threads

- **Mestre/Escravo** -> a main é comumente usada para criar e para esperar pela conclusão de outra threads do processo. A thread master é sempre responsável pela sincronização, sem efetivamente tomar parte na execução do código paralelo.
- **Pipeline** -> as atividades que devem ser executadas são quebradas em suboperações que são executadas em série, mas de maneira conccorrente, por threads diferentes. Os dados manipulados por uma thread são tipicamente encaminhados para a próxima thread depois de serem processaods. A manutenção da estrutura de dados manipulada pelas threads numa área de memória global facilita as comunicações entre elas.
- **Peer** -> Semelhante ao primeiro modelo, mas depois de criar os escravos, a thread principal participa na execução do trabalho.

## Implementação das threads

No início elas eram em UserSpace, mas elas eram ineficientes, e dividiam a fatia de tempo de um único processo.

Eventualmente, esse conceito foi trazido para o kernel, gerando muito mais eficiência e potencial para os códigos. Isso foi feito através da _syscall_ _**clone**_:

```c
 int **clone**(int (*fn)(void *), void *stack, int flags, void *arg, ...
           /* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
```

As threads possuem flags para modificar um pouco seu funcionamento, dependendo da necessidade do SO.

