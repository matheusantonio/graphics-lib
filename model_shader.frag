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

struct TextureMap{
	sampler2D Tex;
	bool hasTex;
};

uniform TextureMap diffuse_map;
uniform TextureMap specular_map;
uniform TextureMap normal_map;

uniform vec4 light[4];

vec3 checkNormal(){
	vec3 N = normalize(normal);
	if(normal_map.hasTex){
		vec3 norm = texture(normal_map.Tex, texCoord).rgb;
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
	if(diffuse_map.hasTex){
		vec4 tex_dif = texture(diffuse_map.Tex, texCoord);
		diffuse = tex_dif*diffuse;
	}
	vec3 E = normalize(-v);
	vec3 h = normalize(L + E);
	vec4 specular = material.specular*light[2]*pow(max(0, dot(h, N)), material.shininess);
	
	if(specular_map.hasTex){
		vec4 tex_spec = texture(specular_map.Tex, texCoord);
		specular = tex_spec*specular;
	}

	FragColor = ambient + diffuse + specular;
}