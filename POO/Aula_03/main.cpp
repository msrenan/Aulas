
/* Participantes: Danilo Magalhães Valois (847901)
		  Gabriel Kazushige Vieira Kosaihira (844938)
		  Renan Machado Santos (846346)
	---------------------------------
	|         Cachorro              |
	|-------------------------------
	| - static quantidade: int      
	| - const nome: String                 
	| - const idade: int                  
	| - const raça: String		        
	| - peso: double		        
	| - const porte: String		        
	| - const cor_do_pelo: String		    
	| - alimentado: bool		    
	| - acordado: bool	
    | - fadiga: bool	
    | - isTired() -> void        
	---------------------------------
	| + CriaCachorro(nome, idade	
	|		 raça, peso	            
	|		 porte, cor)	        
	| + comer() -> alimentado = true
	| + acoradar() ->		        
	|	acordado = true		        
	| + dormir() ->			        
	|	acorado = false		        
	| + latir() -> void		        
	| + getIdade() const -> idade		    
	| + getNome() const-> nome		    
	| + getRaca() const -> raca		    
	| + getPeso() -> peso		    
	| + getCorPelagem() const ->		    
	|	cor_do_pelo		            
	| + getPorte() const -> Porte		    
	| + estaAlimentado() const ->		    
	|	alimentado		            
	| + estaAcordado() const ->		    
	|	acordado		            
	| + acariciar() -> void 	
	| + static adotar() -> void
	| + static getNumeroDoguinhos() -> int	
	---------------------------------
*/



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


void getInfo(Dog& dog) {
    cout << "\n----- Informações do " << dog.getNome() << " -----\n"
    << "Nome: " << dog.getNome() << "\n"
    << "Idade: " << dog.getIdade() << "\n"
    << "Raça: " << dog.getRaca() << "\n"
    << "Peso: " << dog.getPeso() << "\n"
    << "Porte: " << dog.getPorte() << "\n"
    << "Cor da pelagem: " << dog.getCorPelagem() << "\n"
    << "Está acordado: " << (dog.estaAcordado() ? "Sim" : "Não" )<< "\n"
    << "Está alimentado: " << (dog.estaAlimentado() ? "Sim" : "Não")<< "\n"
    << "Número de doguinhos: " << dog.getNumeroDoguinhos() << endl << endl; 

}

int main() {
    Dog rex = Dog("Rex", 4, "PitBull", 8.0, "Grande", "Marrom bom-bom");
    Dog::adotar();
    rex.acordar();

    rex.comer();

    rex.latir();
    rex.latir();
    rex.latir();
    getInfo(rex);
    rex.latir();
    rex.comer();
    rex.latir();
    rex.dormir();
    getInfo(rex);
    rex.acariciar();

    Dog pipoca = Dog("Pipoca", 1, "Shin tzu", 2.0, "Pequeno", "Branco");
    pipoca.acordar();
    pipoca.latir();
    pipoca.latir();
    pipoca.latir();
    getInfo(pipoca);

    Dog amora = Dog("Amora", 3, "BorderColi", 5.0, "Médio", "Vermelho sangue");
    amora.acordar();
    getInfo(amora);
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.comer();
    amora.dormir();
    getInfo(amora);


    
}