#ifndef DEF_COMPLEX
#define DEF_COMPLEX
#include <complex>

typedef std::complex<float> complex2;

// Converte um vetor P para um vetor complexo 
complex2 toComplex(vec2 P);

// Converte um vetor complexo para um vetor P
vec2 toVec2(complex2 c);

// Multiplicação de um vetor P por um complexo a
vec2 operator*(vec2 P, complex2 a);

complex2 operator*(double t, complex2 a);

#endif