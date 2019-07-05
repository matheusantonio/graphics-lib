#ifndef MODEL_H
#define MODEL_h

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include "cguff.h"
#include "Matrix.h"
#include "Vector.h"

struct Vertex;
struct Material;
struct Mesh;

struct Vertex{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
};

struct Material{
	vec4 ambient;
	vec4 specular;
	vec4 diffuse;
	float shininess;

	int texture_diffuse;
	int texture_specular;
	int texture_normal;
};

struct Mesh{
	GLuint vao;
	int Ni;
	Material mat;
};

struct Model{
	std::vector<Mesh> meshes;
	mat4 ModelMatrix;
	std::vector<Model> children;
};

unsigned int loadTexture(const char* filename);
unsigned int loadCubemap(const char* texture_faces[6]);

void draw_mesh(Mesh M);

Model load_model(const char* filename);
void draw_model(Model& M, mat4 ModelMatrix);

Material standard_material();

Mesh sphere_mesh(int nu, int nv, 
	Material mat = standard_material());

Model sphere_model(int nu, int nv, 
	Material mat = standard_material());

Model surface_model(int m, int n,
	Material mat = standard_material());

Model coons_model(int m, int n, 
	Material mat = standard_material());

#endif
