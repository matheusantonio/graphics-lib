#include <iostream>
#include <fstream>
#include "Color.h"
#include "Vector.h"
#include "Image.h"
#include "Desenho.h"
#include "Matrix.h"

using namespace std;

typedef int (*Func)(int, int);

// Desenha um quadrado a partir dos pontos 
void desenhar_quadrado(Color cor, vec2 Pi, vec2 Pf, int width, int height, Color* data){
    for(int i = Pi.x; i<Pf.x ; i++){
        for(int j=Pi.y; j<Pf.y; j++){
            data[i*width + j] = cor;
        }
    }
}

// Desenha um tabuleiro de xadrez preto e branco
void desenhar_xadrez(int width, int height, Color* data){
    float i=0, j=0;
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
            Color c = bilinear(x, y, c_blue(), c_purple(), c_red(), c_green());
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

// Função de recorte no R2
int clipline(Image img, float *xi, float*yi, float*xf, float*yf){
    // Como definir o valor do recorte?
    vec2 Li = {0,0};
    vec2 Lf = {(float)img.width,(float)img.height};

    // Fazer recorte aqui
    float p1 = -1*(*xf - *xi);
    float p2 = (*xf - *xi);
    float p3 = -1*(*yf-*yi);
    float p4 = (*yf-*yi);

    float q1 = *xi - Li.x;
    float q2 = Lf.x - *xi;
    float q3 = *yi - Li.y;
    float q4 = Lf.y - *yi;


    if((p1==0 && q1<0) || (p3==0 && q3<0) ) return 1;

    float posarr[5], negarr[5];
    int posind = 1, negind = 1;
    posarr[0] = 1;
    negarr[0] = 0;

    if(p1!=0){
        float t1=q1/p1, t2=q2/p2;
        if(p1<0){
            negarr[negind++] = t1;
            posarr[posind++] = t2;
        } else{
            negarr[negind++] = t2;
            posarr[posind++] = t1;
        }
    }
    if(p3!=0){
        float t3=q3/p3, t4=q4/p4;
        if(p3<0){
            negarr[negind++] = t3;
            posarr[posind++] = t4;
        } else{
            negarr[negind++] = t4;
            posarr[posind++] = t3;
        }
    }

    float xni, xnf, yni, ynf;
    float rn1=0, rn2=1;
    for(int i=0;i<negind;i++){
        if(negarr[i] > rn1) rn1 = negarr[i];
        if(posarr[i] < rn2) rn2 = posarr[i]; 
    }

    if(rn1> rn2) return 1;

    xni = *xi + p2 * rn1;
    yni = *yi + p4 * rn1;

    xnf = *xi + p2 * rn2;
    ynf = *yi + p4 * rn2;

    *xi=xni;
    *xf=xnf;
    *yi=yni;
    *yf=ynf;

    return 0;
}

// Desenha uma linha em R2
void draw_line(Image I, float xi, float yi, float xf, float yf,  Color c){
    flip_image();
    
    if(clipline (I, &xi, &yi, &xf, &yf)) return;

    //===========================================================================
    //Verificar se esses casos triviais sao necessários ou se o caso geral os cobre
    if(yi==yf){
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
    } else{
        int dx = xf-xi, dy=yf-yi;
        int edx = dx>0?dx:dx*-1; 
        int edy = dy>0?dy:dy*-1; //o erro pode ficar negativo e y nunca vai incrementar ou decrementar
        int erro = 0;
        
        if(edx > edy){
            int y = yi;
            if(dx>=0){
                for(int x=xi; x<=xf; x++){
                    draw_pixel(I, x, y, c);
                    erro = erro+2*edy;
                    if(erro > dx){
                        erro = erro-2*edx;
                        y=(dy>0)?y+1:y-1; 
                    }
                }
            } else {
                for(int x=xi; x>=xf; x--){ 
                    draw_pixel(I, x, y, c);
                    erro = erro+2*edy;
                    if(erro > dx){
                        erro = erro-2*edx;
                        y=(dy>0)?y+1:y-1;
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
                for(int y=yi; y>=yf; y--){ 
                    draw_pixel(I, x, y, c);
                    erro = erro+2*edx;
                    if(erro > dy){
                        erro = erro-2*edy;
                        x=(dx>0)?x+1:x-1; 
                    }
                }
            }
        }        
    }
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
    flip_image();

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
    flip_image();

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
                   Color c = intertri(bari, C[0], C[1], C[2]);
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

float lerpZ(float t, float z1,float z2){
    return (1-t)*z1 + t*z2;
}

void drawZBuffer(Image I, float x, float y, float z, Color C){
    float *zb = zbuffer(I, x, y); 
    //if( z > *zb){
        draw_pixel(I, x, y, C);
        *zb = z; 
    //}
}

// draw line para vec3
void draw_line(Image I, vec3 A, vec3 B, Color C){
//      draw_line(I, A.x, A.y,B.x, B.y, C);
 
    flip_image();


    //===========================================================================
    //Verificar se esses casos triviais sao necessários ou se o caso geral os cobre
    if(A.y==B.y){
        int xa = (A.x < B.x) ? A.x : B.x;
        int xb = (A.x > B.x) ? A.x : B.x;
        for(int x = xa; x<xb;x++){
            float z = lerpZ(x/(xb-xa), A.z, B.z); 
            drawZBuffer(I, x, A.y, z, C);
        }
    }
    if(A.x==B.x){
        int ya = (A.y < B.y) ? A.y : B.y;
        int yb = (A.y > B.y) ? A.y : B.y;
        for(int y = ya; y<yb;y++){
            float z = lerpZ((y/(yb-ya)), A.z, B.z);
            drawZBuffer(I, A.x, y, z, C);
        }
    }
    if(B.x-A.x == B.y-A.y){
        int xa, xb, y;
        if(A.x<B.x){
            xa=A.x, xb=B.x, y=A.y;
        } else{
            xa=B.x, xb=A.x, y=B.y;
        }
        for(int x=xa;x<xb;x++, y++){
            float z = lerpZ(x/(xb-xa), A.z, B.z);
            drawZBuffer(I, x, y,z,  C);
        }
    } else{
        int dx = B.x-A.x, dy=B.y-A.y;
        int edx = dx>0?dx:dx*-1; 
        int edy = dy>0?dy:dy*-1; //o erro pode ficar negativo e y nunca vai incrementar ou decrementar
        int erro = 0;
        
        if(edx > edy){
            int y = A.y;
            if(dx>=0){
                for(int x=A.x; x<=B.x; x++){
                    float z = lerpZ( x/(B.x-A.x), A.z, B.z);
                    drawZBuffer(I, x, y, z, C);
                    
                    erro = erro+2*edy;
                    if(erro > dx){
                        erro = erro-2*edx;
                        y=(dy>0)?y+1:y-1; 
                    }
                }
            } else {
                for(int x=A.x; x>=B.x; x--){ 
                    float z = lerpZ( x/(A.x-B.x), A.z, B.z);
                    drawZBuffer(I, x, y, z, C);
                    
                    erro = erro+2*edy;
                    if(erro > dx){
                        erro = erro-2*edx;
                        y=(dy>0)?y+1:y-1;
                    }
                }
            }
        } else{
            int x = A.x; 
            if(dy>=0){
                for(int y=A.y; y<=B.y; y++){
                    float z = lerpZ( y/(B.y-A.y), A.z, B.z);
                    drawZBuffer(I, x, y, z, C);
                    erro = erro+2*edx;
                    if(erro > dy){
                        erro = erro-2*edy;
                        x=(dx>0)?x+1:x-1;
                    }
                }
            } else {
                for(int y=A.y; y>=B.y; y--){ 
                    float z = lerpZ( y/(A.y-B.y), A.z, B.z);
                    drawZBuffer(I, x, y, z, C);

                    erro = erro+2*edx;
                    if(erro > dy){
                        erro = erro-2*edy;
                        x=(dx>0)?x+1:x-1; 
                    }
                }
            }
        }        
    }
}

// draw triangle para vec3
void draw_triangle(Image img, vec3 P[3], Color C[3]){
    vec2 P2[] = {
        {P[0].x, P[0].y},
        {P[1].x, P[1].y},
        {P[2].x, P[2].y}
    };

    flip_image();

    int xm = P[0].x, ym = P[0].y;

    int mx=0, my=0;
    for(int i=0;i<3;i++){
        if(P[i].x < mx) mx = P[i].x;
        if(P[i].x > xm) xm = P[i].x;
        if(P[i].y < my) my = P[i].y;
        if(P[i].y > ym) ym = P[i].y;
    }
    //cout << mx << ", " << my << endl;

    if(xm < 0 || mx > img.width) return;

    for(int x=mx; x<=xm; x++){
        for(int y=my; y<=ym; y++){
            float bari[3];
            barycentric({(float)x,(float)y}, P2, bari);
            if(bari[0] >= 0 && bari[0] <= 1 &&
               bari[1] >= 0 && bari[1] <= 1 &&
               bari[2] >= 0 && bari[2] <= 1){
                   Color c = intertri(bari, C[0], C[1], C[2]);
                   
                    
                    float *zb = zbuffer(img, x, y); 
                    float z = bari[0]*P[0].z + bari[1]*P[1].z + bari[2]*P[2].z;
                    //if(z>*zb){
                        //cout << "Vou pintar: " << x << ", " << y << ", " << z << " : " << xm << "- " << ym << endl;
                        draw_pixel(img, x, y, c);
                        //*zb = z;
                   //}
               }
        }
    }
}

int clipline(vec4* A, vec4* B, vec4 n){

    float a = dot(*A, n);

    float t = a/dot((*A-*B), n);

    vec4 R = (1-t)* *A + t* *B;

    float b = dot(*B, n);

    t = b/dot((*A-*B), n);

    R = (1-t)* *A + t* *B;

    A = &R;
    B = &R;

    return 0;
}

// Desenha uma linha na tela
void draw_line(Image img, vec4 A, vec4 B, Color color){

    vec4 n[] = {
        {0, 0, -1, 1},
        {0, 0, 1, 1},
        {1, 0, 0, 1},
        {-1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, -1, 0, 1},
    };
    for(int i = 0; i < 6; i++)
        if(clipline(&A, &B, n[i])) return;

    vec3 v0 = toScreen(img, A);
    vec3 v1 = toScreen(img, B);

    draw_line(img, v0, v1, color);

}

void draw_lines(Image img, vec4 *P, int n, Color c){
    for(int i=0;i<n;i+=2){
        draw_line(img, P[i], P[i+1], c);
    }
} 

void draw_line_strip(Image img, vec4 *P, int n, Color c){
    for(int i=0;i<n-1;i++){
        draw_line(img, P[i], P[i+1], c);
    }
}

void draw_line_loop(Image img, vec4 *P, int n, Color c){
    for(int i=0;i<n;i++){
        if(i==n-1){
            draw_line(img, P[i], P[0], c);    
        }else{
            draw_line(img, P[i], P[i+1], c);
        }
    }
}

void draw_elements_lines(Image img, vec4 *P, int* indices, int n, Color c){
    for(int i=0;i<(n-1);i+=2){
        draw_line(img, P[indices[i]], P[indices[i+1]], c);
    }
}

void draw_elements_line_strip(Image img, vec4* P, int* indices, int n, Color c){
    for(int i=0;i<(n-1);i++){
        draw_line(img, P[indices[i]], P[indices[i+1]], c);
    }
} 

void draw_elements_line_loop(Image img, vec4* P, int* indices, int n, Color c){
    for(int i=0;i<=(n-1);i++){
        if(i==n-1){
            draw_line(img, P[indices[i]], P[indices[0]], c);
        } else{
            draw_line(img, P[indices[i]], P[indices[i+1]], c);
        }
    }
    
}

int clip_triangle(Image img, vec4 PolC[3], Color Col[3], vec4 n){
    vec4 P[3][2];
    Color C[3];
    vec4 Pextra;

    // Copia Pin e Cin para P e C
    for(int i=0;i<3;i++){     
        P[i][0] = PolC[i];
        if(i==2) P[i][1] = PolC[0];
        else P[i][1] = PolC[i+1];
        clipline(&P[i][0], &P[i][1], n);
        C[i] = Col[i];
    }

    for(int i=0;i<3;i++){
        if(P[i][1] != P[i+1][0]){
            // existe um novo triangulo
        } else{

        }
    }


    




    return 0;
}

// Desenha um triangulo com vertices P (coordenadas normalizadas)
void draw_triangle(Image img, vec4 Pin[3], Color Cin[3]){
    vec4 P[3];
    Color C[3];
    
    // Copia Pin e Cin para P e C
    for(int i=0;i<3;i++){
        if(Pin[i].w < 0.000001) return;
        P[i] = Pin[i];
        C[i] = Cin[i];
    }

    vec4 n[] = {
        {0, 0, -1, 1},
        {0, 0, 1, 1},
        {1, 0, 0, 1},
        {-1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, -1, 0, 1},
    };
    //for(int i = 0; i < 6; i++);
        //if(clip_triangle(img, P, C, n[i]))
            //return;
    
    vec3 P3[] = {
        toScreen(img, Pin[0]),
        toScreen(img, Pin[1]),
        toScreen(img, Pin[2])
    };
    draw_triangle(img, P3, Cin);
}


void draw_triangles(Image img, vec4* P, int n, Color* C){
    for(int i=0;i<n;i+=3){
        draw_triangle(img, (P+i), C);
    }
} 

void draw_triangle_strip(Image img, vec4* P, int n, Color* C){
    for(int i=0;i<(n-2); i++){
        draw_triangle(img, P+i, (C+i));
    }
}

void draw_triangle_fan(Image img, vec4* P, int n, Color* C){
    for(int i=1;i<(n-1);i++){
        vec4 P2[3] = {
            P[0],
            P[i],
            P[i+1]
        };
        Color C2[3] = {
            C[0],
            C[i],
            C[i+1]
        };
        draw_triangle(img, P2, C2);
    } 
}

void draw_elements_triangles(Image img, vec4* P, int* indices, int n, Color*C){
    for(int i=0;i<(n-2);i+=3){
        vec4 P2[3] = {P[indices[i]], P[indices[i+1]], P[indices[i+2]]};
        Color C2[3] = {C[indices[i]], C[indices[i+1]], C[indices[i+2]]};
        //cout << "Gonna draw: " << i << endl;
        draw_triangle(img, P2, C2);
    }
}

void draw_elements_triangle_strip(Image img, vec4* P, int* indices, int n, Color* C){
    for(int i=0;i<(n-2); i++){
        vec4 P2[3] = {P[indices[i]], P[indices[i+1]], P[indices[i+2]]};
        Color C2[3] = {C[indices[i]], C[indices[i+1]], C[indices[i+2]]};
        draw_triangle(img, P+i, (C+i));
    }
}

void draw_elements_triangle_fan(Image img, vec4* P, int* indices, int n, Color* C){
    for(int i=1;i<(n-1);i++){
        vec4 P2[3] = {P[indices[0]], P[indices[i+1]], P[indices[i+2]]};
        Color C2[3] = {C[indices[0]], C[indices[i+1]], C[indices[i+2]]};
        draw_triangle(img, P2, C2);
    } 
}

// tem algum problema nessa função que preciso identificar
int lerArquivo(char* filename, vec2 * P){

    FILE *arq = fopen(filename, "r");
    if(!arq) return 1;

    int N;

    fscanf(arq, "%d\n", &N);

    P = (vec2*)malloc(N*sizeof(vec2));

    for(int i=0;i<N;i++){
        fscanf(arq, "%f %f\n", &P[i].x, &P[i].y);
    }

    fclose(arq);
    return 0;
}
