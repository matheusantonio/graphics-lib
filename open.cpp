//g++ -o main main.cpp -lglut -lGLU -lGL -lGLEW -lXmu -lXext -lX11 -lm
#include <GL/glew.h>
#include <GL/glut.h>

#include "cguff.h"
#include <math.h>
#include<iostream>
#include "Vector.h"
#include "Color.h"
#include "Matrix.h"
#include "Quaternions.h"

// Controle da rotacao
int last_x, last_y;
mat4 R = loadIdentity();

// Variaveis do shader
unsigned int shaderProgram;
int MVP_location, ModelView_location, NormalMatrix_location;

// Variaveis da superficie
const int m = 50, n = 50;
const int N = m*n;
vec4 P[N];
vec3 Normal[N];
const int Ni = 6*(m-1)*(n-1);
int indices[Ni];

void desenha(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float aspect = (float)w/(float)h;
	
	//mat4 Projection = scale(1, 1, -1)*perspective(50, aspect, 1, 50);
	mat4 Projection = orthogonal(-2, 2, -2, 2, -3, 3);
	mat4 View = lookAt({0, 0, 15}, {0, 0, 0}, {0, 1, 0});
	mat4 Model = R;
	
	mat4 ModelView = View*Model;
	mat3 NormalMatrix = transpose(inverse(toMat3(ModelView)));
	mat4 MVP = Projection*View*Model;

	glUniformMatrix4fv(ModelView_location, 1, true, (float*)(&ModelView));
	glUniformMatrix3fv(NormalMatrix_location, 1, true, (float*)(&NormalMatrix));
	glUniformMatrix4fv(MVP_location, 1, true, (float*)(&MVP));

	glDrawElements(GL_TRIANGLES, Ni, GL_UNSIGNED_INT, indices);

	glutSwapBuffers();
}

// Funcao auxiliar para obter a localizacao de um uniform
int location(const char* var){
	return glGetUniformLocation(shaderProgram, var);
}

void initMaterial(){
	float material[] = {
	 0.2, 0.2, 0.2, 1.0, // ambient
	 0.0, 0.5, 1.0, 1.0, // diffuse
	 1.0, 1.0, 1.0, 1.0, // specular
	};
	glUniform4fv(location("material"),  3, material);
	glUniform1f(location("shininess"), 40.0);
}

void initLight(){
	float light[] = {
		 1.0, 1.0, 1.0, 1.0, // ambient
		 1.0, 1.0, 0.5, 1.0, // diffuse
		 1.0, 1.0, 1.0, 1.0, // specular
		 1.0, 1.0, 1.0, 0.0, // position
	};
	glUniform4fv(location("light"),  4, light);
}

void initSurface(){
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
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, 0, P);
	
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, Normal);
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

	MVP_location = location("MVP");
	ModelView_location = location("ModelView");
	NormalMatrix_location = location("NormalMatrix");

	initMaterial();
	initLight();
	initSurface();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
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

/* 

int last_x, last_y;
Quaternion Q = {1, 0, 0, 0};

unsigned int shaderProgram;

void desenha(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float aspect = (float)w/(float)h;
	gluPerspective(50, aspect, 1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 4, 0, 0, 0, 0, 2, 0);
	glRotatef(2*acos(Q.a)*180/M_PI, Q.b, Q.c, Q.d);
	
	glCullFace(GL_FRONT);
	glUseProgram(0);
	glColor3f(0, 0, 0);
	glLineWidth(5);
	glPolygonMode(GL_BACK, GL_LINE);
	//glutSolidTorus(0.4, 1.5, 50, 50); 
	glFrontFace(GL_CW);
	glutSolidTeapot(1);

	glCullFace(GL_BACK);
	glUseProgram(shaderProgram);
	//glutSolidTorus(0.4, 1.5, 50, 50); 
	glutSolidTeapot(1);
	glutSwapBuffers();
}

void init(){
	glewInit();
	shaderProgram = cguff::createShaderProgram(
			"sample05.vert", 
			"sample05.frag");
	glUseProgram(shaderProgram);
	
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	
	float zero[] = {0, 0, 0, 1};
	float mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	float mat_diffuse[] = { 0.0, 0.5, 1.0, 1.0 };
	float mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float mat_shininess[] = { 20.0 };
	
	unsigned int loc;
       
	loc = glGetUniformLocation(shaderProgram, "mat_ambient");
	glUniform4fv(loc, 1, mat_ambient);
	loc = glGetUniformLocation(shaderProgram, "mat_diffuse");
	glUniform4fv(loc, 1, mat_diffuse);
	loc = glGetUniformLocation(shaderProgram, "mat_specular");
	glUniform4fv(loc, 1, mat_specular);
	loc = glGetUniformLocation(shaderProgram, "mat_shininess");
	glUniform1fv(loc, 1, mat_shininess);

	float light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	float light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 0.5, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	loc = glGetUniformLocation(shaderProgram, "light_position");
	glUniform4fv(loc, 1, light_position);
	loc = glGetUniformLocation(shaderProgram, "light_ambient");
	glUniform4fv(loc, 1, light_ambient);
	loc = glGetUniformLocation(shaderProgram, "light_diffuse");
	glUniform4fv(loc, 1, light_diffuse);
	loc = glGetUniformLocation(shaderProgram, "light_specular");
	glUniform4fv(loc, 1, light_specular);
}

void mouse(int button, int state, int x, int y){
	last_x = x;
	last_y = y;
}

void mouseMotion(int x, int y){
	int dx = x - last_x;
	int dy = y - last_y;

	vec3 u = {0, 1, 0};
	vec3 v = {1, 0, 0};
	Q = exp(dx*0.01, u)*exp(dy*0.01, v)*Q;

	last_x = x;
	last_y = y;	
	glutPostRedisplay();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	int w = 800, h = 600;
	glutInitWindowSize(w, h);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	init();
	glutMainLoop();
}*/