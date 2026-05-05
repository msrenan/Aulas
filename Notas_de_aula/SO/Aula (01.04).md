# Escalonamento de tarefas

O escalonamento de tarefas ocorre (task scheduling) quando uma tarefa pronta precisa entrar em execução. Primeiro, o uma tarefa sai de execução, por qualquer motivo que seja, e então passa para o estado de bloqueada ou volta para o estado de pronta. Em seguida, entra o **scheduler**, que irá selecionar qual tarefa é a melhor para ser executada.

O escalonador é acionado a partir da função `schedule()` , ela sempre retorna algo, ou seja, sempre que ela for chamada, deve existir uma tarefa para ser executada. O escalonamento funciona com base na analise e comparção de alguns aspectos:
- Prioridade
- Proximidade
- Prazo de conclusão

### Política de escalonamento
Tudo o que for uma TASK entra na fila de escalonamento, seja uma thread ou um processo comum. Todas as tasks são avaliadas em critérios para saber qual é a task que será escalada para entrar em execução:
- **throughput**: Se o processador precisar aumentar o número de processos por um período de tempo que ele está processando, ele prioriza tarefas menores.
- **Latência**: Se um programa submete um processo e espera um resultado, o processador pode priorizar o processo com a menor latência entre a submissão e a resposta.
- **Fairness**: Prioriza um uso balanceado do processador, evitando que tarefas tenham suas execuções preteridas (deferred) indefinidamente, como time sharing (cada processo recebe uma fatia de tempo, em cada roda vários processos são executados, por 15 segundos, por exemplo), ou então atribuindo uma fatia justa de uso de processador que cada processo utiliza.
- **Prazos**: O SO precisa garantir que a execução de tarefas críticas ocorrem dentro de prazos previstos.
- **Maximização da ocupação de recursos**
- (entre outros...)

Existem também algumas politicas de escalonamento:
- **FIFO**: First in, First Out, a tarefa é executada até acabar ou ser bloqueada.
- **Shortest First**: Task mais curta primeiro, é necessário que a lista de tarefas seja completamente conhecida.
- **Shortest Remaining Time**: Task com o menor tempo restante primeiro, também requer que a lista de tarefas seja previamente conhecida. É uma política preemptiva, ou seja, o processador pode comparar a tarefa atual com tarefas prontas para decidir como escalonar.
- **Chaveamento Circular (Round Robin)**: A seleção de tarefas é feita com base em fatias de tempo, distribuindo uma mesma fatia de tempo para todas as tasks prontas, e executando elas apenas durante essa fatia de tempo. Sendo um tipo de política justa.
- **Escalonamento por prioridades**: Tarefas com maior prioridades são executadas primeiro.
- **Filas Múltiplas**: Essa política utiliza várias filas que podem ou não receber tarefas, cada uma com um possível valor de prioridade. O escalonador deve selecionar uma tarefa pronta da fila de maior prioridade que possua tarefas. Dentro dessa fila, as tarefas podem ser alternadas da mesma forma que o round-robin. Isso implica que as tarefas que são mais curtas, são finalizadas primeiro durante as rodadas das fatias de tempo.
- **Escalonamento garantido**: A ideia é prover uma fração igualitária do uso dos processadores para cada tarefa. Para tanto, é preciso saber quantas tarefas existe, qual o tempo de vida de cada task e quanto tempo de CPU cada tarefa já recebeu.
-