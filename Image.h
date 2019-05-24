//#pragma once
#ifndef DEF_IMAGE
#define DEF_IMAGE

#include "Color.h"

struct Image{
    int width, height;
    Color* data;
    float* zBuffer;
};

void flip_image();

// Inicializa uma imagem
Image newImage(int w, int h);

// Limpa o espaço reservado de uma imagem
void freeImage(Image img);

// Retorna a cor de um pixel na imagem
Color* pixel(Image I, int x, int y);

// Retorna o valor do zbuffer no ponto dado
float* zbuffer(Image I, int x, int y);

// Inicializa todas as posições do zbuffer com -1
void initZBuffer(Image img);

// Inicializa a imagem com uma determinada cor
void initImage(Image img, Color color);

// Salva o conteúdo da imagem em um arquivo .png
void savePNG(const char* filename, Image img);

#endif