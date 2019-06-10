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
        {0, 0, 1, 1}, {1, 0, 1, 1}, {1, 1, 1, 1}, {0, 1, 1, 1},
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
    mat4 Projection = orthogonal(-2, 2, -2, 2, -3, 3)*perspective(45,Img.width/(float)Img.height , 0.1, 10);
    //mat4 Projection = orthogonal(-2, 2, -2, 2, -2, 2);
    // MVP Matrix
    mat4 M = Projection*View*Model;
    vec4 MP[8];
    multMV4(M, P, 8, MP);

    draw_elements_lines(Img, MP, indices, 24, c_blue());
/* 
    vec4 P2[8] = {
        {0, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 0, 1}, {0, 1, 0, 1},
        {0, 0, 0.2, 1}, {1, 0, 0.2, 1}, {1, 1, 0.2, 1}, {0, 1, 0.2, 1},
    };

    Model = rotate_y(0.5)*rotate_x(0.2)*translate(0, 0, -0.5);
    M = Projection*View*Model;
    multMV4(M, P2, 8, MP);

    draw_elements_lines(Img, MP, indices, 24, c_green());
*/

    int m=30, n=20;
    int N = m*n;
    float u0 = 0, u1 = 1;
	float du = (u1-u0)/(m-1);
	
	float v0 = -5, v1 = 5;
	float dv = (v1-v0)/(n-1);

    vec4 P[m*n];
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m;
            P[ij] = {u, v, sin(u*v/4), 1};
            
            
        }
    }

    int Ni = 6*(m-1)*(n-1);
    int indices[Ni];
    int k=0;
    for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			int ij = i + j*m;
			indices[k++] = ij;
			indices[k++] = ij+1;
			indices[k++] = ij+m;
			
			indices[k++] = ij+m+1;
			indices[k++] = ij+m;
			indices[k++] = ij+1;
		}
	}
    
    vec4 MP[n*m];
    mat4 Model = rotate_y(3.14/2);
    mat4 View = lookAt({10,10,10}, {0,0,0}, {0,0,1});
    mat4 Projection = orthogonal(-2, 2, -2, 2, -3, 3)*perspective(50,(float)Img.width/Img.height , 0.1, 10);


    mat4 M = Projection*View*Model;
    multMV4(M, P, N, MP);

    /*for(int i=0;i<N;i++){
        cout << i<<": " << MP[i].x << ", " << MP[i].y << ", " << MP[i].z << endl;
    } */

    draw_elements_lines(Img, MP, indices, Ni, c_blue());    

    savePNG("figuras/wireframe3d.png", Img);
    freeImage(Img);

    return 0;
}