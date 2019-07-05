#include "Surface.h"
#include "Vector.h"
#include <iostream>

struct super;

super lerObj(const char* filename){
    super S;

    FILE* obj = fopen(filename, "r");


    try{
        fscanf(obj, "#v = %d\n", &S.numVertices);
        fscanf(obj, "#f = %d\n", &S.numFaces);
    } catch(exception e){
        return S;
    }

    S.vertices = (vec3*)malloc(S.numVertices*sizeof(vec3));
    S.arestas = (int**)malloc(S.numFaces*sizeof(int*));

    for(int i=0;i<S.numVertices;i++){
        fscanf(obj, "v %f %f %f\n", &S.vertices[i].x, &S.vertices[i].y, &S.vertices[i].z );
    }

    for(int i=0;i<S.numFaces;i++){
        S.arestas[i] = (int*)malloc(3*sizeof(int));
        fscanf(obj, "f %d %d %d\n", &S.arestas[i][0], &S.arestas[i][1], &S.arestas[i][2]);
    }
    
    return S;
}

vec4* getVertices(super S, Color* Cin, Color* Cout){
    vec4* P = (vec4*)malloc(S.numVertices*sizeof(vec4));
    for(int i=0;i<S.numVertices;i++){
        P[i] = { S.vertices[i].x, S.vertices[i].y, S.vertices[i].z, 1};
        Cout[i] = bilinear(S.vertices[i].x, S.vertices[i].y, Cin[0], Cin[1], Cin[2], Cin[3]);
    }

    return P;
}

int* getIndices(super S){
    int* indices = (int*)malloc(3*S.numFaces*sizeof(int));
    for(int i=0,k=0;i<S.numFaces;i++){
        // Precisa substriar 1 porque ele considera o vertice inicial como 1, mas no vetor é 0
        indices[k++] = S.arestas[i][0]-1;
        indices[k++] = S.arestas[i][1]-1;
        indices[k++] = S.arestas[i][2]-1;
    }

    return indices;
}

