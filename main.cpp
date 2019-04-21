#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"

int main()
{
    Image Img = newImage(1000,1000);
    initImage(Img, c_white());

    // Vertices de um cubo
    vec4 P[8] = {
        {0, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 0, 1}, {0, 1, 0, 1},
        {0, 0, 1, 1}, {1, 0, 1, 1}, {1, 1, 1, 1}, {0, 1, 1, 1},
    };
    // Arestas do cubo
    int indices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6,
        6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7
    };

    // Model Matrix
    mat4 Model = rotate_y(0.2)*rotate_x(0.1)*translate(-0.5, -0.5, -0.5);
    // View Matrix
    mat4 View = translate(0, 0, -2);
    // Projection Matrix:
    mat4 Projection = frustum(-2,2, -2,2, -2,-4);
    // MVP Matrix
    mat4 M = Projection*View*Model;
    vec4 MP[8];
    multMV4(M, P, 8, MP);

    draw_elements_lines(Img, MP, indices, 24, c_blue());

    savePNG("figuras/cubo_3d.png", Img);
    freeImage(Img);

    return 0;
}