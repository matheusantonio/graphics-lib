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
float par=1;

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
            s.P[ij] = {u, v, cos(par)+sin(u*v/4), 1};
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

//float c0(float s){
vec3 c0(float s){
    return {15*cos(s), 10*sin(0.1*s), 0.8*sin(6*s+2.5)};
    //return -s*s+3;
}

//float c1(float s){
vec3 c1(float s){
    return {15*cos(s+0.2), 10*sin(0.1*s+1), -0.332917*sin(6*s+2.5)};
    //return cos(s);
}

//float d0(float t){
vec3 d0(float t){
    return {15*cos(0.2*t), 10*sin(t), 0.478778*cos(2*t)};
    //return -t*t-3;
}

//float d1(float t){
vec3 d1(float t){
    return {15*cos(0.2*t+1), 10*sin(t+0.1), 2*(1-t)*t+0.63879*cos(2*t)};
    //return 2*t*t;
}

vec3 Lc(float s, float t){
    return (1-t)*c0(s) + t*c1(s);
}

vec3 Ld(float s, float t){
    return (1-s)*d0(t) + s*d1(t);
}

//float b(float s, float t){
vec3 b(float s, float t){
    return (1-s)*(1-t)*c0(0) + s*(1-t)*c0(1) + (1-s)*t*c1(0) + s*t*c1(1);
}

float factorial(float n){
    float res=1;
    for(int i = n;i>1;i--){
        res*=i;
    }
    return res;
}

float bni(int n, int i, float u){
    return (factorial(n)/(factorial(i)*factorial(n-i)))*pow(u, i)*pow(1-u, n-i);
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

Vertex initSuper(const char* filename, mat4 Model, Color *C){

    super S = lerObj(filename);
    
    Vertex V;
    V.N = S.numVertices;
    V.Ni = 3*S.numFaces;
    
    //V.P = (vec4*)malloc(V.N*sizeof(vec4));

    V.C = (Color*)malloc(V.N*sizeof(Color));
    V.P = getVertices(S, C, V.C);
    //V.indices = (int*)malloc(V.Ni*sizeof(int));
    V.indices = getIndices(S);
    V.ModelMatrix = Model;

    for(int i=0;i<V.N;i++){
        //V.C[i] = {i*255/V.N, i*120/V.N, i*230/V.N};
        //V.C[i] = c_red();
    }

    return V;
}

Vertex initCoons(mat4 Model){
    
    Vertex c;

    int m=15, n=15;
    c.N = m*n;
    float u0 = 0, u1 = 1;
	float du = (u1-u0)/(m-1);
	
	float v0 = 0, v1 = 1;
	float dv = (v1-v0)/(n-1);

    c.P = (vec4*)malloc(c.N*sizeof(vec4));
    c.C = (Color*)malloc(c.N*sizeof(Color));

    for(int i =0;i<m;i++){
        for(int j=0;j<n;j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m;
            vec3 uv = Lc(u, v) + Ld(u, v) - b(u, v);
            c.P[ij] = {uv.x, uv.y, uv.z, 1};
            //cout << c.P[ij].x << ", "<< c.P[ij].y << ", "<< c.P[ij].z << endl;
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

Vertex initBezier(mat4 Model, vec4 P[]){
    
    Vertex c;

    int m = 20, n=20;
    c.N = m*n;

    float u0 = -5, u1 = 5;
	float du = (u1-u0)/(m-1);
	
	float v0 = -5, v1 = 5;
	float dv = (v1-v0)/(n-1);

    c.P = (vec4*)malloc(c.N*sizeof(vec4));
    c.C = (Color*)malloc(c.N*sizeof(Color));

    for(int i =0;i<m;i++){
        for(int j=0;j<n;j++){
            float u = u0 + i*du;
            float v = v0 + j*dv;
            int ij = i + j*m;
            //float uv = bez(i, j, 4, 4, P);
            //c.P[ij] = {u, v, uv, 1};
            c.P[ij] = bez(i, j, 4, 4, P);
            c.C[ij] = bilinear((float)i/m, (float)j/n, {0,0,0},{0,100,0},{34,139,34},{173,255,47});
        }
    }



}

void desenha(Vertex* models, int nModels){

    mat4 View = lookAt({0,0,10}, {0,0,0}, {0,1,0});
    mat4 Projection = perspective(50,(float)Img.width/Img.height, 1, 50);

    for(int i=0;i<nModels; i++){

        vec4 MP[models[i].N];
    
        mat4 M = Projection*View*models[i].ModelMatrix;
        multMV4(M, models[i].P, models[i].N, MP);

        for(int j =0;j<4;j++){
            //cout << models[i].ModelMatrix.M[j][0] <<", " << models[i].ModelMatrix.M[j][1] <<", " << models[i].ModelMatrix.M[j][2] <<", " << models[i].ModelMatrix.M[j][3] << endl;
        }

        for(int j =0;j<models[i].N;j++){
            //cout << MP[j].x/MP[j].w <<", " << MP[j].y/MP[j].w <<", " << MP[j].z/MP[j].w <<", " << MP[j].w << endl;
            //cout << models[i].P[j].x <<", " << models[i].P[j].y <<", " << models[i].P[j].z <<", " << models[i].P[j].w << endl;
        }

        //cout << models[i].Ni << endl;

        draw_elements_triangles(Img, MP, models[i].indices, models[i].Ni, models[i].C);  
    }

    char* filename = (char*)malloc(25*sizeof(char));
    sprintf(filename, "figuras/scene%f.png", par);
    savePNG(filename, Img);

}


int main()
{
    for(par=1;par<10;par++){
        Img = newImage(800,600, true);
        initImage(Img, {0,191,255});
        
        int nModels = 3;
        Vertex *models = (Vertex*)malloc(nModels*sizeof(Vertex));

        Color colorBoat[4] = {
            {160,82,45},
            {139,69,19},
            {210,105,30},
            {160,82,45}
        };

        models[1] = initSurface(scale(1.6,1,1)*rotate_x(-1.3)*translate(0,-3,0));
        //models[0] = initCoons(rotate_x(-0.8)*rotate_y(M_PI)*rotate_z(M_PI)*translate(-17,-10,3));
        models[0] = initCoons(rotate_x(-0.8)*rotate_z(M_PI)*translate(-7,-5,3));
        //models[1] = initCoons(scale(0.05, 0.05, 0.05));
        //models[2] = initCube();
        //models[1] = initSuper("bunny.obj", scale(20, 20, 1)*rotate_x(-0.2)*rotate_y(-0.1)*translate(0, -0.2, 1));
        //models[2] = initSuper("teapot2.obj", scale(1, 1, 1)*rotate_x(-0.2)*rotate_y(-0.1));
        models[2] = initSuper("untitled.obj", scale(0.5, 0.5, 0.5)*rotate_z(0.4)*rotate_y(0.5)*rotate_x(0.2)*translate(0,cos(par)+-1,0), colorBoat);

        desenha(models, nModels);

        freeImage(Img);


    }

    return 0;
}