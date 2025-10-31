#include <iostream>
#include <math.h>

using namespace std;

class Ponto {
    private:
        int x;
        int y;
    
    public:
        Ponto();
        Ponto(int x, int y);

        void setX(int x);
        void setY(int y);

        int getX();
        int getY();

        void print();
        
        friend ostream& operator<<(ostream& out, Ponto p);

};


Ponto::Ponto() {
    x = 0;
    y = 0;
}

Ponto::Ponto(int x, int y) {
    this->x = x;
    this->y = y;
}

void Ponto::setX(int x) {
    this->x = x;
}

void Ponto::setY(int y) {
    this->y = y;
}

int Ponto::getX() {
    return x;
}

int Ponto::getY() {
    return y;
}

void Ponto::print() {
    cout << "(" << getX() << ", " << getY() << ")" << endl;
}

ostream& operator<<(ostream& out, Ponto p) {
    out << "(" << p.getX() << ", " << p.getY() << ")";

    return out;
}
// -------------------------------------------------------------

class Vetor : public Ponto {
    private:
        double const norma;
    
    public:
        Vetor();
        Vetor(int x, int y);

        friend Vetor operator+(Vetor a, Vetor b);
        friend Vetor operator-(Vetor a, Vetor b);
        friend Vetor operator*(int c, Vetor a);
        friend Vetor operator*(Vetor a, int c);

        double getNorma() const;

        friend ostream& operator<<(ostream& out, Vetor v);

        double pEsc(Vetor b);

};

Vetor::Vetor() : Ponto(0, 0) , norma(0) {}

Vetor::Vetor(int x, int y) : Ponto(x, y) , norma(sqrt(pow(x, 2) + pow(y, 2))){}

Vetor operator+(Vetor a, Vetor b) {
    return Vetor(a.getX() + b.getX(), a.getY() + b.getY());
}
Vetor operator-(Vetor a, Vetor b) {
    return Vetor(a.getX() - b.getX(), a.getY() - b.getY());
}

Vetor operator*(int c, Vetor a) {
    return Vetor(a.getX() * c, a.getY() * c);
}

Vetor operator*(Vetor a, int c) {
    return Vetor(a.getX() * c, a.getY() * c);
}

double Vetor::getNorma() const {
    return norma;
}

ostream& operator<<(ostream& out, Vetor v) {
    out << "(" << v.getX() << ", " << v.getY() << ")" << " Norma: " << v.getNorma();
    return out;
}

double Vetor::pEsc(Vetor b) {
    return (this->getX() * b.getX()) + (this->getY() * b.getY());
}

//----------------------------------------------------------
Vetor operator-(Ponto a, Ponto b) {
    return Vetor(a.getX() - b.getX(), a.getY() - b.getY());
}

Ponto operator+(Ponto a, Vetor v) {
    return Ponto(a.getX() + v.getX(), a.getY() + v.getY());
}

Ponto operator-(Ponto a, Vetor v) {
    return Ponto(a.getX() - v.getX(), a.getY() - v.getY());
}