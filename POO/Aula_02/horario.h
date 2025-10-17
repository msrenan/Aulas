#include <iostream>

using namespace std;

class horario {

    private:
        int hora;
        int min;
        int sec;

    public:
        horario(int h = 0, int m = 0, int s = 0) {
            this->hora = h;
            this->min = m;
            this->sec = s;
        }

        void set_horario(int h, int m, int s) {
            this->hora = h;
            this->min = m;
            this->sec = s;
        }

        void print12() {

            this->secure_time();

            if (this->hora > 12 && this->hora < 23) {

                printf("Agora são: %d hora(s) %d minuto(s) e %d segundo(s) %s\n", this->hora - 12, this->min, this->sec, "P.M.");

            } else if (this->hora >= 0 && this->hora <= 12) {

                printf("Agora são: %d hora(s) %d minuto(s) e %d segundo(s) %s\n", this->hora, this->min, this->sec, "A.M.");

            } 
        }

        void print24() {
            this->secure_time();
            printf("Agora são: %d hora(s) %d minuto(s) e %d segundo(s)\n", this->hora, this->min, this->sec);
        }

        void secure_time() {
    
            if (this->sec > 59) {
                this->min += this->sec / 60;
                this->sec = this->sec % 60;
            }

            if (this->min > 59) {
                this->hora += this->min / 60;
                this->min = this->min % 60;
                
            }

            if (this->hora >= 24) {
                this->hora = this->hora % 24;
            }
        }

        int get_hora() {
            return this->hora;
        }

        void set_hora(int h) {
            this->hora = h;
        }

        int get_min() {
            return this->min;
        }

        void set_min(int m) {
            this->min = m;
        }

        int get_sec() {
            return this->sec;
        }

        void set_sec(int s) {
            this->sec = s;
        }



};