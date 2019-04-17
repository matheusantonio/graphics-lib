#ifndef DEF_COLOR
#define DEF_COLOR

using namespace std;

typedef unsigned char Byte;

// Estrutura para representar uma cor no padrão RGB
struct Color
{
    Byte r, g, b;
};

Color c_red = {255,0,0};
Color c_green = {0,255,0};
Color c_blue = {0,0,255};
Color c_purple = {255,0,255};

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
    Color C = {A.r+B.r, A.g+B.g, A.b+B.b};
}

// 
string str_color (Color a){
    return "{" + to_string(a.r) + "," + to_string(a.g) + "," + to_string(a.b) + "}";
}

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

Color intertri(float r, float s, Color A, Color B, Color C){
    Color D = lerp(r, A, B);
    return lerp(s, D, C);
}

#endif