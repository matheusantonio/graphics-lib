#ifndef DEF_COLOR
#define DEF_COLOR

using namespace std;

typedef unsigned char Byte;

// Estrutura para representar uma cor no padrão RGB
struct Color
{
    Byte r, g, b;
};

Color c_red();
Color c_green();
Color c_blue();
Color c_purple();
Color c_white();

// Retorna um valor no intervalo [a,b]
float clamp(float v, float a, float b);

// Operação que checa a igualdade de duas cores
bool operator== (Color a, Color b);

Color operator* (float u, Color A);

Color operator+ (Color A, Color B);
// 
//string str_color (Color a);

// Interpolacao linear de duas cores A e B
Color lerp(float t, Color A, Color B);

// Interpolação bilinear de quatro cores A, B, C, D
Color bilinear(float u, float v, Color A, Color B, Color C, Color D);

Color intertri(float *b, Color A, Color B, Color C);

#endif