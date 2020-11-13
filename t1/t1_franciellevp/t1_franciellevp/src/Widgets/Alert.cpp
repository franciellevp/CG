#include "Alert.h"

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

Alert::Alert () : Widget()
{
    Create();
}

Alert::~Alert() {}

Alert::Alert(int x, int y, float w, float h, const string txt, int type, int isActive)
    : Widget(), txt(txt), type(type), isActive(isActive)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    if (type == Utils::WARNING)
        this->bgColor = Utils::RGBtoFloat(255,255,0);
    else
        this->bgColor = Utils::RGBtoFloat(255, 250, 250);
}

void Alert::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    vector<float> color = Utils::RGBtoFloat(28, 28, 28);;
    CV::color(color[0], color[1], color[2]);
    int c = 5 * txt.size();
    CV::text(x-c+width/2, y+height/2, txt.c_str()); //escreve o label do botao mais ou menos ao centro.

    vector<float> bg = Utils::RGBtoFloat(128,128,128);
    vector<float> co = Utils::RGBtoFloat(255, 250, 250);
    CV::color(bg[0], bg[1], bg[2]);
    int xb = x+width/2 - wb/2;
    int yb = y+height/1.5;
    CV::rectFill(xb, yb, xb+wb, yb+hb);
    CV::color(co[0], co[1], co[2]);
    CV::text(xb+18, yb+15, "OK");
}

void Alert::RenderWidgets()
{
    if (this->isActive)
        alert->Render();
}

void Alert::CheckState(int state, int x, int y)
{
    if(state == 0) {
        if(alert->Colidiu(x, y)) {
            cout << "Clicou no alert " << endl;
            this->isActive = FALSE;
        }
    }
}

void Alert::Create()
{
    alert = new Alert(350, 300, 500, 200, "Arquivo BMP nao tem largura multipla de 4", Utils::WARNING, FALSE);

    this->isActive = 1;
}

// testa se colidiu com o botao do alert
bool Alert::Colidiu(int mx, int my)
{
    wb = 50;
    hb = 30;
    int xb = x+width/2 - wb/2;
    int yb = y+height/1.5;
    if(mx >= xb && mx <= (xb + wb) && my >= y && my <= (yb + hb))
        return true;
    return false;
}