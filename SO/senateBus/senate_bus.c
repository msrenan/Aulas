
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
continuar esperando na estação, e o ônibus sai em sua rota.

Usar mutexes para acesso em partes de memória compartilhada.

*/

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define NEW_PASSENGER_ARRIVED() (rand() % 2)

