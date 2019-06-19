#include "Matrix.h"
#include "cmath"

// Estrutura para representar matriz de quatro dimensões
struct mat4;

struct mat3;

// MATRIZ DE QUATRO DIMENSÕES

mat4 operator*(float u, mat4 A){
    return{{
        {u*A.M[0][0], u*A.M[0][1], u*A.M[0][2], u*A.M[0][3]},
        {u*A.M[1][0], u*A.M[1][1], u*A.M[1][2], u*A.M[1][3]},
        {u*A.M[2][0], u*A.M[2][1], u*A.M[2][2], u*A.M[2][3]},
        {u*A.M[3][0], u*A.M[3][1], u*A.M[3][2], u*A.M[3][3]},
    }};
}

// Multiplicação de matrizes de 4 dimensões
mat4 operator*(mat4 A, mat4 B){
    /* mat4 C;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            float s = 0;
            for(int k=0;k<4;k++){
                s+= A.M[i][k]*B.M[k][j];
            }
            C.M[i][j] = s;
        }
    }*/

    return {{
        { A.M[0][0]*B.M[0][0]+A.M[0][1]*B.M[1][0]+A.M[0][2]*B.M[2][0]+A.M[0][3]*B.M[3][0], 
          A.M[0][0]*B.M[0][1]+A.M[0][1]*B.M[1][1]+A.M[0][2]*B.M[2][1]+A.M[0][3]*B.M[3][1],
          A.M[0][0]*B.M[0][2]+A.M[0][1]*B.M[1][2]+A.M[0][2]*B.M[2][2]+A.M[0][3]*B.M[3][2], 
          A.M[0][0]*B.M[0][3]+A.M[0][1]*B.M[1][3]+A.M[0][2]*B.M[2][3]+A.M[0][3]*B.M[3][3]},

        { A.M[1][0]*B.M[0][0]+A.M[1][1]*B.M[1][0]+A.M[1][2]*B.M[2][0]+A.M[1][3]*B.M[3][0], 
          A.M[1][0]*B.M[0][1]+A.M[1][1]*B.M[1][1]+A.M[1][2]*B.M[2][1]+A.M[1][3]*B.M[3][1],
          A.M[1][0]*B.M[0][2]+A.M[1][1]*B.M[1][2]+A.M[1][2]*B.M[2][2]+A.M[1][3]*B.M[3][2], 
          A.M[1][0]*B.M[0][3]+A.M[1][1]*B.M[1][3]+A.M[1][2]*B.M[2][3]+A.M[1][3]*B.M[3][3]},

        { A.M[2][0]*B.M[0][0]+A.M[2][1]*B.M[1][0]+A.M[2][2]*B.M[2][0]+A.M[2][3]*B.M[3][0], 
          A.M[2][0]*B.M[0][1]+A.M[2][1]*B.M[1][1]+A.M[2][2]*B.M[2][1]+A.M[2][3]*B.M[3][1],
          A.M[2][0]*B.M[0][2]+A.M[2][1]*B.M[1][2]+A.M[2][2]*B.M[2][2]+A.M[2][3]*B.M[3][2], 
          A.M[2][0]*B.M[0][3]+A.M[2][1]*B.M[1][3]+A.M[2][2]*B.M[2][3]+A.M[2][3]*B.M[3][3]},

        { A.M[3][0]*B.M[0][0]+A.M[3][1]*B.M[1][0]+A.M[3][2]*B.M[2][0]+A.M[3][3]*B.M[3][0], 
          A.M[3][0]*B.M[0][1]+A.M[3][1]*B.M[1][1]+A.M[3][2]*B.M[2][1]+A.M[3][3]*B.M[3][1],
          A.M[3][0]*B.M[0][2]+A.M[3][1]*B.M[1][2]+A.M[3][2]*B.M[2][2]+A.M[3][3]*B.M[3][2], 
          A.M[3][0]*B.M[0][3]+A.M[3][1]*B.M[1][3]+A.M[3][2]*B.M[2][3]+A.M[3][3]*B.M[3][3]}
    }};


    //return C;
}

// Translação usando matrizes de 4 dimensões
mat4 translate(float a, float b, float c){
    return {{
        {1,0,0,a},
        {0,1,0,b},
        {0,0,1,c},
        {0,0,0,1}
    }};
}

// Gera matriz de escala m4
mat4 scale(float a, float b, float c){
    return {{
        {a, 0, 0, 0},
        {0, b, 0, 0},
        {0, 0, c, 0},
        {0, 0, 0, 1}
    }};
}

// Matriz de rotação em torno do eixo X
mat4 rotate_x(float t){
    return {{
        {1, 0, 0, 0},
        {0, cos(t), -sin(t), 0},
        {0, sin(t), cos(t), 0},
        {0, 0, 0, 1}
    }};
}

// Matriz de rotação em torno do eixo Y
mat4 rotate_y(float t){
    return {{
        {cos(t), 0, sin(t), 0},
        {0, 1, 0, 0},
        { -sin(t), 0, cos(t), 0},
        {0, 0, 0, 1}
    }};
}

// Matriz de rotação em torno do eixo Z
mat4 rotate_z(float t){
    return {{
        {cos(t), -sin(t), 0, 0},
        {sin(t), cos(t), 0, 0},
        { 0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
}

// Multiplicação de um vetor vec4 por uma matriz m4
vec4 operator* (mat4 A, vec4 u){
    vec4 v;
    v.x = {A.M[0][0]*u.x + A.M[0][1]*u.y + A.M[0][2]*u.z + A.M[0][3]*u.w};
    v.y = {A.M[1][0]*u.x + A.M[1][1]*u.y + A.M[1][2]*u.z + A.M[1][3]*u.w};
    v.z = {A.M[2][0]*u.x + A.M[2][1]*u.y + A.M[2][2]*u.z + A.M[2][3]*u.w};
    v.w = {A.M[3][0]*u.x + A.M[3][1]*u.y + A.M[3][2]*u.z + A.M[3][3]*u.w};
    return v;
}


// Multiplicação de vários vetores de 4 dimensões por uma matriz m4
void multMV4(mat4 M, vec4 P[], int n, vec4 R[]){
    for(int i=0;i<n;i++){
        R[i] = M*P[i];
    }
} // R[i] = M*P[i] para cada i de 0 a 19


mat4 orthogonal(float l, float r, float b, float t, float n, float f){
    return {{
        {2/(r-l), 0, 0, -1*(r+l)/(r-l)},
        {0, 2/(t-b), 0, -1*(t+b)/(t-b)},
        {0, 0, 2/(n-f), -1*(n+f)/(n-f)},
        {0,0,0,1}
    }};
}

mat4 frustum(float l, float r, float b, float t, float n, float f){
    return {{
        {-2*n/(r-l), 0, (r+l)/(r-l), 0},
        {0, -2*n/(t-b), (t+b)/(t-b), 0},
        {0, 0, (f+n)/(f-n), 2*n*f/(n-f)},
        {0, 0, -1, 0}
    }};
    /*/return {{
        {-2*n/(r-l), 0, (r+l)/(r-l), 0},
        {0, -2*n/(t-b), (t+b)/(t-b), 0},
        {0, 0, (f+n)/(f-n), 1},
        {0, 0, -2*n*f/(n-f), 0}
    }};*/
}

mat4 lookAt(vec3 O, vec3 C, vec3 Up){
    vec3 f = normalize(C-O);
    vec3 s = normalize(cross(f, Up));
    vec3 u = cross(s, f);

    mat4 A = {{
        {s.x, s.y, s.z, 0 }, 
        {u.x, u.y, u.z, 0}, 
        {-1*f.x, -1*f.y, -1*f.z, 0}, 
        {0, 0, 0, 1}
    }};
    mat4 B = {{
        {1, 0, 0, -1*O.x}, 
        {0, 1, 0, -1*O.y}, 
        {0, 0, 1, -1*O.z}, 
        {0, 0, 0, 1}
    }};

    return A*B;
}

mat4 perspective(float teta, float a, float n, float f){
    float t = n*tan(((teta/180)*M_PI)/2);
    float r = t*a;
    return frustum(-1*r, r, -1*t, t, -n, -f);
}

mat4 loadIdentity(){
    return {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
}

mat4 transpose(mat4 M){
    return {{
        {M.M[0][0], M.M[1][0], M.M[2][0], M.M[3][0]},
        {M.M[0][1], M.M[1][1], M.M[2][1], M.M[3][1]},
        {M.M[0][2], M.M[1][2], M.M[2][2], M.M[3][2]},
        {M.M[0][3], M.M[1][3], M.M[2][3], M.M[3][3]},
    }};
}

mat3 transpose(mat3 M){
    return {{
        {M.M[0][0], M.M[1][0], M.M[2][0]},
        {M.M[0][1], M.M[1][1], M.M[2][1]},
        {M.M[0][2], M.M[1][2], M.M[2][2]}
        
    }};
}

float cofator(mat4 M, int i, int j){
    float C[3][3];
    for (int u=0;u<4;u++){
        for(int v=0;v<4;v++){
            if(u!=i && v!=j){
                if(u>i && v>j){
                    C[u-1][v-1] = M.M[u][v];
                } else if(u>i){
                    C[u-1][v] = M.M[u][v];
                } else if(v>j){
                    C[u][v-1] = M.M[u][v];
                } else{
                    C[u][v] = M.M[u][v];
                }
            }
        }
    }
    float parcel1 = C[0][0]*C[1][1]*C[2][2] + C[0][1]*C[1][2]*C[1][0] + C[0][2]*C[1][0]*C[2][1];
    float parcel2 = C[0][2]*C[1][1]*C[2][0] + C[0][0]*C[1][2]*C[2][1] + C[0][1]*C[1][0]*C[2][2];

    return pow(-1, i+j)*(parcel1 - parcel2);
}

mat4 inverse(mat4 M){
    mat4 C;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            C.M[i][j] = cofator(M, i, j);
        }
    }
    float detA =0;
    for(int i=0;i<4;i++){
        detA+= C.M[0][i]*M.M[0][i];
    }

    C = (1/detA)*C;

    return C;
}

mat3 operator*(float u, mat3 A){
    return {{
        {A.M[0][0]*u, A.M[0][1]*u, A.M[0][2]*u},
        {A.M[1][0]*u, A.M[1][1]*u, A.M[1][2]*u},
        {A.M[2][0]*u, A.M[2][1]*u, A.M[2][2]*u}
    }};
}


float cofator(mat3 M, int i, int j){
    float C[2][2];
    for (int u=0;u<3;u++){
        for(int v=0;v<3;v++){
            if(u!=i && v!=j){
                if(u>i && v>j){
                    C[u-1][v-1] = M.M[u][v];
                } else if(u>i){
                    C[u-1][v] = M.M[u][v];
                } else if(v>j){
                    C[u][v-1] = M.M[u][v];
                } else{
                    C[u][v] = M.M[u][v];
                }
            }
        }
    }
    float det = C[0][0]*C[1][1] - C[0][1]*C[1][0];

    return pow(-1, i+j)*(det);
}

mat3 inverse(mat3 M){
    mat3 C;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            C.M[i][j] = cofator(M, i, j);
        }
    }
    float detA =0;
    for(int i=0;i<3;i++){
        detA+= C.M[0][i]*M.M[0][i];
    }

    C = (1/detA)*C;

    return C;
}

mat3 toMat3(mat4 M){
    return {{
        {M.M[0][0], M.M[0][1], M.M[0][2]},
        {M.M[1][0], M.M[1][1], M.M[1][2]},
        {M.M[2][0], M.M[2][1], M.M[2][2]}
    }};
}