#version 330

//layout(location=0) in vec4 Vertex;
//layout(location=1) in vec3 Normal;

out vec3 v;
out vec3 normal;
//out vec3 tangent;
//out vec3 bitangent;
out vec2 texCoord;
//out mat3 NM;
out vec4 vLightSpace;

in vec4 Vertex;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;
in vec2 TexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 LightSpaceMatrix;

//uniform mat4 ModelView;
//uniform mat4 MVP;
//uniform mat3 NormalMatrix;

void main(){
	texCoord = TexCoord;

	//v = vec3(ModelView*Vertex);
	//normal = NormalMatrix*Normal;
	//NM = NormalMatrix;

	//tangent = mat3(ModelView)*Tangent;
	//bitangent = mat3(ModelView)*Bitangent;
	mat4 ModelView = View*Model;
	mat3 NormalMatrix = transpose(inverse(mat3(ModelView)));
	mat4 MVP = Projection*ModelView;
	v = vec3(ModelView*Vertex);
	normal = normalize(NormalMatrix*Normal);

	//NM =  NormalMatrix;

	vLightSpace = LightSpaceMatrix*Model*Vertex;

	gl_Position = MVP*Vertex;
} 