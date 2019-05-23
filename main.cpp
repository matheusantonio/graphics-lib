#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"

int main()
{
    Image Img = newImage(500,500);
    initImage(Img, c_white());

/*
    // Vertices de um cubo
    vec4 P[8] = {
        {0, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 0, 1}, {0, 1, 0, 1},
        {0, 0, 2, 1}, {1, 0, 2, 1}, {1, 1, 2, 1}, {0, 1, 2, 1},
    };
    // Arestas do cubo
    int indices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6,
        6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7
    };

    // Model Matrix
    mat4 Model = rotate_y(180)*rotate_x(90)*rotate_z(10)*translate(0, 0, -0.5);
    // View Matrix
    //mat4 View = translate(-1, -1, -2);
    mat4 View = lookAt({1,3,2}, {0,0,0}, {0,2,0});
    // Projection Matrix:
    //mat4 Projection = frustum(-2,2, -2,2, -2,-4);
    mat4 Projection = perspective(45,Img.width/(float)Img.height , 0.1, 10);
    //mat4 Projection = orthogonal(-2, 2, -2, 2, -2, 2);
    // MVP Matrix
    mat4 M = Projection*View*Model;
    vec4 MP[8];
    multMV4(M, P, 8, MP);

    draw_elements_lines(Img, MP, indices, 24, c_blue());

    vec4 P2[8] = {
        {0, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 0, 1}, {0, 1, 0, 1},
        {0, 0, 0.2, 1}, {1, 0, 0.2, 1}, {1, 1, 0.2, 1}, {0, 1, 0.2, 1},
    };

    Model = rotate_z(-20)*rotate_y(0.5)*rotate_x(2.6)*translate(0, 0, -0.5);
    M = Projection*View*Model;
    multMV4(M, P2, 8, MP);

    draw_elements_lines(Img, MP, indices, 24, c_green());
    */
    vec2 P[12] = {
        {10,450}, {250,300}, 
        {240,310}, {230,230}, 
        {230,230}, {430,430}, 
        {230,230}, {400,235},
        {400,235}, {320, 320}, 
        {400,235}, {320,90}
    };

    vec2 PCubo[4] = {
        {100,100}, {100,400},{400,400},{400,100}
    };

    draw_line_loop(Img, PCubo, 4, c_blue());

    draw_lines(Img, P, 12, c_red());

  
    savePNG("figuras/testerecorte_3d.png", Img);
    freeImage(Img);

    return 0;
}