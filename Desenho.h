#ifndef DEF_DESENHO
#define DEF_DESENHO

#include "Color.h"
#include "Vector.h"
#include "Image.h"

using namespace std;

typedef int (*Func)(int, int);

// Desenha um quadrado a partir dos pontos 
void desenhar_quadrado(Color cor, vec2 Pi, vec2 Pf, int width, int height, Color* data);

// Desenha um tabuleiro de xadrez preto e branco
void desenhar_xadrez(int width, int height, Color* data);

// Desenha uma figura feita a partir da interpolação bilinear de
void desenhar_interpolacao(int width, int height, Color* data);

// Desenha um pixel de uma determinada cor em uma figura
void draw_pixel(Image I, int x, int y, Color color);

// Desenha uma curva implícita no plano
void draw_implicit(Image I, Color color, Func f);

// Desenha uma linha
void draw_line(Image I, int xi, int yi, int xf, int yf,  Color c);

// Desenha uma sequência de linhas no plano
void draw_lines(Image img, vec2 *P, int n, Color c);

// Desenha uma sequência de linhas contínuas no plano
void draw_line_strip(Image img, vec2 *P, int n, Color c);

// Desenha um loop de linhas contínuas no plano
void draw_line_loop(Image img, vec2 *P, int n, Color c);

// Desenha uma sequência de linhas formando uma figura
void draw_elements_lines(Image img, vec2 *P, int* indices, int n, Color c);

void draw_elements_strip(Image img, vec2* P, int* indices, int n, Color c);

void draw_elements_loop(Image img, vec2* P, int* indices, int n, Color c);

// Desenha uma circunferência no plano
void draw_circle(Image i, int xc, int yc, int R, Color c);

// Desenha um triângulo no plano
void draw_triangle(Image i, vec2* P, Color c);

// Desenha vários triângulos no plano
void draw_triangles(Image img, vec2* P, int n, Color c);

void draw_triangle_strip(Image img, vec2* P, int n, Color c);

void draw_triangle_fan(Image img, vec2* P, int n, Color c);

// VERSÃO USANDO INTERPOLAÇÃO DE CORES
void draw_triangle(Image i, vec2*P, int n, Color*C);

void draw_triangles(Image img, vec2* P, int n, Color* C);

void draw_triangle_strip(Image img, vec2* P, int n, Color* C);

void draw_triangle_fan(Image img, vec2* P, int n, Color* C);

void draw_elements_triangles(Image img, vec2* P, int* indices, int n, Color*C);

void draw_bezier_line(Image img, vec2* P, int N, Color c);

void draw_bezier_spline(Image img, vec2* P, int n, Color c);

// tem algum problema nessa função que preciso identificar
int lerArquivo(string filename, vec2 * P);

// draw line para vec3
void draw_line(Image img, vec3 A, vec3 B, Color C);

// draw triangle para vec3
void draw_triangle(Image img, vec3 P[3], Color C[3]);

// Desenha uma linha na tela em R4
void draw_line(Image img, vec4 A, vec4 B, Color color);

void draw_lines(Image img, vec4 *P, int n, Color c);

void draw_line_strip(Image img, vec4 *P, int n, Color c);

void draw_line_loop(Image img, vec4 *P, int n, Color c);

void draw_elements_lines(Image img, vec4 *P, int* indices, int n, Color c);

void draw_elements_line_strip(Image img, vec4* P, int* indices, int n, Color c);

void draw_elements_line_loop(Image img, vec4* P, int* indices, int n, Color c);

// Desenha um triangulo com vertices P (coordenadas normalizadas)
void draw_triangle(Image img, vec4 P[3], Color C[3]);

void draw_triangles(Image img, vec4* P, int n, Color* C);

void draw_triangle_strip(Image img, vec4* P, int n, Color* C);

void draw_triangle_fan(Image img, vec4* P, int n, Color* C);

void draw_elements_triangles(Image img, vec4* P, int* indices, int n, Color*C);

void draw_elements_triangle_strip(Image img, vec4* P, int* indices, int n, Color* C);

void draw_elements_triangle_fan(Image img, vec4* P, int* indices, int n, Color* C);

#endif