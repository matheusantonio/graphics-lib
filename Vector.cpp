#include "Vector.h"
#include "Image.h"
#include <cmath>
#include <iostream>

// Estrutura para representar um vetor de duas dimensões
struct vec2;

// Estrutura para representar um vetor de três dimensões
struct vec3;

struct vec4;


//==================================
// PLANO
// Operação de soma entre vetores de duas dimensões
vec2 operator+(vec2 u, vec2 v)
{
    return {u.x+v.x, u.y+v.y};
}

// Operação de multiplicação entre um vetor de duas dimensões e um escalar
vec2 operator*(float a, vec2 u)
{
    return {a*u.x, a*u.y};
}


// TRIANGULOS
// Cálculo da área de um triângulo
float areaTri(vec2 p0, vec2 p1, vec2 p2)
{
    return ((p1.x - p0.x)*(p1.y + p0.y)/2 + 
           (p2.x - p1.x)*(p2.y + p1.y)/2 +
           (p0.x - p2.x)*(p0.y + p2.y)/2);
}

// Cálculo das coordenadas baricêntricas de um triângulo
void barycentric(vec2 p, vec2 T[3], float b[3])
{
    b[0] = areaTri(p, T[1], T[2])/areaTri(T[0], T[1], T[2]);
    b[1] = areaTri(T[0], p, T[2])/areaTri(T[0], T[1], T[2]);
    b[2] = areaTri(T[0], T[1], p)/areaTri(T[0], T[1], T[2]);
}

// 
vec2 decasteljau(vec2 P[4], float t){
    vec2 Q[3] = {
        (1-t)*P[0] + t*P[1],
        (1-t)*P[1] + t*P[2],
        (1-t)*P[2] + t*P[3]
    };
    vec2 R[2] = {
        (1-t)*Q[0] + t*Q[1],
        (1-t)*Q[1] + t*Q[2],
    };
    return (1-t)*R[0] + t*R[1];
}


// TRANSLAÇÃO, ROTAÇÃO, ETC
// Translação de um ponto P para um vetor v
vec2 translate(vec2 P, vec2 v){
    return P + v;
}

// Translação de um vetor de pontos P para um vetor v
void translateV(vec2 P[], int N, vec2 v, vec2 R[]){
    for(int i = 0; i < N; i++)
        R[i] = translate(P[i], v);
}

// Multiplicação de uma matriz no R2 por um ponto P
vec2 multMP2(float M[2][2], vec2 P){
    return {
        M[0][0]*P.x + M[0][1]*P.y,
        M[1][0]*P.x + M[1][1]*P.y
    };
}

// Multiplicação de uma matriz no R2 por um vetor de pontos P
void multMV2(float M[2][2], vec2 P[], int N, vec2 R[]){
    for(int i = 0; i < N; i++)
        R[i] = multMP2(M, P[i]);
}

// Multiplicação de duas matrizes no R2
void multMM2(float M1[2][2], float M2[2][2], float M[2][2]){
    M[0][0] = M1[0][0]*M2[0][0] + M1[0][1]+M2[1][0];
    M[0][1] = M1[0][0]*M2[0][1] + M1[0][1]+M2[1][1];
    M[1][0] = M1[1][0]*M2[0][0] + M1[1][1]+M2[1][0]; 
    M[1][1] = M1[1][0]*M2[0][1] + M1[1][1]+M2[1][1];
}

//==================================
// ESPAÇO

// Operação de soma de dois vetores no R3
vec3 operator+(vec3 u, vec3 v){
    return {u.x + v.x, u.y + v.y, u.z + v.z};
}

// Operação de multiplicação de um vetor no R3 por um escalar
vec3 operator*(float a, vec3 v){
    return {a*v.x, a*v.y, a*v.z};
}

// Diferença entre vetores no R3
vec3 operator-(vec3 u, vec3 v){
    return {u.x-v.x, u.y-v.y, u.z-v.z};
}

// Produto interno entre vetores no R3
float dot(vec3 u, vec3 v){
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

float dot(vec4 u, vec4 v){
    return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
}

// Norma de um vetor no R3
float norm(vec3 u){
    return sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2));
}

// Retorna o vetor normalizado no R3
vec3 normalize(vec3 u){
    float norma = norm(u);
    return {u.x/norma, u.y/norma, u.z/norma};
}

// Produto vetorial no R3
vec3 cross(vec3 u, vec3 v){
    return {
        u.y*v.z - u.z*v.y,
        u.y*v.x - u.x*v.y,
        u.x*v.y - u.y*v.x
    };
}

// VETORES 3D COM REFERENCIAL (VEC4)
// Soma de vec4
vec4 operator+(vec4 u, vec4 v){
    return {u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w};
}

vec4 operator-(vec4 u, vec4 v){
    return {u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w};
}

// multiplicação por escalar de vec4
vec4 operator*(float a, vec4 v){
    return {a*v.x, a*v.y, a*v.z, a*v.w};
}

// Conversão de vec4 para vec3
vec3 vec4to3(vec4 u){
    return {u.x/u.w, u.y/u.w, u.z/u.w};
    //return {u.x, u.y, u.z};
}

vec3 toScreen(Image img, vec4 P){
    vec3 v = vec4to3(P);
    v.x = ((v.x+1)*img.width - 1)/2;
    v.y = ((v.y+1)*img.height - 1)/2;
    return v;
}