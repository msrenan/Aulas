#include <iostream>

using namespace std;

struct horario {
    int h;
    int min;
    int sec;
};

void set_time(horario* horario,int hora, int min, int segs) {
    horario->h = hora;
    horario->min = min;
    horario->sec = segs;
}

void print12(horario time) {

    if (time.h > 12 && time.h < 23) {

        printf("Agora são: %d hora(s) %d minuto(s) e %d segundo(s) %s\n", time.h - 12, time.min, time.sec, "P.M.");

    } else if (time.h >= 0 && time.h <= 12) {

        printf("Agora são: %d hora(s) %d minuto(s) e %d segundo(s) %s\n", time.h, time.min, time.sec, "A.M.");

    } 
}

void secure_time(horario& time) {
    
    if (time.sec > 59) {
        time.sec = time.sec - 60;
        time.min++;
    }

    if (time.min > 59) {
        time.min = time.min - 60;
        time.h++;
    }

    if (time.h >= 24) {
        time.h = time.h - 24;
    }

}

void print24(horario hora) {
    printf("Agora são: %d hora(s) %d minuto(s) e %d segundo(s)\n", hora.h, hora.min, hora.sec);
}

int main() {

    horario mytime;

    set_time(&mytime, 24, 60, 67);

    secure_time(mytime);

    print12(mytime);
    print24(mytime);

}