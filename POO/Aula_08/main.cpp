//Feito por Renan Machado Santos (846346), Gabriel Kazushige Vieira (844938) e Eduardo Pereira Filho (844851)

#include <iostream>
#include <vector>
#include "polimorfismo.h"
#include <chrono>
#include <thread>

using namespace std;

void DisplayRefresh(vector<Figura*> figs) {
    for (Figura* f : figs) {
        f->draw();
    }
    cout << "\r" << flush;
}

int main() {

    int frames = 0;
    Figura* A = new Figura('A', 0, 0);
    ColoredShape* redB = new ColoredShape("\033[31m", 'B', 0, 0);
    BigShape* big_3_C = new BigShape(3, 'C', 0, 0);

    vector<Figura*> figuras;

    while (frames < 20) {
        figuras.clear();

        A->move(1, 1);
        redB->move(1, 1);
        big_3_C->move(1, 1);

        figuras.push_back(A);
        figuras.push_back(redB);
        figuras.push_back(big_3_C);
        DisplayRefresh(figuras);
        this_thread::sleep_for(chrono::milliseconds(500));
        frames++;
    }

}


