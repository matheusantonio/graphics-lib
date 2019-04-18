#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"

using namespace std;

int main()
{
    Image Img = newImage(800,600);
    initImage(Img, c_white());

    vec2 P[] = {
        { 60, 105},
        {229, 114},
        {145, 270},
        {364, 208},
        {283, 333},
        {471, 298}
    };
    Color C[] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 0, 255},
        {0, 255, 255},
        {0, 0, 255}
    };
    draw_triangles(Img, P, 6, C);    

    savePNG("figuras/triangulos_coloridos.png", Img);
    freeImage(Img);

    return 0;
}