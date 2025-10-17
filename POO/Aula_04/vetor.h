#include <iostream>

using namespace std;



class Vetor {
    private:
        int tam;
        int* vec_ptr;

    public:
        Vetor(int size) {
            this->tam = size;
            this->vec_ptr = new int[size];
        }

        bool setValor(int pos, int val) {
            if (pos > this->tam - 1) {
                return false;
            }

            this->vec_ptr[pos] = val;
            return true;
        }

        bool getValor(int pos, int& vec_val) {
            if (pos > this->tam - 1) {
                return false;
            }
            
            vec_val = this->vec_ptr[pos];
            return true;
        }

        int len() {
            return this->tam;
        }

        ~Vetor() {
            delete [] this->vec_ptr;
        }
};

