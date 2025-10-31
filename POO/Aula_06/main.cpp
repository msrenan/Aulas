#include <iostream>
//Renan Machado Santos (846346) - Gabriel Kazushige Vieira Kosaihira (844938)
using namespace std;

class Fracao {
    private:
        int denominador;
        int numerador;
    
    public:

        Fracao();
        Fracao(int den, int num);
        void mostra();
        void setden(int x);
        void setnum(int x);
        int getden() const;
        int getnum() const;
        double value() const;
    
        friend int FindCommonDen(const Fracao& f1, const Fracao& f2);
        friend Fracao operator*(const Fracao& f1, const Fracao& f2);
        friend Fracao operator*(const Fracao& f1, int cte);
        friend Fracao operator*(int cte, const Fracao& f1);
        friend Fracao operator+(const Fracao& f1, const Fracao& f2);
        friend Fracao operator+(const Fracao& f1, int cte);
        friend Fracao operator+(int cte, const Fracao& f1);
        friend Fracao operator-(const Fracao& f1, const Fracao& f2);
        friend Fracao operator-(const Fracao& f1, int cte);
        friend Fracao operator-(int cte, const Fracao& f1);
        friend ostream &operator<<(ostream& out, const Fracao& f);
        friend istream &operator>>(istream& in, Fracao& f);
        Fracao operator++(int n);
        Fracao operator--(int n);
        Fracao operator++();
        Fracao operator--();
        friend bool operator>(const Fracao& f1, const Fracao& f2);
        friend bool operator>(const Fracao& f1, int cte);
        friend bool operator>(int cte, const Fracao& f1);
        friend bool operator<(const Fracao& f1, const Fracao& f2);
        friend bool operator<(const Fracao& f1, int cte);
        friend bool operator<(int cte, const Fracao& f1);
        friend bool operator==(const Fracao& f1, const Fracao& f2);
        friend bool operator==(const Fracao& f1, int cte);
        friend bool operator==(int cte, const Fracao& f1);
        friend bool operator!=(const Fracao& f1, const Fracao& f2);
        friend bool operator!=(const Fracao& f1, int cte);
        friend bool operator!=(int cte, const Fracao& f1);
        friend bool operator>=(const Fracao& f1, const Fracao& f2);
        friend bool operator>=(const Fracao& f1, int cte);
        friend bool operator>=(int cte, const Fracao& f1);
        friend bool operator<=(const Fracao& f1, const Fracao& f2);
        friend bool operator<=(const Fracao& f1, int cte);
        friend bool operator<=(int cte, const Fracao& f1);
};

int FindCommonDen(const Fracao& f1, const Fracao& f2) {
    int dmax = f1.denominador > f2.denominador ? f1.denominador : f2.denominador;
    int dmin = f1.denominador > f2.denominador ? f2.denominador : f1.denominador;

    if (dmax % dmin == 0) {
        return dmax;
    }

    return dmax * dmin;
}

Fracao operator*(const Fracao& f1, const Fracao& f2) {
    return Fracao(f1.denominador * f2.denominador, f1.numerador * f2.numerador);
}

Fracao operator*(const Fracao& f1, int cte) {
    return Fracao(f1.denominador, f1.numerador * cte);
}

Fracao operator*(int cte, const Fracao& f1) {
    return Fracao(f1.denominador, f1.numerador * cte);
}

Fracao operator+(const Fracao& f1, const Fracao& f2) {
    int den = FindCommonDen(f1, f2);
    int num;

    int nf1 = f1.numerador * (den / f1.denominador);
    int nf2 = f2.numerador * (den / f2.denominador);

    return Fracao(den, nf1 + nf2);
}

Fracao operator+(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 + f2;
}

Fracao operator+(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f2 + f1;
}

Fracao Fracao::operator++(int n) {
    Fracao cpy(*this);
    setnum(numerador + denominador);
    return cpy;
}

Fracao Fracao::operator++() {
    return Fracao(denominador, numerador + denominador);
}

Fracao operator-(const Fracao& f1, const Fracao& f2) {
    int den = FindCommonDen(f1, f2);
    int num;

    int nf1 = f1.numerador * (den / f1.denominador);
    int nf2 = f2.numerador * (den / f2.denominador);

    return Fracao(den, nf1 - nf2);
}

Fracao operator-(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 - f2;
}

Fracao operator-(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f2 - f1;
}

Fracao Fracao::operator--(int n) {
    Fracao cpy(*this);
    numerador = numerador - denominador;
    return cpy;
}

Fracao Fracao::operator--() {
    return Fracao(denominador, numerador - denominador);
}

Fracao::Fracao() {
    denominador = 1;
    numerador = 0;
}

Fracao::Fracao(int den, int num) {
    denominador = den;
    numerador = num;
}

ostream& operator<<(ostream& out, const Fracao& f){
    out << f.numerador << "/" << f.denominador;
    return out;
}

istream &operator>>(istream& in, Fracao& f) {
    in >> f.numerador;
    in.ignore();
    in >> f.denominador;

    return in;
}

void Fracao::setden(int x) {
    denominador = x;
    return;
}

void Fracao::setnum(int x) {
    numerador = x;
    return;
}

int Fracao::getden() const {
    return denominador;
}

int Fracao::getnum() const {
    return numerador;
}

double Fracao::value() const {
    return (double) numerador / (double) denominador;
}

bool operator>(const Fracao& f1, const Fracao& f2) {
    return f1.value() > f2.value();
}

bool operator>(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 > f2;
}

bool operator>(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 > f2;
}

bool operator<(const Fracao& f1, const Fracao& f2) {
    return f1.value() < f2.value();
}

bool operator<(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 < f2;
}

bool operator<(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 < f2;
}

bool operator==(const Fracao& f1, const Fracao& f2) {
    return f1.value() == f2.value();
}

bool operator==(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 == f2;
}

bool operator==(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 == f2;
}

bool operator!=(const Fracao& f1, const Fracao& f2) {
    return f1.value() != f2.value();
}

bool operator!=(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 != f2;
}

bool operator!=(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 != f2;
}

bool operator>=(const Fracao& f1, const Fracao& f2) {
    return f1.value() >= f2.value();
}

bool operator>=(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 >= f2;
}

bool operator>=(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 >= f2;
}

bool operator<=(const Fracao& f1, const Fracao& f2) {
    return f1.value() <= f2.value();
}

bool operator<=(const Fracao& f1, int cte) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 <= f2;
}

bool operator<=(int cte, const Fracao& f1) {
    Fracao f2 = Fracao(f1.denominador, cte * f1.denominador);
    return f1 <= f2;
}


int main() {
    Fracao f1 = Fracao(2, 1); // 1/2
    cout << f1.value() << endl;
    Fracao f2 = Fracao(4, 1); // 1/4
    cout << f2.value() << endl;

    Fracao f3 = Fracao();

    cout << f1 + f2 << endl; // +
    cout << f1 - f2 << endl; // -
    cout << f1 * f2 << endl; // *
    cout << ++f1 << endl; // ++ pre
    cout << f1++ << endl; // ++ pos
    cout << f1 << endl; // <<
    cout << --f1 << endl; // -- pre
    cout << f1-- << endl; // -- pos
    cout << f1 * 2 << endl; // * por constante
    cout << 4 * f1 << endl; // constante * fracao
    cout << f2 + 2 << endl; // + constante
    cout << 4 + f2 << endl; // constante + fracao
    cout << f1 - 2 << endl; // - constante
    cout << 4 - f1 << endl; // constante - fracao

    cin >> f3;

    cout << f3 * 3 << endl;
    Fracao f4;
    cin >> f4; 
    printf("%b (>)\n%b (<)\n%b (==)\n%b (>=)\n%b (>=))\n%b (!=)\n %b (>)\n %b (<=)\n", f1 > f2, f2 < f1, f1 == 2*f2, f1 >= 2*f2, 3*f2 >= f1, f1 != f2, 4*f1 > 2*f2, f1 <= 10*f2);
}