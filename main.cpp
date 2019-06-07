#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"

int main()
{
    Image Img = newImage(500,500, true);
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
    mat4 Model = rotate_y(0.3)*rotate_x(0.2)*translate(0, 0, -0.5);
    // View Matrix
    //mat4 View = translate(-1, -1, -2);
    mat4 View = lookAt({1,1,1}, {0,0,0}, {0,2,0});
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

    Model = rotate_y(0.5)*rotate_x(0.2)*translate(0, 0, -0.5);
    M = Projection*View*Model;
    multMV4(M, P2, 8, MP);

    draw_elements_lines(Img, MP, indices, 24, c_green());
  */  

    int m=50, n=50;
    int N = m*n;
    float
    u0 = 0, u1 = 1, du = (u1-u0)/(m-1),
    v0 = 0, v1 = 1, dv = (v1-v0)/(n-1);
    
    vec4 P[N];
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            P[i + j*m] = {u, v, sin(u*v/4), 1};
        }
    }

    int Ni = 6*m*n;
    int indices[Ni];
    int pos=-1;
    for(int i=0;i<m-2;i++){
        for(int j =1;j<n-1;j++){
            indices[pos++] = i+j*m;
            indices[pos++] = i+j*m+1;
            indices[pos++] = i+j*m+i;
            indices[pos++] = i+j*m+i+1;
        }
    }
    
    vec4 MP[n*m];
    mat4 Model = rotate_y(0.2)*rotate_x(0.3)*translate(-0.5, -0.3, -0.1);
    mat4 View = lookAt({1,3,2}, {0,0,0}, {0,2,0});
    mat4 Projection = perspective(45,Img.width/(float)Img.height , 0.1, 10);

    mat4 M = Projection*View*Model;
    multMV4(M, P, n*m, MP);

    draw_line_strip(Img, P, N, c_blue());
//    draw_elements_lines(Img, MP, indices, Ni, c_blue());
    
    savePNG("figuras/woreframe3d.png", Img);
    freeImage(Img);

    return 0;
}