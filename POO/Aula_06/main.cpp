#include <iostream>
#include "fracao.h"

using namespace std;

Fracao mult(Fracao fd, Fracao fe) {
    return Fracao(fd.getnum() * fe.getnum(), fd.getden() * fe.getden());
}

int main() {

    Fracao f1 = Fracao(1, 4);
    Fracao f2 = Fracao(1, 2);
    Fracao f3 = Fracao();

    f3 = mult(f1, f2);
    f3.mostra();


}