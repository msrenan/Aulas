#include <iostream>

using namespace std;

class Dog {
    private:
        static int quantidade;
        const string nome;
        const int idade;
        const string raca;
        double peso;
        const string porte;
        const string cor_do_pelo;
        bool alimentado;
        bool acordado;
        int fadiga;

        void isTired() {
            if (this->fadiga >= 3) {
                this->alimentado = false;
            }
        }

        int validaIdade(int age) {
            if (age > 14 || age <= 0) {
                return 7;
            } else {
                return age;
            }
        }
    
    public:
        Dog(string _nome, int _idade, string _raca, double _peso, string _porte, string color) : 
        nome(_nome), idade(validaIdade(_idade)), raca(_raca), peso(_peso), porte(_porte), cor_do_pelo(color) {
            this->alimentado = false;
            this->acordado = false;
            this->fadiga = 0;
        }

        void comer() {
            cout << this->nome << " acabou de ser alimentado!" << endl;
            this->peso += 0.5;
            this->alimentado = true;
            this->fadiga = 0;
        }

        void acordar() {
            cout << this->nome << " acabou de acordar!" << endl;
            this->acordado = true;
        }

        void dormir() {
            cout << this->nome << " acabou de dormir!" << endl;
            this->fadiga = 0;
            this->acordado = false;
        }

        void latir() {
            if (!this->alimentado) {
                cout << this->nome << " está com muita fome para latir..." << endl;
                return;
            }
            cout << this->nome << " disse: RUF RUF!!" << endl;
            this->fadiga++;
            this->isTired();
        }

        void acariciar() {
            if (this->fadiga > 0) {
                this->fadiga--;
            }
            cout << this->nome << " abana o rabinho!" << endl;
        }

        string getNome() const {
            return this->nome;
        }

        int getIdade() const {
            return this->idade;
        }

        string getRaca() const {
            return this->raca;
        }

        double getPeso() {
            return this->peso;
        }

        string getPorte() const {
            return this->porte;
        }

        string getCorPelagem() const {
            return this->cor_do_pelo;
        }

        bool estaAlimentado() const {
            return this->alimentado;
        }
        
        bool estaAcordado() const {
            return this->acordado;
        }

        void static adotar() {
            Dog::quantidade++;
        }

        int static getNumeroDoguinhos() {
            return Dog::quantidade;
        }
};

int Dog::quantidade = 0;