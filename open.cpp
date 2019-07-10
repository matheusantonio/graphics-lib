#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <unistd.h>
//#include "cguff.h"

#include "Model.h"


using namespace cguff;

// Modelos

Model boat;
Model ocean;
Model coons;
Model bunny;

// Controle da rotacao
int last_x, last_y;
float s = 0;
mat4 R = loadIdentity();


void desenha(){
	glClearColor(0.090, 0.552, 0.729, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float aspect = (float)w/(float)h;
	
	mat4 Projection = scale(1, 1, -1)*perspective(50, aspect, 1, 50);
	mat4 View = lookAt({0, 0, 10}, {0, 0, 0}, {0, 1, 0});

	setUniform("Projection", Projection);
	setUniform("View", View);

	glDepthMask(GL_FALSE);
	//draw_model(coons, rotate_x(-0.8)*rotate_y(M_PI)*rotate_z(M_PI)*translate(-17,-10,3));
	draw_model(coons, rotate_x(-0.8)*rotate_z(M_PI)*translate(-7,-2,3));
	draw_model(ocean, scale(1.6,1,1)*rotate_x(-1.3)*translate(0,-5,0));
	glDepthMask(GL_TRUE);

	draw_model(boat, scale(0.5, 0.5, 0.5)*rotate_z(0.4)*rotate_y(0.5)*rotate_x(0.2)*translate(0,cos(s)+-3,0));
	//draw_model(bunny, scale(10,10,10));
	
	
	glutSwapBuffers();

	s++;

	ocean = surface_model(30, 30, s);
	sleep(1);

	glutPostRedisplay();
}

void initLight(){
	float light[] = {
		 1.0, 1.0, 1.0, 1.0, // ambient
		 1.0, 1.0, 1.0, 1.0, // diffuse
		 1.0, 1.0, 1.0, 1.0, // specular
		 1.0, 1.0, 1.0, 0.0, // position
	};
	glUniform4fv(getLocation("light"),  4, light);
}

void init(){
	glewInit();
	
	glEnable(GL_DEPTH_TEST); 
	
	GLuint shaderProgram = createShaderProgram("model_shader.vert", "model_shader.frag");
	glUseProgram(shaderProgram);

	initLight();
	ocean = surface_model(30, 30, s);
	boat = load_model("boat2.obj");
	coons = coons_model(15, 15);
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);
	
	init();
	glutMainLoop();
}



