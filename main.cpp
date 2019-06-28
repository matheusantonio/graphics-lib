#include <iostream>
#include "Color.h"
#include "Vector.h"
#include "Desenho.h"
#include "Image.h"
#include "Complexos.h"
#include "Matrix.h"
#include "Surface.h"
#include <math.h>

struct Vertex{
    vec4* P;
    int N;
    int* indices;
    int Ni;
    mat4 ModelMatrix;
    Color* C;
};

Image Img;

Vertex initSurface(mat4 Model){

    Vertex s;

    int m=50, n=50;
    s.N = m*n;
    float u0 = -5, u1 = 5;
	float du = (u1-u0)/(m-1);
	
	float v0 = -5, v1 = 5;
	float dv = (v1-v0)/(n-1);

    s.P = (vec4*)malloc(s.N*sizeof(Vertex));
    s.C = (Color*)malloc(s.N*sizeof(Color));
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m;
            s.P[ij] = {u, v, sin(u*v/4), 1};
            s.C[ij] = bilinear((float)i/m, (float)j/n, {32,178,170}, {30,144,255},{25,25,112}, {0,0,128});
        }
    }

    s.Ni = 6*(m-1)*(n-1);
    s.indices = (int*)malloc(s.Ni*sizeof(int));
    int k=0;
    for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			int ij = i + j*m;
            
			s.indices[k++] = ij;
			s.indices[k++] = ij+1;
			s.indices[k++] = ij+m;
			
			s.indices[k++] = ij+m+1;
			s.indices[k++] = ij+m;
			s.indices[k++] = ij+1;
		}
    }

    vec4 MP[s.N];
    s.ModelMatrix = Model;
    
    return s;
}

/*Vertex initCube(){
    
    Vertex cube;
    
    cube.N=8;
    cube.Ni=12*3;

    cube.P = (vec4*)malloc(cube.N*sizeof(vec4));
    cube.P = {
        {0, 0, 0, 1}, {2, 0, 0, 1}, {2, 1, 0, 1}, {0, 1, 0, 1},
        {0, 0, 1, 1}, {2, 0, 1, 1}, {2, 1, 1, 1}, {0, 1, 1, 1},
    };

    cube.C = (Color*)malloc(cube.N*sizeof(Color));
    cube.C = {
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
    cube.indices = (int*)malloc(cube.Ni*sizeof(int));
    cube.indices = {
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

    cube.ModelMatrix = rotate_y(0.2)*rotate_x(0.1)*translate(-0.5,-0.5,-0.5);

    
    return cube;
}*/

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

Vertex initSuper(const char* filename, mat4 Model){

    super S = lerObj(filename);
    
    Vertex V;
    V.N = S.numVertices;
    V.Ni = 3*S.numFaces;
    
    //V.P = (vec4*)malloc(V.N*sizeof(vec4));
    V.P = getVertices(S);
    //V.indices = (int*)malloc(V.Ni*sizeof(int));
    V.indices = getIndices(S);
    V.ModelMatrix = Model;

    V.C = (Color*)malloc(V.N*sizeof(Color));
    for(int i=0;i<V.N;i++){
        V.C[i] = {i*255/V.N, i*120/V.N, i*230/V.N};
        //V.C[i] = c_red();
    }

    return V;
}

Vertex initCoons(mat4 Model){
    
    Vertex c;

    int m=30, n=30;
    c.N = m*n;
    int u0 = -5;
    int u1 = 5;
	int du = (u1-u0)/(m-1);
	
	int v0 = -5;
    int v1 = 5;
	int dv = (v1-v0)/(n-1);

    c.P = (vec4*)malloc(c.N*sizeof(vec4));
    c.C = (Color*)malloc(c.N*sizeof(Color));

    for(int i =0;i<m;i++){
        for(int j=0;j<n;j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m;
            float uv = Lc(u, v) + Ld(u, v) - b(u, v);
            c.P[ij] = {u, v, uv, 1};
            //P3[ij] = bez(u, v, n3, m3, Pc);
            c.C[ij] = bilinear((float)i/m, (float)j/n, {0,0,0},{0,100,0},{34,139,34},{173,255,47});
        }
    }

    c.Ni = 6*(m-1)*(n-1);
    c.indices=(int*)malloc(c.Ni*sizeof(int));
    int k=0;
    for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			int ij = i + j*m;
            
			c.indices[k++] = ij;
			c.indices[k++] = ij+1;
			c.indices[k++] = ij+m;
			
			c.indices[k++] = ij+m+1;
			c.indices[k++] = ij+m;
			c.indices[k++] = ij+1;       
		}
    }
    c.ModelMatrix = Model;

    return c;
}

void desenha(Vertex* models, int nModels){

    mat4 View = lookAt({0,0,15}, {0,0,0}, {0,1,0});
    mat4 Projection = perspective(50,(float)Img.width/Img.height, 1, 50);

    for(int i=0;i<nModels; i++){

        vec4 MP[models[i].N];
    
        mat4 M = Projection*View*models[i].ModelMatrix;
        multMV4(M, models[i].P, models[i].N, MP);

        for(int j =0;j<4;j++){
            //cout << models[i].ModelMatrix.M[j][0] <<", " << models[i].ModelMatrix.M[j][1] <<", " << models[i].ModelMatrix.M[j][2] <<", " << models[i].ModelMatrix.M[j][3] << endl;
        }

        for(int j =0;j<4;j++){
            //cout << MP[i].x <<", " << MP[i].y <<", " << MP[i].z <<", " << MP[i].w << endl;
        }
        

        draw_elements_triangles(Img, MP, models[i].indices, models[i].Ni, models[i].C);  
    }

    savePNG("figuras/testebarcocoons.png", Img);

}


int main()
{
    Img = newImage(500,500, true);
    initImage(Img, {0,191,255});
    
    int nModels = 4;
    Vertex *models = (Vertex*)malloc(nModels*sizeof(Vertex));

    models[0] = initSurface(scale(1.6,1,1)*rotate_x(-0.5)*translate(0,-2.5,0));
    models[1] = initCoons(loadIdentity());
    //models[2] = initCube();
    models[2] = initSuper("bunny.obj", scale(20, 20, 1)*rotate_x(-0.2)*rotate_y(-0.1)*translate(0, -0.2, 1));
    models[3] = initSuper("teapot2.obj", scale(1, 1, 1)*rotate_x(-0.2)*rotate_y(-0.1));

    desenha(models, nModels);

    freeImage(Img);

    return 0;
}