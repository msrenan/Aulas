#include <iostream>
#include "bintree.h"

using namespace std;

int main() {

    BinaryTree T = BinaryTree();

    T.inserir(50, T.getRoot(), NULL);
    T.inserir(25, T.getRoot(), NULL);
    T.inserir(10, T.getRoot(), NULL);
    T.inserir(26, T.getRoot(), NULL);
    T.mostra(T.getRoot());
    T.remove(26, T.getRoot(), NULL);
    T.mostra(T.getRoot());
 //arrumar o remove.
}