#include <complex>
#include <stdio.h>
#include "Vector.h"
#include "Complexos.h"

//typedef std::complex<float> complex2;
 
// Converte um vetor P para um vetor complexo 
complex2 toComplex(vec2 P){
    return {P.x, P.y};
}

// Converte um vetor complexo para um vetor P
vec2 toVec2(complex2 c){
    return {c.real(), c.imag()};
}

// Multiplicação de um vetor P por um complexo a
vec2 operator*(vec2 P, complex2 a){
    complex2 p = toComplex(P);
    return toVec2(p*a);
}

complex2 operator*(float t, complex2 a){
    return {a.real()*t, a.imag()*t};
}