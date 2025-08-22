#include <iostream>

using namespace std;

class Vehicle { // Base class
    protected: // Protected -> Data access provided inside the parent and child class only.
        string brand;
        string model;
        double price;
        int year;
    
    public: // Public -> Data acces provided inside the parent class, inside childe classes and outside the classes.
        Vehicle(string marca, string modelo, double preco, int ano) { // Base class constructor
            brand = marca;
            model = modelo;
            price = preco;
            year = ano;
        };

        void start() {
            cout << "The vehicle is starting!" << endl;
        }

        void ride() {
            cout << "Riding the vehicle!" << endl;
        }

        void stop() {
            cout << "Stopped the vehicle!" << endl;
        }

        string get_brand() {
            return brand;
        }

        string get_model() {
            return model;
        }
        double get_price() {
            return price;
        }

        int get_year() {
            return year;
        }
};

class Car : public Vehicle {
    private: // Private -> Data access provided only inside data's declaration class
        string gas_type;
    
    public:

        Car(string marca, string modelo, double preco, int ano, string gas) : Vehicle(marca, modelo, preco, ano) {
            gas_type = gas;
        };

        string get_gas() {
            return gas_type;
        }
        
};

class Helicopter : public Vehicle {
    private:
        int amount_ammo;
        string ammo_type;
    
    public:

        Helicopter(string marca, string modelo, double preco, int ano, string type_ammo, int ammo_count) : Vehicle(marca, modelo, preco, ano) {
            amount_ammo = ammo_count;
            ammo_type = type_ammo;
        }

        void report() {
            cout << "Reporting, sir!!!" << endl;
            cout << "Brand: " << brand << endl;
            cout << "Model: " << model << endl;
            cout << "Year: " << year << endl;
            cout << "Price: R$" << price << endl;
            cout << "Ammo Type: " << ammo_type << endl;
            cout << "Amount of Ammo: " << amount_ammo << endl;
            cout << "End of report, sir!!!" << endl;
        }
};

int main() {

    Car mycar("Ford", "Mustang", 650000.0, 2015, "Flex");

    cout << "Brand: " << mycar.get_brand() << endl;
    cout << "Model: " << mycar.get_model() << endl;
    cout << "Year: " << mycar.get_year() << endl;
    cout << "Price: R$" << mycar.get_price() << endl;
    cout << "Fuel Type: " << mycar.get_gas() << endl;

    cout << endl;

    Helicopter heli("Embraer", "Kobra256", 12500000.0, 210, ".50", 500);

    heli.report();

}