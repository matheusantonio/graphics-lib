#ifndef DEF_MATRIX
#define DEF_MATRIX

#include "Vector.h"

// Estrutura para representar matriz de quatro dimensões
struct mat4{
    float M[4][4];
};

// MATRIZ DE QUATRO DIMENSÕES
// Multiplicação de matrizes de 4 dimensões
mat4 operator*(mat4 A, mat4 B);

// Translação usando matrizes de 4 dimensões
mat4 translate(float a, float b, float c);

// Gera matriz de escala m4
mat4 scale(float a, float b, float c);

// Matriz de rotação em torno do eixo X
mat4 rotate_x(float t);

// Matriz de rotação em torno do eixo Y
mat4 rotate_y(float t);

// Matriz de rotação em torno do eixo Z
mat4 rotate_z(float t);

// Multiplicação de um vetor vec4 por uma matriz m4
vec4 operator* (mat4 A, vec4 u);

// Multiplicação de vários vetores de 4 dimensões por uma matriz m4
void multMV4(mat4 M, vec4 P[], int n, vec4 R[]);

mat4 orthogonal(float l, float r, float b, float t, float n, float f);

// Gera o volume de frustum
mat4 frustum(float l, float r, float b, float t, float n, float f);

// Define a matriz view através do ponto de visão do observador dado
mat4 lookAt(vec3 O, vec3 C, vec3 Up);

// Define o volume de frustum a partir da perspectiva dada
mat4 perspective(float teta, float a, float n, float f);



#endif