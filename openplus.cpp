#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <unistd.h>
//#include "cguff.h"

#include "ModelPlus.h"


using namespace cguff;

// Modelos

Model boat;
Model ocean;
Model coons;
Model skybox;

int day_cubemap_texture;
int night_cubemap_texture;
GLuint cubemap_shaderProgram;
GLuint shaderProgram;

// Controle da rotacao
int last_x, last_y;
float s = 0;
float t=0;
float tim=0;
mat4 R = loadIdentity();

void updatelight()
{
    float light[] = {
	1.0, 1.0, 1.0, 1.0,
	(1-tim)*0.8+0.2, (1-tim)*0.8+0.2, (1-tim)*0.8+0.2, 1.0,
	(1-tim)*0.8+0.2, (1-tim)*0.8+0.2, (1-tim)*0.8+0.2, 1.0,
	1.0, 1.0, 1.0, 0.0
    };
    glUniform4fv(getLocation("light"),  4, light);
}

void desenha(){
	glClearColor(0.090, 0.552, 0.729, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float aspect = (float)w/(float)h;

	mat4 Projection = scale(1, 1, -1)*perspective(50, aspect, 1, 50);
	mat4 View = lookAt({0, 0, 10}, {0, 0, 0}, {0, 1, 0});

	


	 ///skybox
	glUseProgram(cubemap_shaderProgram);
	setUniform("Projection", Projection);
	setUniform("View", R);
	setUniform("time",tim);

	glDepthMask(GL_FALSE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, day_cubemap_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, night_cubemap_texture);

	glUniform1i(glGetUniformLocation(cubemap_shaderProgram, "daybox"),0);
	glUniform1i(glGetUniformLocation(cubemap_shaderProgram, "nightbox"),1);
	draw_model(skybox, scale(3, 3, 3));

	///scene

	glUseProgram(shaderProgram);
	updatelight();
	setUniform("Projection", Projection);
	setUniform("View", View);
	
	
	draw_model(coons, rotate_x(-0.8)*rotate_z(M_PI)*translate(-7,-2,3));
	draw_model(ocean, scale(1.6,1,1)*rotate_x(-1.3)*translate(0,-5,0));
	glDepthMask(GL_TRUE);

	draw_model(boat, scale(0.5, 0.5, 0.5)*rotate_z(0.4)*rotate_y(0.5)*rotate_x(0.2)*translate(0,cos(s)-3,0));

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

void keyboard(unsigned char c, int x, int y){
	if(c == ' ')
    {
        t+=0.1;
        tim=abs(sin(t));
    }
	glutPostRedisplay();
}


void initCubemap(){
	const char* day[6] = {
		"skybox/day/Daylight-Box_Right.jpg",
		"skybox/day/Daylight-Box_Left.jpg",
		"skybox/day/Daylight-Box_Top.jpg",
		"skybox/day/Daylight-Box_Bottom.jpg",
		"skybox/day/Daylight-Box_Front.jpg",
		"skybox/day/Daylight-Box_Back.jpg"
	};
	day_cubemap_texture = loadCubemap(day);

    const char* night[6] = {
		"skybox/night_right.jpg",
		"skybox/night_left.jpg",
		"skybox/night_top.jpg",
		"skybox/night_bottom.jpg",
		"skybox/night_front.jpg",
		"skybox/night_back.jpg"
	};
	night_cubemap_texture = loadCubemap(night);
}


void init(){
	glewInit();

	glEnable(GL_DEPTH_TEST);

	cubemap_shaderProgram= createShaderProgram("skybox/cubemap.vert","skybox/cubemap.frag");
	glUseProgram(cubemap_shaderProgram);
	initCubemap();
	skybox = load_model("box.obj");

	shaderProgram = createShaderProgram("model_shader.vert", "model_shader.frag");
	glUseProgram(shaderProgram);

	initLight();
	ocean = surface_model(30, 30, s);
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
	glutKeyboardFunc(keyboard);


	init();
	glutMainLoop();
}
