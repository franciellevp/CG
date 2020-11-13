//************************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
//
//************************************************************

#include "Bmp.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Controller/Controller.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

Bmp::Bmp(string fileName)
{
    width = height = 0;
    data = NULL;
    if(!fileName.empty() && fileName.size() > 0)
        load(fileName.c_str());
    else
        cout << "Error: Invalid BMP filename" << endl;
    convertBGRtoRGB();
}

uchar* Bmp::getImage()
{
    return data;
}

int Bmp::getWidth(void)
{
    return width;
}

int Bmp::getHeight(void)
{
    return height;
}

void Bmp::convertBGRtoRGB()
{
    unsigned char tmp;
    if( data != NULL ) {
        for(int y=0; y<height; y++)
            for(int x=0; x<width*3; x+=3) {
                int pos = y*bytesPerLine + x;
                tmp = data[pos];
                data[pos] = data[pos+2];
                data[pos+2] = tmp;
            }
    }
}

void Bmp::renderBmp(int px, int py)
{
    int x = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            vector<float> rgb = Utils::RGBtoFloat(data[x], data[x + 1], data[x + 2]);
            CV::color(rgb[0], rgb[1], rgb[2]);
            CV::point(px + j, py + height - i); // img certa
            //CV::point(px + i, py + j); // virado p esquerda
            //CV::point(px + j, py + i); // cabeça p baixo
            //CV::point(px + height - i, py + j); // virado p direita
            x += 3;
        }
    }
}

void Bmp::mirroredX()
{
    int x = 0;
    int y = width - 1;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            swap(data[x], data[y - 2]);
            swap(data[x + 1], data[y - 1]);
            swap(data[x + 2], data[y]);
            x += 3;
            y += 3;
            //swap(data[i * width + j], data[i * width + (width - 1 - j)]);
            //newdt[(w-1) * (i + 1) - j + i]
        }
    }
}

void Bmp::mirroredY()
{
    for (int i = 0; i < height / 2; i++) {
        for (int j = 0; j < width; j++) {
            swap(data[i * width + j], data[(height - 1 - i) * width + j]);
        }
    }
}

void Bmp::setFlip(int value)
{
    flip = value;
}

//le o HEADER componente a componente devido ao problema de alinhamento de bytes. Usando
//o comando fread(header, sizeof(HEADER),1,fp) sao lidos 16 bytes ao inves de 14
void Bmp::readHeader(FILE* fp) {
    fread(&header.type,      sizeof(unsigned short int), 1, fp);
    fread(&header.size,      sizeof(unsigned int),       1, fp);
    fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fread(&header.offset,    sizeof(unsigned int),       1, fp);
    /*fp.read(reinterpret_cast<char*>(&header.type), 2);
    fp.read(reinterpret_cast<char*>(&header.size), 4);
    fp.read(reinterpret_cast<char*>(&header.reserved1), 2);
    fp.read(reinterpret_cast<char*>(&header.reserved2), 2);
    fp.read(reinterpret_cast<char*>(&header.offset), 4);*/
}

//le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
void Bmp::readInfoHeader(FILE* fp) {
    fread(&info.size,        sizeof(unsigned int),       1, fp);
    fread(&info.width,       sizeof(int),                1, fp);
    fread(&info.height,      sizeof(int),                1, fp);
    fread(&info.planes,      sizeof(unsigned short int), 1, fp);
    fread(&info.bits,        sizeof(unsigned short int), 1, fp);
    fread(&info.compression, sizeof(unsigned int),       1, fp);
    fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
    fread(&info.xresolution, sizeof(int),                1, fp);
    fread(&info.yresolution, sizeof(int),                1, fp);
    fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
    fread(&info.impcolours,  sizeof(unsigned int),       1, fp);
    /*fp.read(reinterpret_cast<char*>(&info.size), 4);
    fp.read(reinterpret_cast<char*>(&info.width), 4);
    fp.read(reinterpret_cast<char*>(&info.height), 4);
    fp.read(reinterpret_cast<char*>(&info.planes), 2);
    fp.read(reinterpret_cast<char*>(&info.bits), 2);
    fp.read(reinterpret_cast<char*>(&info.compression), 4);
    fp.read(reinterpret_cast<char*>(&info.imagesize), 4);
    fp.read(reinterpret_cast<char*>(&info.xresolution), 4);
    fp.read(reinterpret_cast<char*>(&info.yresolution), 4);
    fp.read(reinterpret_cast<char*>(&info.ncolours), 4);
    fp.read(reinterpret_cast<char*>(&info.impcolours), 4);*/
}

void Bmp::load(const char* fileName)
{
    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL) {
        cout << endl << "Erro ao abrir arquivo " << fileName << " para leitura" << endl;
        return;
    }
    cout << "\nCarregando arquivo " << fileName << endl;
    readHeader(fp);
    readInfoHeader(fp);
    width  = info.width;
    height = info.height;
    bits   = info.bits;
    //image = new Color[width * height];
    cout << "extra bits " <<  (4 - width % 4) % 4 << endl;
    int extrabytes = (4 - width % 4) % 4;
    bytesPerLine =(3 * (width + 1) / 4) * 4; // Todo: IMPORTANTE
    imagesize    = bytesPerLine * height;
    int delta    = bytesPerLine - (width + 1) * 3;

    cout << "Imagem: " << width << "x" << height << " - Bits: " << bits << endl;
    cout << "bytesPerLine: " << bytesPerLine << endl;
    cout << "bytesPerLine: " << (width + 1) * 3 << endl;
    cout << "delta: " << delta << endl;
    cout << "imagesize: " << imagesize << " " << info.imagesize << endl;

    if( header.type != 19778 ) {
        cout << "Error: Arquivo BMP invalido" << endl;
        getchar();
        exit(0);
    }
    if( (width + 1)*height*3 != imagesize ) {
        cout << width*height*3 << " " << imagesize << endl;
        cout << "Warning: Arquivo BMP nao tem largura multipla de 4" << endl;
        //getchar();
    }
    if( info.compression != 0 ) {
        cout << "Error: Formato BMP comprimido nao suportado" <<  endl;
        getchar();
        return;
    }
    if( bits != 24 ) {
        cout << "Error: Formato BMP com %d bits/pixel nao suportado" << endl;
        getchar();
        return;
    }
    if( info.planes != 1 ) {
        cout << "Error: Numero de Planes nao suportado: " << info.planes << endl;
        getchar();
        return;
    }
    data = new unsigned char[imagesize];
    //fread(data, sizeof(unsigned char), imagesize, fp);
    // iterate over infile's scanlines
    int x = 0;
    for (int i = 0; i < height; i++) {
        // iterate over pixels in scanline
        for (int j = 0; j < width; j++) {
            uchar rgb[3];
            // read RGB triple from infile
            fread(&rgb, sizeof(rgb), 1, fp);
            data[x] = rgb[0];
            data[x+1] = rgb[1];
            data[x+2] = rgb[2];
            x += 3;
        }
        // skip over padding, if any
        fseek(fp, extrabytes, SEEK_CUR);
    }
    fclose(fp);
}

/*

    int x = 0, a = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            image[a].rgb[0] = data[x];
            image[a].rgb[1] = data[x+1];
            image[a].rgb[2] = data[x+2];
            x += 3;
            a++;
        }
    }


    int x = 0;
    cout << "IMPRIMINDO" << endl;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            cout << "(" << (int)data[x] << ", " << (int)data[x+1] << ", " << (int)data[x+2] << ")" << " | ";
            x += 3;
        }
        cout << endl << endl;
    }*/
