#include <iostream>
#include "Color.h"
#include "Image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Image;

void flip_image(){
    stbi_flip_vertically_on_write(1);
}


// Inicializa uma imagem
Image newImage(int w, int h, bool useZbuffer){
    Image img = {w, h};
    img.data = (Color*)malloc(w*h*sizeof(Color));
    img.zBuffer = NULL;
    if(useZbuffer)
        img.zBuffer = (float*)malloc(w*h*sizeof(float));
    return img;
}

// Limpa o espaço reservado de uma imagem
void freeImage(Image img){
    free(img.data);
    free(img.zBuffer);
}

// Retorna a cor de um pixel na imagem
Color* pixel(Image I, int x, int y){
    return I.data + (y*I.width + x);
}

// Retorna o valor do zbuffer no ponto dado
float* zbuffer(Image I, int x, int y){
    return I.zBuffer + (y*I.width + x);
}

// Inicializa todas as posições do zbuffer com -1
void initZBuffer(Image img){
    if(img.zBuffer == NULL){
        return;
    }
    for(int y=0;y<img.height;y++){
        for(int x=0;x<img.width;x++){
            float*zb = zbuffer(img, x, y);
            *zb = -1;
        }
    }
}

// Inicializa a imagem com uma determinada cor
void initImage(Image img, Color color){
    for(int y=0;y<img.height;y++){
        for(int x=0;x<img.width;x++){
            Color* p = pixel(img, x, y);
            *p = color;
        }
    }
}

// Salva o conteúdo da imagem em um arquivo .png
void savePNG(const char* filename, Image img){
    stbi_write_png(filename,
        img.width, img.height, 3,
        img.data, 0);
}