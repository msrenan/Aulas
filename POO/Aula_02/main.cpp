#include <iostream>
#include "horario.h"

using namespace std;

int main() {
    horario mytime = horario();

    mytime.set_horario(18, 60, 67);

    mytime.secure_time();

    mytime.print12();
    mytime.print24();
}