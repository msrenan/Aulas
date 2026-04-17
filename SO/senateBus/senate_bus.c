#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct Rider {
    int id;
} typedef Rider;

struct Bus {
    Rider riders[50];
    int total_riders;
    int at_station;
} typedef Bus;

struct BoardBusThreadArgs {
    Bus* bus;
    Rider* rider;
} typedef bb_t_args;

struct BoardThreadArgs {
    Rider* riders;
    Bus* bus;
    int total_waiting_riders;
} typedef b_t_args;

void depart(Bus* bus) {
    printf("Bus has departed!\n");
    for (int i = 0; i < bus->total_riders; i++) {
        if (i >= (0.4 * bus->total_riders)) {
            printf("Bus is almost arriving at station! (Riders left: %d)", bus->total_riders - i);
        }

        printf("Rider %d has arrived its destiny.\n", bus->riders[i]);
        sleep(1);
    }
    memset(bus->riders, 0, sizeof(bus->total_riders));
    bus->at_station = 1;
    return;
}

void boardBus(bb_t_args args) {
    Bus* bus = args.bus;
    Rider rider = *args.rider;
    bus->riders[bus->total_riders + 1] = rider;
    bus->total_riders++;
    printf("Rider %d has boarded!", rider.id);
    return;
}

void board(b_t_args args) {

    int total_waiting_riders = args.total_waiting_riders;
    Bus* bus = args.bus;
    Rider* riders = args.riders;

    pthread_t threads[50];

    int i = 0;

    bb_t_args board_args = {
        .bus = bus
    };

    while (i < 50 && i < total_waiting_riders) {

        board_args.rider = &riders[i];

        int status = pthread_create(&threads[i], NULL, boardBus, &board_args);
        if (status) {
            perror("Falha na criação da thread.");
            return;
        }
        sleep(1);
        i++;
    }

    printf("%d passengers have boarded!", (total_waiting_riders > 50 ? 50 : total_waiting_riders));
    return;
}



int main() {
    Rider* riders = malloc(sizeof(Rider) * 100);
    Rider new = {
        .id = -1
    };
    pthread_t bus_t;
    pthread_t station_t;
    pthread_t board_t;
    int waiting_riders = 0;
    int new_rider = 0;
    Bus bus = {
        .total_riders = 0,
        .riders = malloc(sizeof(Rider) * 50),
        .at_station = 1
    };

    b_t_args board_args = {   
        .bus = &bus,
        .riders = riders,
        .total_waiting_riders = waiting_riders
    };

    while (1) {
        if (new_rider) {
            new.id = waiting_riders;
            riders[waiting_riders] = new;
            waiting_riders++;
        }

        if (bus.at_station) {
            int status = pthread_create(&board_t, NULL, board, &board_args);
            if (status) {
                perror("Falha na criação da thread");
                free(riders);
                free(bus.riders);
                return;
            }

            pthread_join(board_t, NULL);
            pthread_create(&bus_t, NULL, depart, &bus);
        }

        new_rider = rand() % 2;
        sleep(1);
    }

    

    

    free(riders);
    free(bus.riders);
}

//Consertar erros e testar.