#include <iostream>
#include "Color.cpp"
#include "Vector.cpp"
#include "Desenho.cpp"
#include "Image.cpp"
#include "Complexos.cpp"

using namespace std;

int main()
{
    Image Img = newImage(800,600);
    initImage(Img, c_white);

    // Codigo que manipulará a imagem


    savePNG("output.png", Img);
    freeImage(Img);

    return 0;
}