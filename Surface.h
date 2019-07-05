#include "Vector.h"

struct super{
    vec3* vertices;
    int** arestas;
    int numVertices;
    int numFaces;
};

super lerObj(const char* filename);

vec4* getVertices(super S, Color* Cin, Color* Cout);

int* getIndices(super S);