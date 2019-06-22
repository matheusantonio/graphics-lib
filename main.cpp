#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"
#include "Matrix.h"
#include "Surface.h"
#include <math.h>

float c0(float s){
    return 2*s*s -s;
}

float c1(float s){
    return 4*s*s + s;
}

float d0(float t){
    return cos(t);
}

float d1(float t){
    return sin(t);
}

float Lc(float s, float t){
    return (1-t)*c0(s) + t*c1(s);
}

float Ld(float s, float t){
    return (1-s)*d0(t) + s*d1(t);
}

float b(float s, float t){
    return c0(0)*(1-s)*(1-t) + c0(1)*s*(1-t) + c0(0)*(1-s)*t + c0(1)*s*t;
}

float factorial(float n){
    float res=1;
    for(int i = n;i>1;i--){
        res*=i;
    }
    return res;
}

float bni(int n, int i, float u){
    return factorial(n)/(factorial(i)*factorial(n-i))*pow(u, i)*pow(1-u, n-i);
}

vec4 bez(float u, float v, int n, int m, vec4 P[]){
    vec4 zeros = {0,0,0,1};
    vec4 res={0,0,0,0};
    for(int i=0;i<=n;i++){
        for(int j=0;j<=m;j++){
            res = res + bni(n, i, u)*bni(m, j, v)*P[i + j*m];
        }
    }
    return res;
}

int main()
{
    Image Img = newImage(500,500, true);
    initImage(Img, {0,191,255});

    int m=50, n=50;
    int N = m*n;
    float u0 = -5, u1 = 5;
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
            C[ij] = bilinear((float)i/m, (float)j/n, {32,178,170}, {30,144,255},{25,25,112}, {0,0,128});
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
    mat4 Model = scale(1.6,1,1)*rotate_x(-0.5)*translate(0,-2.5,0);
    mat4 View = lookAt({0,0,15}, {0,0,0}, {0,1,0});
    mat4 Projection = perspective(50,(float)Img.width/Img.height, 1, 50);

    mat4 M = Projection*View*Model;
    multMV4(M, P, N, MP);

    //draw_elements_lines(Img, MP, indices, Ni, c_blue());    
    draw_elements_triangles(Img, MP, indices, Ni, C);

    vec4 MP2[N2];
    mat4 Model2 = rotate_y(0.2)*rotate_x(0.1)*translate(-0.5,-0.5,-0.5);

    mat4 M2 = Projection*View*Model2;
    multMV4(M2, P2, N2, MP2);

    //draw_elements_triangles(Img, MP2, indices2, Ni2, C2);


    //============
    // Tentativa de desenhar o barco com Coons
    int m3=30, n3=30;
    int N3 = m3*n3;
    u0 = -5;
    u1 = 5;
	du = (u1-u0)/(m-1);
	
	v0 = -5;
    v1 = 5;
	dv = (v1-v0)/(n-1);

    vec4 Pc[16] = {
        {1,1,1,1}, {1,2,4,1}, {1,0,1,1}, {1,2,0,1},
        {1,1,3,1}, {1,2,1,1}, {1,1,3,1}, {0,2,4,1},
        {1,2,1,1}, {2,2,4,1}, {2,1,1,1}, {1,2,2,1},
        {2,1,1,1}, {1,2,4,1}, {1,1,1,1}, {1,3,4,1}
    };

    vec4 P3[N3];
    Color C3[N3];
    for(int i =0;i<m3;i++){
        for(int j=0;j<n3;j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m3;
            float uv = Lc(u, v) + Ld(u, v) - b(u, v);
            P3[ij] = {u, v, uv, 1};
            //P3[ij] = bez(u, v, n3, m3, Pc);
            //cout << u << ", " << v << ", " << uv << endl;
            C3[ij] = bilinear((float)i/m3, (float)j/n3, {0,0,0},{0,100,0},{34,139,34},{173,255,47});
        }
    }

    int Ni3 = 6*(m3-1)*(n3-1);
    int indices3[Ni3];
    int k3=0;
    for(int i = 0; i < m3-1; i++){
		for(int j = 0; j < n3-1; j++){	
			int ij = i + j*m3;
            
			indices3[k3++] = ij;
			indices3[k3++] = ij+1;
			indices3[k3++] = ij+m3;
			
			indices3[k3++] = ij+m3+1;
			indices3[k3++] = ij+m3;
			indices3[k3++] = ij+1;       
		}
    }

    vec4 MP3[N3];
    mat4 Model3 = scale(0.5, 0.5, 1);

    mat4 M3 = Projection*View*Model3;
    multMV4(M3, P3, N3, MP3);

    //draw_elements_triangles(Img, MP3, indices3, Ni3, C3);

    super S = lerObj("wavefront.obj");
    
    int N4 = S.numVertices;
    int Ni4 = 3*S.numFaces;

    vec4* P4;
    P4 = getVertices(S);
    int* indices4;
    indices4 = getIndices(S);

    vec4 MP4[N4];
    mat4 Model4 = rotate_x(-0.2)*rotate_y(-0.1)*scale(1.5, 1.5, 1);

    mat4 M4 = Projection*View*Model4;
    multMV4(M4, P4, N4, MP4);

    Color C4[N4];
    for(int i=0;i<N4;i++){
        C4[i] = c_blue();
    }

    draw_elements_triangles(Img, MP4, indices4, Ni4, C4);



    savePNG("figuras/testebarcocoons.png", Img);

    freeImage(Img);

    return 0;
}