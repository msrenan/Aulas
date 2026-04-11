#include <iostream>
#include <fstream>

using namespace std;

int main() {

    ifstream tabela("tabela.csv");

    string id, nome, numero;

    //cabeçalho
    string header;
    getline(tabela, header);
    cout << header << endl;
    
    cout << endl;

    while (getline(tabela, id, ',')) {
        getline(tabela, nome, ',');
        getline(tabela, numero);
        cout << id << " | " << nome << " | " << numero << endl;
    }
}