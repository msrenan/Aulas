#include <iostream>
#include <iomanip>

using namespace std;

/*
Para adicionar uma nova Figura, siga os seguintes passos:
* 1. Crie sua classe, herdando a classe figura publicamente:
    class NovaFigura : public Figura
* 2. Crie o construtor padrão da sua nova classe e um construtor contendo todas 
     as informações que devem ser passadas para o funcionamento da classe.
        NovaFigura() : Figura() { ... };
        NovaFigura(args, char f, int x, int y) : Figura(f, x, y) { ... };

* 3. Crie os métodos necessários para o bom funcionamento da classe, não se esqueça de sobrescrever os métodos
        **draw()
        **move(int offX, int offY)
        **setPosition(int x, int y)
     caso necessário.
*/

/// Coordinates
///@param x Coordenada x 
///@param y Coordenada y
struct Coordinates {
    int x;
    int y;
};

/*
# Figura

Classe base que define propriedades básicas de uma figura que será exibida.

@param shape `protected` - Contém o 'sprite' da figura
@param position `protected` - Contém a posição atual da figura na tela
*/
class Figura {
    protected:
        const char shape;
        Coordinates position;
    
    public:
        Figura() : shape('#'){
            position.x = 0;
            position.y = 0;
        }
        Figura(char myShape, int x, int y) : shape(myShape) {
            position.x = x;
            position.y = y;
        }
        virtual ~Figura() {
            delete this;
        }
        virtual void draw() const;
        virtual void setPosition(int x, int y);
        virtual void move(int offX, int offY);
};

/*# Figura::draw

Desenha a figura sem modificações;
*/
void Figura::draw() const {
    cout << setw(position.x) << "" << this->shape << " ";
}

/*
# Figura::setPosition

Define a posição da figura na tela.

@param x `int` - Coordenada x da posição
@param y `int` - Coordenada y da posição
*/
void Figura::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

/*
# Figura::setPosition

Define a posição da figura na tela.

@param offX `int` - Offset x do movimento
@param offY `int` - Offset y do movimento
*/
void Figura::move(int offX, int offY) {
    position.x += offX;
    position.y += offY;
}

/*
# ColoredShape (Figura)

Classe filha da classe Figura que representa figuras ooloridas.

@param color `string` - Código (ex: \033[31m - cor vermelha) de cor da figura.

##### OBS: Os parametros `shape` e `position` são herdados da classe Figura
*/
class ColoredShape : public Figura {
    private:
        string color;

    public:
        ColoredShape() : Figura() {
            color = "";
        }

        ColoredShape(string c, char f, int x, int y) : Figura(f, x, y) {
            color = c;
        }

        void setColor(string c);
        void draw() const;
};

/*
# ColoredShape::setColor

Função que define a cor da figura.

@param c `string` - Código (ex: \033[31m - cor vermelha) de cor da figura.
*/
void ColoredShape::setColor(string c) {
    color = c;
}

/*# ColoredShape::draw

Desenha a figura com a cor escolhida.

##### OBS: Está função está sobrescrevendo a função da classe base (Figura)
*/
void ColoredShape::draw() const {
    cout << setw(position.x) << "" << color << shape << "\033[m ";
}

/*
# BigShape (Figura)

Classe filha da classe Figura que representa figuras que tem um tamanho diferente (maior) que o padrão.

@param width `int` - Tamanho (largura) da Figura
*/
class BigShape : public Figura {
    private:
        int width;

    public:
        BigShape() : Figura() {
            width = 1;
        }

        BigShape(int w, char f, int x, int y) : Figura(f, x, y) {
            width = w;
        }

        void setWidth(int w);
        void draw() const;
        void move(int offX, int offY);
};

/*
# BigShape::setWidth

Função que define a largura da Figura.

@param w `int` - Valor da largura
*/
void BigShape::setWidth(int w) {
    width = w;
}

/*# BigShape::draw

Desenha a figura com a largura escolhida.

##### OBS: Está função está sobrescrevendo a função da classe base (Figura)
*/
void BigShape::draw() const {
    cout << setw(position.x) << "";
    for (int i = 0; i < width; i++) {
        cout << shape;
    }
    cout << " ";
}

/*
# BigShape::setPosition

Define a posição da figura na tela, contando a largura da figura no deslocamento.

@param offX `int` - Offset x do movimento
@param offY `int` - Offset y do movimento

##### OBS: Está função está sobrescrevendo a função da classe base (Figura)
*/
void BigShape::move(int offX, int offY) {
    position.x += offX + width;
    position.y += offY;
}