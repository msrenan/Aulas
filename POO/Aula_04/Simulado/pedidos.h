#include <iostream>

using namespace std;

class Pedido {
    private:
        static int num_pedidos;
        static int next_ID;
        static int total_items;
        const int id;
        string Nome;
        int qtd;
        string* items;

        void static incId() {
            Pedido::next_ID++;
        }

    public:
        Pedido(string nome, int ask_qtd) : id(Pedido::next_ID) {
            this->incId();
            this->num_pedidos++;
            this->Nome = nome;
            if (qtd > 0) {
                this->qtd = ask_qtd;
            } else {
                this->qtd = 1;
            }
            Pedido::total_items += this->qtd;
            this->items = new string[this->qtd];
        }

        bool setItem(int pos, string item) {
            if (pos < 0 && pos >= this->qtd) {
                cout << "Esta não é uma posição válida!" << endl;
                return false;
            }

            this->items[pos] = item;
            return true;
        }

        bool getItem(int pos, string& item) {
            if (pos < 0 && pos >= this->qtd) {
                cout << "Esta não é uma posição válida!" << endl;
                return false;
            }
            
            item = this->items[pos];
            return true;
        }

        string getNome() {
            return this->Nome;
        }

        int static getTotalPedidos() {
            return Pedido::num_pedidos;
        }

        int static getTotalItems() {
            return Pedido::total_items;
        }

        ~Pedido() {
            Pedido::num_pedidos--;
            delete [] this->items;
            cout << "O pedido #" << this->id << "está sendo deletado!";
        }
};

int Pedido::num_pedidos = 0;
int Pedido::next_ID = 0;
int Pedido::total_items = 0;