#include <iostream>
#include <vector>

using namespace std;

#define TAM 10

class Fila {
    private:
        int data[TAM];
        int primeiro_elemento;
        int ultimo_elemento;
        int numero_elementos;
    
    public:
        Fila() {
            primeiro_elemento = 0;
            ultimo_elemento = 0;
            numero_elementos = 0;
        }

        bool cheia() {
            if (numero_elementos == TAM) {
                return true;
            } else {
                return false;
            }
        }

        bool vazia() {
            if (numero_elementos == 0) {
                return true;
            } else {
                return false;
            }
        }

        bool insere(int num) {
            if (!cheia()) {
                data[ultimo_elemento] = num;
                if (ultimo_elemento == TAM - 1) {
                    ultimo_elemento = 0;
                } else {
                    ultimo_elemento++;
                }
                numero_elementos++;
                return true;
            } else {
                return false;
            }
        }

        bool retira(int& valor_retirado) {
            if (!vazia()) {
                valor_retirado = data[primeiro_elemento];
                numero_elementos--;
                if (primeiro_elemento == TAM - 1) {
                    primeiro_elemento = 0;
                } else {
                    primeiro_elemento++;
                }
                return true;
            } else {
                return false;
            }
        }
};

class FilaSequencial {
    private:
        vector<int> data;
        int primeiro_elemento;
        int ultimo_elemento;
        int numero_elementos;
    
    public:
        FilaSequencial() {
            primeiro_elemento = 0;
            ultimo_elemento = 0;
            numero_elementos = 0;
        }

        bool vazia() {
            if (numero_elementos == 0) {
                return true;
            } else {
                return false;
            }
        }

        bool insere(int num) {
            data.push_back(num);
            ultimo_elemento++;
            numero_elementos++;
            return true;
        }

        bool retira(int& valor_retirado) {
            if (!vazia()) {
                valor_retirado = data[0];
                data.erase(data.begin());
                ultimo_elemento--;
                numero_elementos--;
                return true;
            } else {
                return false;
            }
        }
};