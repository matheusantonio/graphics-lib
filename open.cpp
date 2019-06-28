#include <GL/glew.h>
#include <GL/freeglut.h>
#include "cguff.h"
#include "Matrix.h"
#include <cmath>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Vertex{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec2 texCoord;
};

struct Mesh{
	GLuint vao;
	int Ni;
};

struct Model{
	vector<Mesh> meshes;
	mat4 ModelMatrix;
	vector<Model> children;
};

// Controle da rotacao
int last_x, last_y;
mat4 R = loadIdentity();

// Variaveis do shader
unsigned int shaderProgram, texture_shaderProgram, shadow_shaderProgram;
int MVP_location, ModelView_location, NormalMatrix_location, lightSpace_location;

GLuint depthMapFBO, depthMap;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
mat4 lightSpaceMatrix;

// Variaveis da superficie
const int m = 50, n = 50;
const int N = m*n;
vec4 P[N];
vec3 Normal[N];
vec2 TexCoord[N];
const int Ni = 6*(m-1)*(n-1);
int indices[Ni];

GLuint vao_surface;

GLuint vao_sphere;
GLuint vao_quad;

Model bunny;
Model monkey;
Model teapot;

void VertexAttribPointer(const char* var, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer){
	int location = glGetAttribLocation(shaderProgram, var);
	glVertexAttribPointer(location, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(location);
}

Mesh generateMeshes(Vertex V[], int N, unsigned int indices[], int Ni){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, N*sizeof(Vertex), V, GL_STATIC_DRAW);

	int stride = sizeof(Vertex);
	VertexAttribPointer("Vertex", 3, GL_FLOAT, GL_FALSE, stride, 0);
	unsigned long offset = sizeof(vec3);
	VertexAttribPointer("Normal", 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Ni*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	Mesh M = {vao, Ni};
	return M;
}

Mesh processMesh(aiMesh* mesh, const aiScene* scene){
	const int N = mesh->mNumVertices;
	Vertex V[N];

	for(int i=0;i<mesh->mNumVertices; i++){
		aiVector3D P = mesh->mVertices[i];
		V[i].position = {P.x, P.y, P.z};

		aiVector3D N = mesh->mNormals[i];
		V[i].normal = {N.x, N.y, N.z};
	}

	const int Nf = mesh->mNumFaces;
	const int Ni = 3*Nf;
	unsigned int indices[Ni];

	int k =0;
	for(int i=0; i< Nf; i++){
		aiFace face = mesh->mFaces[i];
		for(int j=0;j<3;j++){
			indices[k] = face.mIndices[j];
			k++;
		}
	}

	return generateMeshes(V, N, indices, Ni);
}

Model processNode(aiNode *node, const aiScene *scene){
	Model model;


	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			model.ModelMatrix.M[i][j] = node->mTransformation[i][j];

	model.meshes.resize(node->mNumMeshes);
	for(unsigned int i=0; i< node->mNumMeshes;i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model.meshes[i] = processMesh(mesh, scene);
	}

	model.children.resize(node->mNumChildren);
	for(unsigned int i =0; i< node->mNumChildren; i++){
		model.children[i] = processNode(node->mChildren[i], scene);
	}

	return model;
}

Model load_models(const char* filename){
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename,
					aiProcess_GenSmoothNormals | aiProcess_Triangulate);
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
		Model model;
		model.ModelMatrix = loadIdentity();
		return model;
	}
	return processNode(scene->mRootNode, scene);
}


void draw_mesh(Mesh M){
	glBindVertexArray(M.vao);

	glDrawElements(GL_TRIANGLES, M.Ni, GL_UNSIGNED_INT, 0);


}

void initSurface(){

	Vertex V[N];

	float u0 = -5, u1 = 5;
	float du = (u1-u0)/(m-1);
	
	float v0 = -5, v1 = 5;
	float dv = (v1-v0)/(n-1);

	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			float u = u0 + i*du;
			float v = v0 + j*dv;
		
			int ij = i + j*m;
			P[ij] = {u, v, sin(u*v/4), 1};
			
			vec3 Su = {1, 0, cos(u*v/4)*v/4};
			vec3 Sv = {0, 1, cos(u*v/4)*u/4};
			
			Normal[ij] = normalize(cross(Su, Sv));
			TexCoord[ij] = {i/(m-1.0f), j/(n-1.0f)};

			V[ij].position = {u, v, sin(u*v/4)};;
			V[ij].normal = normalize(cross(Su, Sv));
			V[ij].tangent = normalize(Su);
			V[ij].bitangent = normalize(Sv);
			V[ij].normal = normalize(cross(Su, Sv));
			V[ij].texCoord = {i/(m-1.0f), j/(n-1.0f)};
		}
	}
	
	int k = 0;
	for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			int ij = i + j*m;
			indices[k++] = ij;
			indices[k++] = ij+1;
			indices[k++] = ij+m;
			
			indices[k++] = ij+m+1;
			indices[k++] = ij+m;
			indices[k++] = ij+1;
		}
	}
	// vao e vbo
	glGenVertexArrays(1, &vao_surface);
	glBindVertexArray(vao_surface);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, N*sizeof(Vertex), V, GL_STATIC_DRAW);

	unsigned long offset = 0;
	VertexAttribPointer("Vertex", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	offset += sizeof(V[0].position);
	VertexAttribPointer("Normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);

	offset += sizeof(V[0].normal);
	VertexAttribPointer("Tangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);

	offset+= sizeof(V[0].tangent);
	VertexAttribPointer("Bitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);

	offset+= sizeof(V[0].bitangent);
	VertexAttribPointer("TexCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);

	GLuint element_buffer;
	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Ni*sizeof(unsigned int), indices, GL_STATIC_DRAW);




	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(4, GL_FLOAT, 0, P);
	//VertexAttribPointer("Vertex", 4, GL_FLOAT, GL_FALSE, 0, P);
	//VertexAttribPointer("Vertex", 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &V[0].position);

	//glEnableClientState(GL_NORMAL_ARRAY);
	//glNormalPointer(GL_FLOAT, 0, Normal);
	//VertexAttribPointer("Normal", 3, GL_FLOAT, GL_FALSE, 0, Normal);
	//VertexAttribPointer("Normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &V[0].normal);
	
	//VertexAttribPointer("Tangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &V[0].tangent);
	//VertexAttribPointer("Bitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &V[0].bitangent);

	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glTexCoordPointer(2, GL_FLOAT, 0, TexCoord);
	//VertexAttribPointer("TexCoord", 2, GL_FLOAT, GL_FALSE, 0, TexCoord);
	//VertexAttribPointer("TexCoord", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &V[0].texCoord);
}


// Verifica qual o shader program utilizado atualmente
int currentProgram(){
	int program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	return program;
}


// Obtem a localizacao de um uniform no shader program atual
int getLocation(const char* var){
	return glGetUniformLocation(currentProgram(), var);
}


// Define o valor de uma matriz 3x3 uniform no shader program atual
void setUniform(const char* var, mat3 M){
	int loc = getLocation(var);
	if(loc >= 0)
		glUniformMatrix3fv(loc, 1, true, (float*)&M);
}



// Define o valor de uma matriz 4x4 uniform no shader program atual
void setUniform(const char* var, mat4 M){
	int loc = getLocation(var);
	if(loc >= 0)
	glUniformMatrix4fv(loc, 1, true, (float*)&M);
}

void draw_model(Model& M, mat4 ModelMatrix){
	ModelMatrix = ModelMatrix*M.ModelMatrix;

	setUniform("Model", ModelMatrix);

	for(int i=0;i<M.meshes.size(); i++){
		draw_mesh(M.meshes[i]);
	}

	for(int i=0;i<M.children.size(); i++){
		draw_model(M.children[i], ModelMatrix);
	}
}







void draw_surface(){
	glBindVertexArray(vao_surface);
	glDrawElements(GL_TRIANGLES,Ni, GL_UNSIGNED_INT, 0);
}



void draw_model(GLuint vao, int Ni, mat4 Model){
	setUniform("Model", Model);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, Ni, GL_UNSIGNED_INT, 0);
}

// Funcao auxiliar para obter a localizacao de um uniform
int location(const char* var){
	return glGetUniformLocation(shaderProgram, var);
}

void desenha(){
//	draw_shadow_buffer();
//	draw_scene();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	
	float aspect = (float)w/(float)h;
	mat4 Projection = scale(1, 1, -1)*perspective(50, aspect, 1, 50);
	mat4 View = lookAt({0,0,15}, {0,0,0}, {0,1,0});

	setUniform("Projection", Projection);
	setUniform("View", View);
	//setUniform("Model", R);

	// draw bunny, teapot, monkey...
	draw_model(bunny, R*translate(-2, -2, 0)*scale(20, 20, 20));
	draw_model(teapot, R);
	draw_model(monkey, R*translate(3, 2, 1)*scale(2, 2, 2));
	//draw_surface();

	glutSwapBuffers();

}

void initMaterial(){
	float material[] = {
	 0.2, 0.2, 0.2, 1.0, // ambient
	 0.8, 0.8, 0.8, 1.0, // diffuse
	 0.1, 0.1, 0.1, 1.0, // specular
	};
	glUniform4fv(location("material"),  3, material);
	glUniform1f(location("shininess"), 100.0);
}

void initLight(){
	float light[] = {
		 1.0, 1.0, 1.0, 1.0, // ambient
		 1.0, 1.0, 1.0, 1.0, // diffuse
		 1.0, 1.0, 1.0, 1.0, // specular
		 1.0, 1.0, 1.0, 0.0, // position
	};
	glUniform4fv(location("light"),  4, light);
}

unsigned int loadTexture(const char* filename){
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texture;
}

void initTexture(){
	glActiveTexture(GL_TEXTURE0);
	loadTexture("bob.jpg");

	glActiveTexture(GL_TEXTURE1);
	loadTexture("shrek.jpg");

	glActiveTexture(GL_TEXTURE2);
	loadTexture("brickwall.jpg");

	glActiveTexture(GL_TEXTURE3);
	loadTexture("box2.png");

	glActiveTexture(GL_TEXTURE4);
	loadTexture("brickwall_normal.jpg");

	//glUniform1i(location("diffuse_map"), 2);
	//glUniform1i(location("specular_map"), 3);
	//glUniform1i(location("normal_map"), 4);
	//glUniform1i(location("texture0"), 0);
	glUniform1i(location("texture1"), 1);
}


void mouse(int button, int state, int x, int y){
	last_x = x;
	last_y = y;
}

void mouseMotion(int x, int y){
	int dx = x - last_x;
	int dy = y - last_y;

	R = rotate_y(dx*0.01)*rotate_x(dy*0.01)*R;

	last_x = x;
	last_y = y;	
	glutPostRedisplay();
}

void init(){
	glewInit();
	
	glEnable(GL_DEPTH_TEST); 
	
	shaderProgram = cguff::createShaderProgram("sample.vert", "sample.frag");
	glUseProgram(shaderProgram);

//	MVP_location = location("MVP");
//	ModelView_location = location("ModelView");
//	NormalMatrix_location = location("NormalMatrix");

	teapot = load_models("teapot.obj");
	bunny = load_models("bunny.obj");
	monkey = load_models("monkey.obj");

	initMaterial();
	initLight();
	//initSurface();
	//initTexture();

	
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	int w = 400, h = 400;
	glutInitWindowSize(w, h);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	init();
	glutMainLoop();
}