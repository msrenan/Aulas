#include <iostream>

using namespace std;

// ------------ Definição da classe ----------------

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

        int static getTotalPedidos() {
            return Pedido::num_pedidos;
        }

        int static getTotalItems() {
            return Pedido::total_items;
        }

        string getNome() {
            return this->Nome;
        }

        int getQtd() {
            return this->qtd;
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

// ---------- Programa Principal -----------------

int main() {
    Pedido renan = Pedido("Renan", 2);
    Pedido danilo = Pedido("Danilo", 5);

    cout << "Total de items solicitados: " << renan.getTotalItems() << endl;
    cout << "Total de pedidos ativos: " << renan.getTotalPedidos() << endl;

    renan.setItem(0, "SilkSong");
    danilo.setItem(0, "Fiat uno 0km");
    renan.setItem(1, "1000 reais no pix");
    danilo.setItem(1, "1000 reais no pix");
    danilo.setItem(2, "Férias");
    danilo.setItem(3, "Nota boa na prova de POO");
    danilo.setItem(4, "Uma passagem pra casa");

    cout << endl;
    cout << "Pedidos de " << renan.getNome() << ": " << endl;
    string item;
    for (int i = 0; i < renan.getQtd(); i++) {
        bool res = renan.getItem(i, item);
        if (!res) {
            cout << "Item não encontrado..." << endl;
            continue;
        }
        cout << "#" << i << ": " << item << endl;
    }

    cout << endl;
    cout << "Pedidos de " << danilo.getNome() << ": " << endl;
    string item2;
    for (int i = 0; i < danilo.getQtd(); i++) {
        bool res = danilo.getItem(i, item2);
        if (!res) {
            cout << "Item não encontrado..." << endl;
            continue;
        }
        cout << "#" << i << ": " << item2 << endl;
    }
}