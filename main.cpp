#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"

using namespace std;

void print_matrix(mat4 M){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            cout << M.M[i][j] << ", " << ends;
        }
        cout << endl;
    }
    cout << endl;
}

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
    mat4 Projection = orthogonal(-2,2, -2,2, -2,2);
    // MVP Matrix
    mat4 M = Projection*View*Model;
    vec4 MP[8];
    multMV4(M, P, 8, MP);

    cout<< "Matrizes: "<< endl;
    print_matrix(Model);
    print_matrix(View);
    print_matrix(Projection);
    print_matrix(M);
    
    cout << "vec4 resultante: " << endl;
    for(int i=0;i<8;i++){
        cout << MP[i].x << ", " << MP[i].y << ", " << MP[i].z << ", " << MP[i].w << endl;
    }

    draw_elements_lines(Img, MP, indices, 24, c_blue());
    

    //vec4 MP[8];
    //multMV4(M, P, 8, MP);
    //draw_elements_lines(Img, MP, indices, 24, c_blue());    

    savePNG("figuras/cubo_3d.png", Img);
    freeImage(Img);

    return 0;
}