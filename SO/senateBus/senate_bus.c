
/*

Passageiros chegam em um ponto de ônibus e esperam por um ônibus. Quando o busão chega
todos os passageiros no ponto de ônibus tentam entrar no ônibus, mas todos que chegarem enquanto o ônibus
está recebendo passageiros tem que esperar pelo próximo ônibus. O ônibus tem capacidade máxima de 
50 pessoas. Se não tiver ninguém no ponto de ônibus, o ônibus deve sair imediatamente. Se tiverem
mais pessoas esperando do que a capacidade do ônibus, os sobressalentes devem esperar o próximo ônibus.

Precisamos de threads para cada um dos passageiros, para o ônibus e para a estação.

Cada thread de passageiro é criada quando o passageiro chega na estação. Logo, todas as threads
de passageiros nascem esperando o ônibus (e já nascem na estação). Usar um semáforo para bloquear as threads
enquanto não tiver ônibus para elas embarcarem. Usar outro semáforo para bloquear as threads dos passageiros
que chegaram enquanto o ônibus estava sendo preenchido.

Quando o ônibus chega na estação, as threads dos passageiros devem ser acordadas e devem embarcar no ônibus.
Usar um semáforo para indicar que o ônibus está sendo preenchido. Usar outro semáforo que indica a capacidade
atual do ônibus. Se a capacidade do ônibus for totalmente preenchida, os passageiros sobressalentes devem 
continuar esperando na estação, e o ônibus sai em sua rota. Usar outro semáforo que indica se o ônibus está
pronto para sair, seja porque todos que estavam na estação embarcaram, ou porque lotou sua capacidade máxima.

Usar mutexes para acesso em partes de memória compartilhada.

*/
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>

//Gera um sinal que é utilizado para definir quando um novo passageiro chega na rodoviária
#define NEW_PASSENGER_ARRIVED() (rand() % 2)
//Gera os IDs de cada passageiro
#define GEN_ID() (_count + 1) 
//Gera um tempo aleatório pertencente a [5, 15]
#define RANDOM_INITIAL_TIME() ((rand() % 15) + 5)

//Constante pra definir quantos passageiros totais circularam no sistema.
#define MAX_PASS 500

//Mutex para exclusão mutua
pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;

//Barreira para controlar o embarque
pthread_barrier_t _board_barrier;

//Contador para limitar a quantidade máxima de passageiros e funcionamento de algumas threads
int _count;
//Mantém a contagem de quantos passageiros estão esperando na estação.
int _station;

//Armazena todas as threads dos passageiros
pthread_t _threads[500];

//Representa o ônibus que armazena os IDs de cada passageiro embarcado.
int _bus[50];

//Semáforos para controle de embarque e para controle da saída do ônibus
sem_t _board, _bus_ready;

time_t _starting_time;

struct tm* get_current_time() {
    time_t t = time(NULL);

    time_t elapsed = t - _starting_time;

    return gmtime(&elapsed);
}

//Função que embarca passageiros no ônibus. Só é acessada quando o ônibus está na estação e possuí
//assentos livres para o embarque
void board(int id, int place) {
    struct tm *curr = get_current_time();
    printf("\033[35m[%02d:%02d:%02d - PASSAGEIRO %d] Embarcou no ônibus, no assento %d!\033[0m\n",curr->tm_hour, curr->tm_min, curr->tm_sec, id, place);
    //Tranca o mutex para garantir segurança na manipulação do ônibus que é uma variável
    // comum a todas as threads.
    pthread_mutex_lock(&_mutex);
    _bus[place] = id;
    _station--;
    pthread_mutex_unlock(&_mutex);
    //Trava as threads de passageiro até que todos os passageiros que estavam na estação quando o ônibus
    // chegou embarquem.
    int status = pthread_barrier_wait(&_board_barrier);
    //O último passageiro a embarcar gera o sinal que libera o ônibus para sair da estação.
    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
        sleep(1);
        sem_post(&_bus_ready);
    }
}

//Rotina das threads Passageiro, espera o ônibus chegar na estação e então tenta embarcar.
void * passenger(void * arg) {
    int id = *(int *) arg;
    struct tm *curr = get_current_time();
    printf("\033[34m[%02d:%02d:%02d - PASSAGEIRO %d] Está esperando...\033[0m\n",curr->tm_hour, curr->tm_min, curr->tm_sec, id);
    int restante;
    //Trava cada thread passageiro até que haja espaço no ônibus para elas.
    sem_wait(&_board);
    //Se tem espaço, a thread embarca no ônibus imediatamente.
    board(id, id % 50);
    pthread_exit(NULL);
}

//Função que o ônibus executa sempre que está pronto para sair da estação em sua rota, só é executada
//quando todos os passageiros que estavam na estação no momento da chegada do ônibus embarcam, ou
//imediatamente se não havia ninguém na estação.
void depart(int qtd) {
    struct tm *curr = get_current_time();
    printf("\033[33m[%02d:%02d:%02d - ÔNIBUS] Saiu da estação, com %d passageiros!\033[0m\n",curr->tm_hour, curr->tm_min, curr->tm_sec, qtd);
    sleep(RANDOM_INITIAL_TIME());
    int i = 0, count = 0;
    while (count < qtd) {

        //Adaptação para ler sempre assentos ocupados.
        if (_bus[i] == 0) {
            i++;
            continue;
        }

        //Trava o mutex para manipular o ônibus sem problemas.
        pthread_mutex_lock(&_mutex);
        struct tm *curr = get_current_time();
        printf("\033[1;33m[%02d:%02d:%02d - ÔNIBUS] O passageiro %d desceu do ônibus\033[0m\n",curr->tm_hour, curr->tm_min, curr->tm_sec, _bus[i]);
        _bus[i] = 0;
        pthread_mutex_unlock(&_mutex);
        i++;
        count++;
        sleep(1);
    }

    //Se o ônibus não estiver vazio, destrói a barreira para reinicialização
    // caso contrário, ela não foi inicializada.
    if (qtd > 0) {
        pthread_mutex_lock(&_mutex);
        pthread_barrier_destroy(&_board_barrier);
        pthread_mutex_unlock(&_mutex);
    }
    
}

//Rotina da thread do ônibus, se tiver passageiros na estação no momento em que ele chegar,
//espera todos embarcarem e então sai, senão sai imediatamente.
void * bus(void * args) {
    int qtd;
    while (_count < MAX_PASS || _station > 0) {
        struct tm *curr = get_current_time();
        printf("\033[32m[%02d:%02d:%02d - ÔNIBUS] Chegou na estação...\033[0m\n", curr->tm_hour, curr->tm_min, curr->tm_sec);
        //Se tiver pessoas na estação, garante que somente essa quantidade de pessoas entre no ônibus
        // já que o problema especifica que só quem está na estação no momento de chegada do ônibus
        // pode embarcar nele.
        if (_station > 0) {
            struct tm *curr = get_current_time();
            printf("\033[1;31m[%02d:%02d:%02d - ESTAÇÃO] Tem %d passageiros esperando na estação!\033[0m\n",curr->tm_hour, curr->tm_min, curr->tm_sec, _station);
            //Variável que será usada para definir quantos passageiros devem sincronizar na barreira.
            // Se tem menos de 50 pessoas na estção, somente elas devem entrar no ônibus.
            // Se tem mais de 50 pessoas, só as 50 primeiras entram e o resto espera o próximo busão.
            qtd = (_station >= 50 ? 50 : _station);
            //Trava o mutex para garantir manipulação segura da barreira e para reiniciar
            // o semáforo que contabiliza quantas pessoas devem embarcar no ônibus
            // sem problemas.
            pthread_mutex_lock(&_mutex);
            pthread_barrier_init(&_board_barrier, NULL, qtd);
            for (int i = 0; i < qtd; i++) {
                sem_post(&_board);
            }
            pthread_mutex_unlock(&_mutex);
        } else {
            qtd = 0;
            sem_post(&_bus_ready);
            struct tm *curr = get_current_time();
            printf("\033[1;31m[%02d:%02d:%02d - ESTAÇÃO] Não havia ninguém lá...\033[0m\n", curr->tm_hour, curr->tm_min, curr->tm_sec);
        }
        //Trava a thread até que o ônibus esteja pronto para seguir sua rota, por qualquer motivo que
        // seja (estação vazia ou todos os passageiros embarcaram).
        sem_wait(&_bus_ready);
        depart(qtd);
    }
    pthread_exit(NULL);
}

//Função principal, thread principal, representando a própria estação, onde os passageiros que estão
//a espera do ônibus e o próprio ônibus se encontram.
int main() {
    //Inicialização do timer
    _starting_time = time(NULL);
    //Inicialização de variáveis de controle populacional
    _count = 0;
    _station = 0;

    //Inicialização dos semáforos
    sem_init(&_board, 0, 0);
    sem_init(&_bus_ready, 0, 1);

    //Inicialização da thread do ônibus.
    pthread_t bus_t;
    pthread_create(&bus_t, NULL, bus, NULL);

    //Loop principal de execução da estação.
    int pass_id;
    struct tm *curr = get_current_time();
    printf("\033[1;31m[%02d:%02d:%02d - ESTAÇÃO] Mais um dia começa na estação!\033[0m\n", curr->tm_hour, curr->tm_min, curr->tm_sec);
    while (_count < MAX_PASS) {
        if (NEW_PASSENGER_ARRIVED() || 1) {
            pass_id = GEN_ID();

            pthread_create(&_threads[_count], NULL, passenger, &pass_id);

            pthread_mutex_lock(&_mutex);
            _count++;
            _station++;
            pthread_mutex_unlock(&_mutex);
        }
        sleep(1);
    }
    curr = get_current_time();
    printf("\033[1;31m[%02d:%02d:%02d - ESTAÇÃO]É o fim do dia na estação.\033[0m\n", curr->tm_hour, curr->tm_min, curr->tm_sec);

    pthread_join(bus_t, NULL);

    curr = get_current_time();
    printf("\033[32m[%02d:%02d:%02d - ÔNIBUS] Terminou o último percurso\033[0m\n", curr->tm_hour, curr->tm_min, curr->tm_sec);


    sem_destroy(&_board);
    sem_destroy(&_bus_ready);
}
