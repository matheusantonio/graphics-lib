#ifndef DEF_VECTOR
#define DEF_VECTOR

#include "Image.h"

// Estrutura para representar um vetor de duas dimensões
struct vec2{
    float x, y;
}; 

// Estrutura para representar um vetor de três dimensões
struct vec3{
    float x, y, z;
};

struct vec4{
    float x, y, z, w;
};

//==================================
// PLANO


// Operação de soma entre vetores de duas dimensões
vec2 operator+(vec2 u, vec2 v);

// Operação de multiplicação entre um vetor de duas dimensões e um escalar
vec2 operator*(float a, vec2 u);


// TRIANGULOS
// Cálculo da área de um triângulo
float areaTri(vec2 p0, vec2 p1, vec2 p2);

// Cálculo das coordenadas baricêntricas de um triângulo
void barycentric(vec2 p, vec2 T[3], float b[3]);

// 
vec2 decasteljau(vec2 P[4], float t);


// TRANSLAÇÃO, ROTAÇÃO, ETC
// Translação de um ponto P para um vetor v
vec2 translate(vec2 P, vec2 v);

// Translação de um vetor de pontos P para um vetor v
void translateV(vec2 P[], int N, vec2 v, vec2 R[]);

// Multiplicação de uma matriz no R2 por um ponto P
vec2 multMP2(float M[2][2], vec2 P);

// Multiplicação de uma matriz no R2 por um vetor de pontos P
void multMV2(float M[2][2], vec2 P[], int N, vec2 R[]);

// Multiplicação de duas matrizes no R2
void multMM2(float M1[2][2], float M2[2][2], float M[2][2]);

//==================================
// ESPAÇO

// Operação de soma de dois vetores no R3
vec3 operator+(vec3 u, vec3 v);

// Operação de multiplicação de um vetor no R3 por um escalar
vec3 operator*(float a, vec3 v);

// Diferença entre vetores no R3
vec3 operator-(vec3 u, vec3 v);

// Produto interno entre vetores no R3
float dot(vec3 u, vec3 v);

float dot(vec4 u, vec4 v);

// Norma de um vetor no R3
float norm(vec3 u);

// Retorna o vetor normalizado no R3
vec3 normalize(vec3 u);

// Produto vetorial no R3
vec3 cross(vec3 u, vec3 v);




// VETORES 3D COM REFERENCIAL (VEC4)
// Soma de vec4
vec4 operator+(vec4 u, vec4 v);

vec4 operator-(vec4 u, vec4 v);

// multiplicação por escalar de vec4
vec4 operator*(float a, vec4 v);

bool operator!=(vec4 u, vec4 v);

// Conversão de vec4 para vec3
vec3 vec4to3(vec4 u);



vec3 toScreen(Image img, vec4 P);

#endif