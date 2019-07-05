#version 330 core

layout (location = 0) in vec4 Vertex;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

out vec3 v;
out vec2 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
	mat4 ModelView = View*Model;
	mat3 NormalMatrix = transpose(inverse(mat3(ModelView)));
	mat4 MVP = Projection*ModelView;
	
	v = vec3(ModelView*Vertex);
	texCoord = TexCoord;
	
	normal = normalize(NormalMatrix*Normal);
	tangent = mat3(ModelView)*Tangent;
	bitangent = mat3(ModelView)*Bitangent;
	
	gl_Position = MVP*Vertex;
} 
