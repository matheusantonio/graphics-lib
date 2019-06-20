#version 130

out vec3 v;
out vec3 normal;
out vec2 texCoord;

uniform mat4 ModelView;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

void main(){
	texCoord = gl_MultiTexCoord0.st;

	v = vec3(ModelView*gl_Vertex);
	normal = NormalMatrix*gl_Normal;

	gl_Position = MVP*gl_Vertex;
} 