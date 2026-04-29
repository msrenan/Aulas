#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// Definindo macro para garantir compatibilidade com barreiras no Linux
#define _POSIX_C_SOURCE 200112L

pthread_barrier_t barreira_embarque;
sem_t sem_pista_livre;

void* rotina_torre(void* arg) {
    while(1) {
        // A torre espera o sinal do último passageiro de QUALQUER voo
        sem_wait(&sem_pista_livre);
        printf("[TORRE] 📡 Sinal de embarque completo recebido! Pista liberada para decolagem.\n");
        printf("[PILOTO] ✈️  VOO EM CURSO... (subindo para altitude de cruzeiro)\n");
        sleep(1); 
    }
    return NULL;
}

void* rotina_passageiro(void* arg) {
    int id = *(int*)arg;
    
    printf("  [Passageiro %d] Entrando no avião...\n", id);
    
    int status = pthread_barrier_wait(&barreira_embarque);

    // O ÚLTIMO passageiro de cada voo sinaliza a torre
    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("\n[COMISSÁRIO] Voo lotado! Avisando a torre agora...\n");
        sem_post(&sem_pista_livre);
    }

    printf("O passageiro %d está embarcado.\n", id);

    return NULL;
}

void realizar_voo(int id_voo, int qtd) {
    pthread_t passageiros[qtd];
    int ids[qtd];

    printf("\n--- 🛫 PREPARANDO VOO Gemini-%03d (%d passageiros) ---\n", id_voo, qtd);

    // REINICIALIZAÇÃO: Barreira criada com o tamanho exato deste grupo
    pthread_barrier_init(&barreira_embarque, NULL, qtd);

    for (int i = 0; i < qtd; i++) {
        ids[i] = i + 1;
        pthread_create(&passageiros[i], NULL, rotina_passageiro, &ids[i]);
    }

    // Espera todos embarcarem e a thread de cada um encerrar
    for (int i = 0; i < qtd; i++) {
        pthread_join(passageiros[i], NULL);
    }

    // LIMPEZA: Destrói para que o próximo voo possa ter um número diferente
    pthread_barrier_destroy(&barreira_embarque);
    printf("--- ✅ VOO Gemini-%03d CHEGOU AO DESTINO ---\n", id_voo);
}

int main() {
    pthread_t t_torre;
    srand(time(NULL));

    sem_init(&sem_pista_livre, 0, 0);

    // A torre fica rodando em background (daemon)
    pthread_create(&t_torre, NULL, rotina_torre, NULL);
    pthread_detach(t_torre);

    // Simulando 3 decolagens com números aleatórios de passageiros
    for (int v = 1; v <= 3; v++) {
        int passageiros_hoje = (rand() % 4) + 2; // Entre 2 e 5 passageiros
        realizar_voo(v, passageiros_hoje);
        sleep(2); // Intervalo entre voos
    }

    printf("\n[SISTEMA] Aeroporto fechado. Sem mais voos hoje.\n");
    sem_destroy(&sem_pista_livre);
    
    return 0;
}