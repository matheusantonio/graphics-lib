#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
//#include "cguff.h"

#include "ModelPlus.h"


using namespace cguff;

// Modelos

Model boat;
Model ocean;
Model coons;

// Controle da rotacao
int last_x, last_y;
mat4 R = loadIdentity();


void desenha(){
	glClearColor(0.090, 0.552, 0.729, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float aspect = (float)w/(float)h;
	
	mat4 Projection = scale(1, 1, -1)*perspective(50, aspect, 1, 50);
	mat4 View = lookAt({0, 0, 10}, {0, 0, 0}, {0, 1, 0})*R;

	setUniform("Projection", Projection);
	setUniform("View", View);

	glDepthMask(GL_FALSE);
	draw_model(coons, rotate_x(-0.8)*rotate_z(M_PI)*translate(-7,-5,3));
	draw_model(ocean, scale(1.6,1,1)*rotate_x(-1.3)*translate(0,-3,0));
	glDepthMask(GL_TRUE);

	draw_model(boat, scale(0.5, 0.5, 0.5)*rotate_z(0.4)*rotate_y(0.5)*rotate_x(0.2)*translate(0,-1,0));
	
	glutSwapBuffers();
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
	
	GLuint shaderProgram = createShaderProgram("model_shader.vert", "model_shader.frag");
	glUseProgram(shaderProgram);

	initLight();
	ocean = surface_model(30, 30);
	boat = load_model("boat.obj");
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
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	init();
	glutMainLoop();
}