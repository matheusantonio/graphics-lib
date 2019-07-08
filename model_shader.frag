#version 330 core

in vec3 v;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

out vec4 FragColor;

uniform struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
}material;

uniform sampler2D diffuse_map;
uniform bool has_diffuse_map;
uniform sampler2D normal_map;
uniform bool has_normal_map;
uniform sampler2D specular_map;
uniform bool has_specular_map;

uniform vec4 light[4];

vec3 checkNormal(){
	vec3 N = normalize(normal);
	if(has_normal_map){
		vec3 norm = texture(normal_map, texCoord).rgb;
		norm = (2*norm - 1);
		vec3 T = normalize(tangent);
		vec3 B = normalize(cross(N, T));
		T = cross(B, N);
		mat3 TBN = mat3(T, B, N);
		N = normalize(TBN*norm);
	}
	return N;
}

void main(){     
	vec3 N = checkNormal();

	vec4 ambient = material.ambient*light[0];

	// Verificando se é luz direcional ou fixa
	vec3 L; 
	if(light[3].w == 0)
		L = normalize(light[3].xyz);
	else
		L = normalize(light[3].xyz - v);

	vec4 diffuse = material.diffuse*light[1]*max(0, dot(L, N));
	if(has_diffuse_map){
		vec4 tex_dif = texture(diffuse_map, texCoord);
		diffuse = tex_dif*diffuse;
	}
	vec3 E = normalize(-v);
	vec3 h = normalize(L + E);
	vec4 specular = material.specular*light[2]*pow(max(0, dot(h, N)), material.shininess);
	
	if(has_specular_map){
		vec4 tex_spec = texture(specular_map, texCoord);
		specular = tex_spec*specular;
	}

	FragColor = ambient + diffuse + specular;
}
