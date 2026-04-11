#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ofstream file("teste.txt");
    if (file.is_open()) {
        file << "receba" << endl;
        file << "Caralho" << endl;
        file.close();
    }

    ifstream read_file("teste.txt");
    string line;
    if (read_file.is_open()) {
        while(getline(read_file, line)) {
            cout << line << endl;
            cout << line[0] << endl;
        }
        read_file.close();
    }
}