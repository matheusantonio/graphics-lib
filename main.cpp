#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"

int main()
{
    Image Img = newImage(500,500, true);
    initImage(Img, {0,100,0});
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
    mat4 Model = rotate_y(0.2)*rotate_x(0.1)*translate(-0.5, -0.5, -0.5);
    // View Matrix
    mat4 View = translate(0, 0, -2);
    //mat4 View = lookAt({1,1,1}, {0,0,0}, {0,2,0});
    // Projection Matrix:
    //mat4 Projection = frustum(-2,2, -2,2, -2,4);
    //mat4 Projection = orthogonal(-2, 2, -2, 2, -3, 3)*perspective(50,Img.width/(float)Img.height , 0.1, 10);
    //mat4 Projection = orthogonal(-2, 2, -2, 2, -2, 2);
    mat4 Projection = perspective(45, Img.width/(float)Img.height, 0.1, 10);
    // MVP Matrix
    mat4 M = Projection*View*Model;
    vec4 MP[8];
    multMV4(M, P, 8, MP);

    for(int i=0;i<8;i++){
        //cout << MP[i].x/MP[i].w << ", " << MP[i].y/MP[i].w << ", " << MP[i].z/MP[i].w << " | "<< MP[i].w<< endl;
    } 


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
 



 
    int m=50, n=30;
    int N = m*n;
    float u0 = -7, u1 = 7;
	float du = (u1-u0)/(m-1);
	
	float v0 = -5, v1 = 5;
	float dv = (v1-v0)/(n-1);

    vec4 P[m*n];
    Color C[n*m];
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m;
            P[ij] = {u, v, sin(u*v/4), 1};
            C[ij] = bilinear((float)i/m, (float)j/n, {135,206,250}, {0,191,255}, {240,248,255}, {176,224,230});
            //C[ij] = c_green();
            
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
            
            /*indices[k++] = ij;
            indices[k++] = ij+1;
            indices[k++] = ij;
            indices[k++] = ij+m;
            indices[k++] = ij+m;
            indices[k++] = ij+1;
            indices[k++] = ij+m;
            indices[k++] = ij+m+1;
            indices[k++] = ij+m+1;
            indices[k++] = ij+1;*/
            
            
            
            
		}
    }
    
    int N2=8;
    int Ni2=12*3;

// Vertices de um cubo
    vec4 P2[8] = {
        {0, 0, 0, 1}, {2, 0, 0, 1}, {2, 1, 0, 1}, {0, 1, 0, 1},
        {0, 0, 1, 1}, {2, 0, 1, 1}, {2, 1, 1, 1}, {0, 1, 1, 1},
    };

    Color C2[8] = {
        {165,42,42},
        {255,140,0},
        {255,255,0},
        {139,69,19},
        {160,82,45},
        {210,105,30},
        {222,184,135},
        {240,230,140},
    };
    // Arestas do cubo
    int indices2[12*3] = {
        0, 1, 4,
        1, 4, 5,
        1, 2, 5,
        2, 5, 6,
        3, 2, 7,
        2, 7, 6,
        0, 3, 4,
        3, 4, 7,
        4, 5, 7,
        5, 6, 7,
        0, 1, 3,
        1, 2, 3,
    };

    vec4 MP[N];
    mat4 Model = scale(1.5,1,1)*rotate_x(0.1)*translate(5,-0.5,-0.5);
    mat4 View = lookAt({10,10,10}, {0,0,0}, {0,0,1});
    //mat4 Projection = frustum(-2,2,-2,2,-3,-6);
    mat4 Projection = perspective(50,(float)Img.width/Img.height, 0.1, 10);
    //mat4 Projection = orthogonal(-2, 2, -2, 2, -2, 2);

    mat4 M = Projection*View*Model;
    multMV4(M, P, N, MP);

    for(int i=0;i<8;i++){
        cout << MP[i].x/MP[i].w << ", " << MP[i].y/MP[i].w << ", " << MP[i].z/MP[i].w << " | " << MP[i].w << endl;
    }

    //draw_elements_lines(Img, MP, indices, Ni, c_blue());    
    draw_elements_triangles(Img, MP, indices, Ni, C);

    vec4 MP2[N2];
    mat4 Model2 = rotate_y(0.2)*rotate_x(0.1)*translate(-0.5,-0.5,-0.5);
    mat4 View2 = lookAt({1,1,1}, {0,0,0}, {0,0,1});
    //mat4 Projection = frustum(-2,2,-2,2,-3,-6);
    //mat4 Projection = perspective(50,(float)Img.width/Img.height, 0.1, 10);
    mat4 Projection2 = orthogonal(-2, 2, -2, 2, -2, 2);

    mat4 M2 = Projection2*View2*Model2;
    multMV4(M2, P2, N2, MP2);

    draw_elements_triangles(Img, MP2, indices2, Ni2, C2);


    //char filename[20];

    //sprintf(filename, "figuras/barco0%d.png", p);

    savePNG("figuras/barcoimage.png", Img);

    freeImage(Img);

    return 0;
}