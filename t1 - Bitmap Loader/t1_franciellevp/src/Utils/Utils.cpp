//*************************************************************
//
// classe que contem funcoes auxiliares utilizadas na aplicacao
//
//*************************************************************
#include "../Canvas/gl_canvas2d.h"
#include "Utils.h"
#include <unistd.h>
#include <iostream>

using namespace std;

/* Metodo estatico auxiliar ao RGBtoFloat
   @param i: numero equivalente a RED, GREEN ou BLUE
   @return : ponto flutuante equivalente a componente RGB
*/
static float calcRtoF (float i)
{
    float maxRGB = 255;
    return (i / maxRGB);
}

/* Converte RGB para float
   @param r: inteiro equivalente a RED
   @param g: inteiro equivalente a GREEN
   @param b: inteiro equivalente a BLUE
*/
vector<float> Utils::RGBtoFloat (int r, int g, int b)
{
    vector<float> rgb;
    rgb.push_back(calcRtoF(r));
    rgb.push_back(calcRtoF(g));
    rgb.push_back(calcRtoF(b));
    return rgb;
}

/* Pega o caminho fisico em que os arquivos se encontram
   @return path: retorna todo o endereco fisico em uma string
*/
string Utils::getCurrentPath() {
    char buf[100];
    getcwd(buf, sizeof(buf));
    string path = buf;
    return path;
}

/* Pega o caminho fisico que a imagem desejada se encontra
   @param filename: nome da imagem bmp
   @return path: endereco fisico que a imagem se encontra
*/
string Utils::getImagePath (const char* filename) {
    string path = getCurrentPath();
    const char* aux;
    #ifdef WIN32
        aux = "\\t1_franciellevp\\resources\\";
        path += aux;
    #else
        aux = "/t1_franciellevp/resources/";
        path += aux;
    #endif
    path += filename;
    return path;
}
