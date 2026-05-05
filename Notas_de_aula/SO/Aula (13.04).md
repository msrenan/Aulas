# Retomando IPC
- Estruturas e mecanismos
	- **Semáforo:**
		- quem tranca não precisa ser quem destranca
		- mais genérico, pode bloquear tarefas não só para o acesso compartilhado de recursos
		- contador -> Pode representar número de itens produzidos, exclusão mutua, espaço no buffer e etc;
		- fila;
		- sem_init();
		- sem_wait() (~lock) -> Se o contador é maior que zero, decrementa ele, senão insere o processo corrente na fila do semáforo e chama o escalonador;
		- sem_post() (~unlock) -> Se a fila não estiver vazia, acorda a primeira tarefa da fila do semáforo, senão, incrementa o contador;
	- **Mutex:**
		- quem tranca DEVE ser quem destranca
		- contador (binário);
		- file;
		- lock() -> Se o recurso estiver liberado, contador = 0, senão, adiciona a tarefa na fila do mutex e chama o escalonador;
		- unlock() -> Se tiver tarefa na fila do mutex, acorda ela, senão, contador = 1;


## Monitores

Pensado para garantir que não vá haver erro no uso de semáforo/mutex, como o não uso.

```c++
**Monitor** monitor_do_buffer   
{  
  **public** {  
    void insere(int item);  
    void retira(int *item);  
  }  
  
  **private** {  
  
    #define TAM 128  
  
    int buffer[TAM];  
    int in, out;  
  
    sem_t s_espaço, s_item;   
    pthread_mutex_t mutex;  
  
    void _init()  
    {   
      in = out = 0;  
      pthread_mutex_init(&mutex, NULL);  
      sem_init(&s_espaço, TAM, 0);  
      sem_init(&s_item, 0,0);  
      ...  
    }   
  
    void **insere**(int item)   
    {   
       sem_wait(&sem_espaço);  
       pthread_mutex_lock(&_mutex);  
       
       buffer[in] = item;  
       in = (in+1) % TAM;  
  
       pthread_mutex_unlock(&_mutex);  
       sem_post(&sem_item);  
     }  
  
     void **retira**(int *item)  
     {  
       ...  
     }   
}
```

## Barreira
A barreira só deixa os processos acessarem o recurso quando TODOS os processos tiverem chamado a barreira, independente de quando cada processo chamou a barreira.

Possuí um contador e uma fila. O contador é iniciado com o número total de processos. **barrier_wait** verifica se o contador é 0, se for 0, acorda todos os processos, senão, bloqueia a tarefa.

É uma estrutura que serve para garantir que vários processos chegaram em um mesmo ponto do código, antes de prosseguir.

# Problemas de IPC
- Problemas clássicos:
	- Produtor / Consumidor
	- Leitores e escritores
	- Jantar dos filósofos
	- Barbeiro dorminhoco

## Produtor / Consumidor
Representa uma classe de problemas que x tarefas vão produzir algo e y tarefas vão consumir o que os produtores consumirem. 

No caso mais simples há apenas 1 produtor e 1 consumidor. O produtor está incialmente ativo e o consumidor está aguardando a chegada do produto desejado. Se o consumidor está consumindo um produto, o produtor não tem onde colocar um novo produto. Solução **Buffer**.

- Se o consumidor demorar para consumir o produto, o produtor vai enchendo o buffer.
- Se o produtor parar de produzir por um tempo X, o consumidor ainda tem os produtos remanescentes no buffer para consumir.
- Assim, o fluxo de produção e consumo nunca para.

Para garantir que o consumidor não irá consumir algo enquanto o produtor está colocando um novo produto no buffer, usamos semáforos e mutexes.

#### Código do produtor:

```c
do {
  produz_item();         // função associada à geração ou ao recebimento dos dados para processamento
  sem_wait (sema_espaços); // Se não tem espeço no buffer, espera.
  mutex_lock(mutex_inserção);
  insere_item();
  mutex_unlock(mutex_inserção);
  sem_post (sema_items); //Se tinha algum consumidor esperando um produto no buffer, acorda ele.
} while (!fim);
```

#### Código do consumidor:
```c
do {
  sem_wait (sema_itens); // Se não tem item pra consumir, espera.
  mutex_lock(mutex_retirada);;
  retira_item();
  mutex_unlock(mutex_retirada);
  sem_post (sema_espaços); //Se tinha algum produtor esperando abrir um espaço no buffer para produzir, acorda ele.
  consome_item();           // função de processamento das requisições ou dados recebidos
} while (!fim);
```


## Leitores e Escritores
Se alguns escritores escrevem algo que muitos leitores precisam, isso se torna um problema. Primeiro, pois cada escritor deve escrever sua parte uma de cada vez, e cada leitor deve esperar que os escritores terminem de escrever para ler, senão cairemos em uma race conditions. Sendo assim, um escritor impede a ação de outros escritores e todos os leitores. **Solução:** todos os leitores podem ler ao mesmo tempo, mas se um escritor precisar escrever, assim que os leitores acabarem suas regiões críticas, o escritor bloqueia a todos.

## Jantar de filósofos
![[Pasted image 20260413092148.png]]
Para comer, cada filósofo precisa de 2 garfos. Como garantir que se todos os filosofos vão ser capaz de comer?
```c
do {
  pensar por um tempo aleatório
  pegar um garfo (qual?)
  pegar o outro garfo
  comer por um tempo aleatório
  liberar um garfo (qual?)
  liberar o outro garfo
  [se for preciso, sai um pouquinho mas volta logo :-]
} while(1);
```

Essa situação exige alguns bloqueios e estratégias.
- E se alguém pegar um garfo e não conseguir pegar o outro?
- E se alguém nunca conseguir pegar 2 garfos?
**Solução:** Usar operações não bloquantes (mutex + try_lock())

Este é um problema que há dependência em múltiplos recursos. Se todos deram lock no garfo a direita, eles TENTAM dar lock no da esquerda. Se der certo, o filosofo come. Se der errado, ele solta o garfo da direita também e espera e tenta denovo depois.

# Impasses: deadlocks
Se todos os filósofos pegassem cegamente o garfo a direita e depois tentassem, sem try_lock(), pegar o garfo a esquerda, todos os filósofos esperariam infinitamente pelo recurso. Isso é um deadlock.
Ou então, se todos os filosofos pegam o garfo a direita, falham em pegar o da esquerda e soltam o da direita. Esperam denovo e tentam novamente. Nesse cenário, os filosofos iriam ficar infinitamente pegando o garfo e soltando ele depois. Isso é um livelock.
Ou então, se os filósofos agora pegam os garfos com base na velocidade, pode ser que um deles nunca consiga pegar 2 garfos. Isso é um starvation.

Situações críticas já envolvendo semáforos e mutexes ocorrem sempre quando há dependência de múltiplos recursos.

**A ordem de bloqueio e liberação importa muito:**

```c
(a)       // Uso correto, sem deadlock.                          
   void tarefa_**A**() {         void tarefa_**B**() {
     sem_wait(s**1**);              sem_wait(s**1**);
     sem_wait(s**2**);              sem_wait(s**2**);
     usa_recursos();            usa_recursos();
     sem_post(s**2**);              sem_post(s**2**);
     sem_post(s**1**);              sem_post(s**1**);
  }                          }
```

```c
(b)   // Uso errado, com deadlock.
void tarefa_**A**() {            void tarefa_**B**() {
   **sem_wait(s1);                  sem_wait(s2);
   sem_wait(s2);                    sem_wait(s1);**
   usa_recursos();                  usa_recursos();
   sem_post(s**2**);                sem_post(s**1**);
   sem_post(s**1**);                sem_post(s**2**);
}                                }
```

#### Algumas condições são necessárias para ocorrência de impasses:

1. Uso de recursos compartilhados em exclusão mútua
2. Tarefas que já bloquearam recursos podem necessitar bloquear outros recursos
3. Recursos bloqueados não podem ser retirados das tarefas que os alocaram (não-preempção)
4. Há uma dependência circular entre as tarefas e os recursos bloqueados e necessários

#### Ao lidar com impasses, é possível:
- Ignorar e torcer para que não ocorram
- Tentar detectar e tratar
- Evitar, com a alocação cuidadosa de recursos
- Prevenir, negando a ocorrência de alguma da 4 condições para impasse mostradas anteriormente (melhor saída)