#ifndef DEF_DESENHO
#define DEF_DESENHO

#include <iostream>
#include <fstream>
#include "Color.cpp"
#include "Vector.cpp"
#include "Image.cpp"

using namespace std;

typedef int (*Func)(int, int);

Color C = {255,0,0};
Color D = {0,255,0};
Color A = {0,0,255};
Color B = {255,0,255};

// Desenha um quadrado a partir dos pontos 
void desenhar_quadrado(Color cor, vec2 Pi, vec2 Pf, int width, int height, Color* data)
{
    for(int i = Pi.x; i<Pf.x ; i++){
        for(int j=Pi.y; j<Pf.y; j++){
            data[i*width + j] = cor;
        }
    }
}

// Desenha um tabuleiro de xadrez preto e branco
void desenhar_xadrez(int width, int height, Color* data){
    int i=0, j=0;
    int passo=width/8;
    Color cor_da_vez = {0,0,0}, preto = {255,255,255};
    while(i<8*passo){
        while(j<8*passo){
            desenhar_quadrado(cor_da_vez, {i, j}, {i+passo, j+passo}, width, height, data);
            if(cor_da_vez == preto){
                cor_da_vez = {0,0,0};
            } else{
                cor_da_vez = {255,255,255};
            }
            j+=passo;
        }
        if(cor_da_vez == preto){
            cor_da_vez = {0,0,0};
        } else{
            cor_da_vez = {255,255,255};
        }
        i+=passo;
        j=0;
    }
}

// Desenha uma figura feita a partir da interpolação bilinear de
void desenhar_interpolacao(int width, int height, Color* data){
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            float x = (float)j/width, y=(float)(height-i)/height;
            Color c = bilinear(x, y, c_blue, c_purple, c_red, c_green);
            data[i*width+j] = c;
        }
    }
}

// Desenha um pixel de uma determinada cor em uma figura
void draw_pixel(Image I, int x, int y, Color color){
    if( x>=0 && x<I.width && y>=0 && y< I.height){
        Color *p = pixel(I, x, y);
        *p = color;
    }
}

// Desenha uma curva implícita no plano
void draw_implicit(Image I, Color color, Func f){
    for(int x = 0; x < I.width; x++){
        for(int y = 0; y < I.height; y++){
            if(f(x, y) <= 0){
                draw_pixel(I, x, y, color);
            }
        }
    }
}

// Desenha uma linha
void draw_line(Image I, int xi, int yi, int xf, int yf,  Color c){
    stbi_flip_vertically_on_write(1);
    
    //yi = I.height-yi; //a imagem no slide considera y=0 na parte inferior
    //yf = I.height-yf;
    
    //Verificar se esses casos sao necessários ou se o caso geral os cobre
    /*if(yi==yf){
        int xa = (xi < xf) ? xi : xf;
        int xb = (xi > xf) ? xi : xf;
        for(int x = xa; x<xb;x++){
            draw_pixel(I, x, yi, c);
        }
    }
    if(xi==xf){
        int ya = (yi < yf) ? yi : yf;
        int yb = (yi > yf) ? yi : yf;
        for(int y = ya; y<yb;y++){
            draw_pixel(I, xi, y, c);
        }
    }
    if(xf-xi == yf-yi){
        int xa, xb, y;
        if(xi<xf){
            xa=xi, xb=xf, y=yi;
        } else{
            xa=xf, xb=xi, y=yf;
        }
        for(int x=xa;x<xb;x++, y++){
            draw_pixel(I, x, y, c);
        }
    } else{*/
        int dx = xf-xi, dy=yf-yi; //-10, -140
        int edx = dx>0?dx:dx*-1; //10
        int edy = dy>0?dy:dy*-1; //140   o erro pode ficar negativo e y nunca vai incrementar ou decrementar
        int erro = 0;
        
        if(edx > edy){
            int y = yi; //y=440
            if(dx>=0){
                for(int x=xi; x<=xf; x++){
                    draw_pixel(I, x, y, c);
                    erro = erro+2*edy;
                    if(erro > dx){
                        erro = erro-2*edx;
                        y=(dy>0)?y+1:y-1; //um teste com y decrementando
                    }
                }
            } else {
                for(int x=xi; x>=xf; x--){ // x de 80 a 70
                    draw_pixel(I, x, y, c);
                    erro = erro+2*edy; //280
                    if(erro > dx){
                        erro = erro-2*edx; //260
                        y=(dy>0)?y+1:y-1; //um teste com y decrementando
                    }
                }
            }
        } else{
            int x = xi; 
            if(dy>=0){
                for(int y=yi; y<=yf; y++){
                    draw_pixel(I, x, y, c);
                    erro = erro+2*edx;
                    if(erro > dy){
                        erro = erro-2*edy;
                        x=(dx>0)?x+1:x-1;
                    }
                }
            } else {
                for(int y=yi; y>=yf; y--){ // x de 80 a 70
                    draw_pixel(I, x, y, c);
                    erro = erro+2*edx; //280
                    if(erro > dy){
                        erro = erro-2*edy; //260
                        x=(dx>0)?x+1:x-1; //um teste com y decrementando
                    }
                }
            }
        }        
    //}
}

// Desenha uma sequência de linhas no plano
void draw_lines(Image img, vec2 *P, int n, Color c){
    for(int i=0;i<n;i+=2){
        draw_line(img, P[i].x, P[i].y, P[i+1].x, P[i+1].y, c);
    }
}

// Desenha uma sequência de linhas contínuas no plano
void draw_line_strip(Image img, vec2 *P, int n, Color c){
    for(int i=0;i<n-1;i++){
        draw_line(img, P[i].x, P[i].y, P[i+1].x, P[i+1].y, c);
    }
}

// Desenha um loop de linhas contínuas no plano
void draw_line_loop(Image img, vec2 *P, int n, Color c){
    for(int i=0;i<n;i++){
        if(i==n-1){
            draw_line(img, P[i].x, P[i].y, P[0].x, P[0].y, c);    
        }else{
            draw_line(img, P[i].x, P[i].y, P[i+1].x, P[i+1].y, c);
        }
    }
}

// Desenha uma sequência de linhas formando uma figura
void draw_elements_lines(Image img, vec2 *P, int* indices, int n, Color c){
    for(int i=0;i<(n-1);i+=2){
        draw_line(img, P[indices[i]].x, P[indices[i]].y, P[indices[i+1]].x, P[indices[i+1]].y, c);
    }
}

void draw_elements_strip(Image img, vec2* P, int* indices, int n, Color c){
    for(int i=0;i<(n-1);i++){
        draw_line(img, P[indices[i]].x, P[indices[i]].y, P[indices[i+1]].x, P[indices[i+1]].y, c);
    }
}

void draw_elements_loop(Image img, vec2* P, int* indices, int n, Color c){
    for(int i=0;i<=(n-1);i++){
        if(i==n-1){
            draw_line(img, P[indices[i]].x, P[indices[i]].y, P[indices[0]].x, P[indices[0]].y, c);
        } else{
            draw_line(img, P[indices[i]].x, P[indices[i]].y, P[indices[i+1]].x, P[indices[i+1]].y, c);
        }
    }
}

// Desenha uma circunferência no plano
void draw_circle(Image i, int xc, int yc, int R, Color c){
    yc = i.height - yc;
    int y = R;
    int d = 1-R;
    draw_pixel(i, xc, yc, c);
    for(int x = 0; x<=y; ++x){

        draw_pixel(i, xc+x, yc+y, c); //(x,y)
        draw_pixel(i, xc+y, yc+x, c); //(y,x)
        draw_pixel(i, xc+y, yc-x, c); //(y,-x)
        draw_pixel(i, xc+x, yc-y, c); // (x, -y)
        draw_pixel(i, xc-x, yc-y, c); // (-x, -y)
        draw_pixel(i, xc-y, yc-x, c); // (-y, -x)
        draw_pixel(i, xc-y, yc+x, c); // (-y, x)
        draw_pixel(i, xc-x, yc+y, c); // (-x, y)

        if(d>0){
            d+=2*x-2*y+5;
            y--;
        } else{
            d+=2*x+3;
        }
    }
}

// Desenha um triângulo no plano
void draw_triangle(Image i, vec2* P, Color c){
    stbi_flip_vertically_on_write(1);

    int mx=0, my=0, xm=0, ym=0;
    for(int i=0;i<3;i++){
        if(P[i].x < mx) mx = P[i].x;
        if(P[i].x > xm) xm = P[i].x;
        if(P[i].y < my) my = P[i].y;
        if(P[i].y > ym) ym = P[i].y;
    }

    for(int x=mx; x<=xm; x++){
        for(int y=my; y<=ym; y++){
            float bari[3];
            barycentric({(float)x,(float)y}, P, bari);
            if(bari[0] >= 0 && bari[0] <= 1 &&
               bari[1] >= 0 && bari[1] <= 1 &&
               bari[2] >= 0 && bari[2] <= 1){
                   draw_pixel(i, x, y, c);
               }
        }
    }
}

// Desenha vários triângulos no plano
void draw_triangles(Image img, vec2* P, int n, Color c){
    for(int i=0;i<n;i+=3){
        draw_triangle(img, (P+i), c);
    }
}

void draw_triangle_strip(Image img, vec2* P, int n, Color c){
    for(int i=0;i<(n-2); i++){
        draw_triangle(img, P+i, c);
    }
}

void draw_triangle_fan(Image img, vec2* P, int n, Color c){
    for(int i=1;i<(n-1);i++){
        vec2 P2[3] = {
            P[0],
            P[i],
            P[i+1]
        };
        draw_triangle(img, P2, c);
    } 
}

// VERSÃO USANDO INTERPOLAÇÃO DE CORES
void draw_triangle(Image i, vec2*P, int n, Color*C){
    stbi_flip_vertically_on_write(1);

    int mx=0, my=0, xm=0, ym=0;
    for(int i=0;i<3;i++){
        if(P[i].x < mx) mx = P[i].x;
        if(P[i].x > xm) xm = P[i].x;
        if(P[i].y < my) my = P[i].y;
        if(P[i].y > ym) ym = P[i].y;
    }

    for(int x=mx; x<=xm; x++){
        for(int y=my; y<=ym; y++){
            float bari[3];
            barycentric({(float)x,(float)y}, P, bari);
            if(bari[0] >= 0 && bari[0] <= 1 &&
               bari[1] >= 0 && bari[1] <= 1 &&
               bari[2] >= 0 && bari[2] <= 1){
                   Color c = bari[0]*C[0] + bari[1]*C[1] + bari[2]*C[2];
                   draw_pixel(i, x, y, c);
               }
        }
    }
}

void draw_triangles(Image img, vec2* P, int n, Color* C){
    for(int i=0;i<n;i+=3){
        draw_triangle(img, (P+i),n,  (C+i));
    }
}

void draw_triangle_strip(Image img, vec2* P, int n, Color* C){
    for(int i=0;i<(n-2); i++){
        draw_triangle(img, P+i, n, (C+i));
    }
}

void draw_triangle_fan(Image img, vec2* P, int n, Color* C){
    for(int i=1;i<(n-1);i++){
        vec2 P2[3] = {
            P[0],
            P[i],
            P[i+1]
        };
        Color C2[3] = {
            C[0],
            C[i],
            C[i+1]
        };
        draw_triangle(img, P2, n, C2);
    } 
}

void draw_elements_triangles(Image img, vec2* P, int* indices, int n, Color*C){
    for(int i=0;i<(n-2);i+=3){
        vec2 P2[3] = {P[indices[i]], P[indices[i+1]], P[indices[i+2]]};
        Color C2[3] = {C[indices[i]], C[indices[i+1]], C[indices[i+2]]};
        draw_triangle(img, P2, 3, C2);
    }
}

void draw_bezier_line(Image img, vec2* P, int N, Color c){
    vec2 C[N];
    for(int i = 0; i < N; i++){
        float t = i/(N-1.0);
        C[i] = decasteljau(P, t);
    }
    draw_line_strip(img, C, N, c);
}

void draw_bezier_spline(Image img, vec2* P, int n, Color c){
    for(int i=0;i<(n-3);i+=3){
        draw_bezier_line(img, P+i, 50, c);
    }
}

// tem algum problema nessa função que preciso identificar
int lerArquivo(string filename, vec2 * P){
    ifstream is(filename);
    if(is){
        int n, i=0;
        is >> n;
        P=(vec2*)malloc(sizeof(vec2));
        while(i<n){
            float x, y;
            is>>x;
            is>>y;
            P[i] = {x, y};
            i++;
        }
        is.close();
        return n;
    }
    return 0;
}

#endif