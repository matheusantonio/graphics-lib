#version 130

in vec3 v;
in vec3 normal;

out vec4 FragColor;

uniform vec4 material[3];
uniform float shininess;
uniform vec4 light[4];

void main(){     
	vec3 N = normalize(normal);
	vec4 ambient = material[0]*light[0];

	// Verificando se é luz direcional ou fixa
	vec3 L; 
	if(light[3].w == 0)
		L = normalize(light[3].xyz);
	else
		L = normalize(light[3].xyz - v);

	vec4 diffuse = material[1]*light[1]*max(0, dot(L, N));

	vec3 E = normalize(-v);
	vec3 h = normalize(L + E);
	vec4 specular = material[2]*light[2]*pow(max(0, dot(h, N)), shininess);

	FragColor = ambient + diffuse + specular;
}