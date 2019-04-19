#include "Vector.h"
#include <cmath>

// Estrutura para representar um vetor de duas dimensões
struct vec2;

// Estrutura para representar um vetor de três dimensões
struct vec3;

// Estrutura para representar quartérnios
struct Quaternion;

struct vec4;

// Estrutura para representar matriz de quatro dimensões
struct mat4;

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

// QUARTERNIOS
// Operação de soma de quartérnios
Quaternion operator+(Quaternion q1, Quaternion q2){
    Quaternion q;
    q.a = q1.a + q2.a;
    q.b = q1.b + q2.b;
    q.c = q1.c + q2.c;
    q.d = q1.d + q2.d;
    return q;
}

// Operação de multiplicação de um quatérnio por um escalar
Quaternion operator*(float alpha, Quaternion q1){
    Quaternion q;
    q.a = alpha*q1.a;
    q.b = alpha*q1.b;
    q.c = alpha*q1.c;
    q.d = alpha*q1.d;
    return q;
}

// Multiplicação de quaternios
Quaternion operator*(Quaternion q1, Quaternion q2){
    Quaternion q;

    q.a = q1.a*q2.a + q1.b*q2.b + q1.c*q2.c + q1.d*q2.d;
    q.b = q1.a*q2.b + q1.b*q2.a + q1.d*q2.d - q1.d*q2.c;
    q.c = q1.a*q2.c + q1.c*q2.a - q1.b*q2.d + q1.d*q2.b;
    q.d = q1.a*q2.d + q1.d*q2.a + q1.b*q2.c - q1.c*q2.b;

    return q;
}

// VETORES 3D COM REFERENCIAL (VEC4)
// Soma de vec4
vec4 operator+(vec4 u, vec4 v){
    return {u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w};
}

// multiplicação por escalar de vec4
vec4 operator*(float a, vec4 v){
    return {a*v.x, a*v.y, a*v.z, a*v.w};
}

// Conversão de vec4 para vec3
vec3 vec4to3(vec4 u){
    return {u.x/u.w, u.y/u.w, u.z/u.w};
}

// MATRIZ DE QUATRO DIMENSÕES
// Multiplicação de matrizes de 4 dimensões
mat4 operator*(mat4 A, mat4 B){
    mat4 C;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            float s = 0;
            for(int k=0;k<4;k++){
                s+= A.M[i][j]*B.M[k][j];
            }
            C.M[i][j] = s;
        }
    }
    return C;
}

// Translação usando matrizes de 4 dimensões
mat4 translate(float a, float b, float c){
    mat4 T;
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            if(i==j) T.M[i][j] = 1;
            else T.M[i][j] = 0;
        }
    }
    T.M[4][0] = a;
    T.M[4][1] = b;
    T.M[4][2] = c;
    T.M[4][3] = 1;
    return T;
}

// Gera matriz de escala m4
mat4 scale(float a, float b, float c){
    mat4 T;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(i!=j) T.M[i][j] = 0;
        }
    }
    T.M[0][0] = a;
    T.M[1][1] = b;
    T.M[2][2] = c;
    T.M[3][3] = 1;
    return T;
}

// Gera matriz m4 a partir da matriz dada
mat4 rotate(float MR[4][4]){
    mat4 R;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            R.M[i][j] = MR[i][j];
        }
    }
    return R;
}

// Matriz de rotação em torno do eixo X
mat4 rotate_x(float t){
    float MR[4][4] = {
        {1, 0, 0, 0},
        {0, cos(t), -sin(t), 0},
        {0, sin(t), cos(t), 0},
        {0, 0, 0, 1}
    };
    return rotate(MR);
}

// Matriz de rotação em torno do eixo Y
mat4 rotate_y(float t){
    float MR[4][4] = {
        {cos(t), 0, sin(t), 0},
        {0, 1, 0, 0},
        { -sin(t), 0, cos(t), 0},
        {0, 0, 0, 1}
    };
    return rotate(MR);
}

// Matriz de rotação em torno do eixo Z
mat4 rotate_z(float t){
    float MR[4][4] = {
        {cos(t), -sin(t), 0, 0},
        {sin(t), cos(t), 0, 0},
        { 0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    return rotate(MR);
}

// Multiplicação de um vetor vec4 por uma matriz m4
vec4 operator* (mat4 A, vec4 u){
    vec4 v;
    v.x = {A.M[0][0]*u.x + A.M[0][1]*u.y + A.M[0][2]*u.z + A.M[0][3]*u.w};
    v.x = {A.M[1][0]*u.x + A.M[1][1]*u.y + A.M[1][2]*u.z + A.M[1][3]*u.w};
    v.x = {A.M[2][0]*u.x + A.M[2][1]*u.y + A.M[2][2]*u.z + A.M[2][3]*u.w};
    v.x = {A.M[3][0]*u.x + A.M[3][1]*u.y + A.M[3][2]*u.z + A.M[3][3]*u.w};
    return v;
}

// Multiplicação de vários vetores de 4 dimensões por uma matriz m4
void multMV4(mat4 M, vec4 P[], int n, vec4 R[]){
    for(int i=0;i<n;i++){
        R[i] = M*P[i];
    }
} // R[i] = M*P[i] para cada i de 0 a 19