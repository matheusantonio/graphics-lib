#include "Color.h"

using namespace std;

typedef unsigned char Byte;

// Estrutura para representar uma cor no padrão RGB
struct Color;

Color c_red() {
    return {255,0,0};
} 

Color c_green(){
    return {0,255,0};
}

Color c_blue(){
    return {0,0,255};
}
Color c_purple(){
    return {255,0,255};
}

Color c_white(){
    return {255,255,255};
}

// Retorna um valor no intervalo [a,b]
float clamp(float v, float a=0, float b=255){
    if(v < a)
        v = a;
    else if(v > b)
        v = b;
return v;
}
 
// Operação que checa a igualdade de duas cores
bool operator== (Color a, Color b){
    return ( a.b == b.b && a.g==b.g && a.r==b.r);
}

Color operator* (float u, Color A){
    Color C;
    C.r = clamp(u*A.r);
    C.g = clamp(u*A.g);
    C.b = clamp(u*A.b);
    return C;
}

Color operator+ (Color A, Color B){
    Color C;
    C.r = clamp(A.r + A.b);
    C.g = clamp(A.g + B.g);
    C.b = clamp(A.b + B.b);
    return C;
}

// 
//string str_color (Color a){
//    return "{" + to_string(a.r) + "," + to_string(a.g) + "," + to_string(a.b) + "}";
//}

// Interpolacao linear de duas cores A e B
Color lerp(float t, Color A, Color B){
    Color C;
    C.r = clamp((1-t)*A.r + t*B.r, 0, 255);
    C.g = clamp((1-t)*A.g + t*B.g, 0, 255);
    C.b = clamp((1-t)*A.b + t*B.b, 0, 255);
    return C;
}

// Interpolação bilinear de quatro cores A, B, C, D
Color bilinear(float u, float v, Color A, Color B, Color C, Color D){
    Color E = lerp(u, A, B);
    Color F = lerp(u, C, D);
    Color G = lerp(v, E, F);
    return G;
}

Color intertri(float *b, Color A, Color B, Color C){
    Color D;
    D.r = b[0]*A.r + b[1]*B.r + b[2]*C.r;
    D.g = b[0]*A.g + b[1]*B.g + b[2]*C.g;
    D.b = b[0]*A.b + b[1]*B.b + b[2]*C.b;
    return D;
} 