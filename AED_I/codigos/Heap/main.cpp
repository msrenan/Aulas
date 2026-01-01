#include <iostream>
#include "heap.h"

int main() {
    
    Heap h = Heap();

    h.mostra(h.getRoot());

    h.insere(h.getRoot(), 2);

    h.mostra(h.getRoot());

    

}