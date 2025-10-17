#include <iostream>

using namespace std;

class Fracao {
    private:
        int den;
        int num;
    
    public:
        void setnum(int n) {
            num = n;
        }

        void setden(int n) {
            if (n == 0) {
                n = 1;
            }
            den = n;
        }

        int getnum() const {
            return num;
        }

        int getden() const {
            return den;
        }

        void mostra() {
            cout << getnum() << "/" << getden() << endl;
        }

        Fracao() {
            setnum(0);
            setden(1);
        }

        Fracao(int num, int den) {
            setnum(num);
            setden(den);
        }
};