#include <iostream>

using namespace std;

class Teste {
    private:
        int a = 10; //Class atributes
        int c;
    
    public:

        Teste(int x) { //Class constructor
            c = x;
        }

        int b = 20;

        void teste() {
            cout << "Tested" << endl;
        }

        int get_a() { //Class method
            return a;
        }

        int get_c() {
            return c;
        }
};

int main() {

    cout << "Hello, World!" << endl;

    Teste t(5);
    t.teste();
    cout << t.get_a() << t.b << endl;

    cout << t.get_c() << endl;

}