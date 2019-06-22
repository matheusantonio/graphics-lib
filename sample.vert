#version 330

out vec3 v;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec2 texCoord;
out mat3 NM;

in vec4 Vertex;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;
in vec2 TexCoord;

uniform mat4 ModelView;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

void main(){
	texCoord = TexCoord;

	v = vec3(ModelView*Vertex);
	normal = NormalMatrix*Normal;
	NM = NormalMatrix;

	tangent = mat3(ModelView)*Tangent;
	bitangent = mat3(ModelView)*Bitangent;

	gl_Position = MVP*Vertex;
} 