
//*********************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
//
//**********************************************************
#ifndef ___CONTROLLER__H___
#define ___CONTROLLER__H___

#include "../Bitmap/Bmp.h"
#include "../Widgets/Button.h"
#include "../Widgets/Checkbox.h"
#include "../Widgets/Alert.h"
#include <vector>

using namespace std;

class Controller
{
private:
    Bmp* img;                     // imagem a ser manipulada
    vector<float> rgb;           // controla cores passadas em RGB
    int opcao = 50;             // variavel para selecao do que sera exibido na canvas.
    int mx, my;                //variaveis do mouse para poder exibir dentro da render().

public:
    int screenWidth = 1200, screenHeight = 1000;
    vector<Widget*> wds;     // controla todos os widgets do programa
    Controller();
    ~Controller();
    void InitCanvas();
    void Render();
    void Keyboard(int key);
    void Mouse(int x, int y, int state);
};

#endif