#ifndef DEF_QUATERNIONS
#define DEF_QUATERNIONS

#include "Vector.h"


// Estrutura para representar quartérnios
struct Quaternion{
    float a, b, c, d;
};

Quaternion exp(float e, vec3 u);

// QUARTERNIOS
// Operação de soma de quartérnios
Quaternion operator+(Quaternion q1, Quaternion q2);

// Operação de multiplicação de um quartérnio por um escalar
Quaternion operator*(float alpha, Quaternion q1);

#endif