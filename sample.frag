#version 330

in vec3 v;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoord;
//in mat3 NM;
in vec4 vLightSpace;

//uniform sampler2D texture0;
//uniform sampler2D texture1;

//uniform sampler2D diffuse_map;
//uniform sampler2D specular_map;
//uniform sampler2D normal_map;
uniform sampler2D shadowMap;

out vec4 FragColor;

uniform vec4 material[3];
uniform float shininess;
uniform vec4 light[4];

struct TextureMap{
	sampler2D Tex;
	bool hasTex;
};

uniform TextureMap diffuse_map;
uniform TextureMap specular_map;
uniform TextureMap normal_map;

float calculateShadow(vec4 pos, float bias){
	// perform perspective divide
	vec3 projCoords = pos.xyz/pos.w;

	// transform to [0,1] range
	projCoords = projCoords*0.5 + 0.5;

	if(projCoords.z > 1)
		return 0.0;

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r; 

	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0/textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y)*texelSize).r; 
			shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	return shadow;
	// check whether current frag pos is in shadow
	//return currentDepth - bias > closestDepth? 1.0 : 0.0;

/*	vec3 projCoords = pos.xyz/pos.w;

	projCoords = projCoords*0.5 + 0.5;

	if(projCoords.z > 1) return 0.0;

	float closestDepth = texture(shadowMap, projCoords.xy).r;

	float currentDepth = projCoords.z;

	//new stuff
	/*
	float shadow = 0.0;
	vec2 texelSize = 1.0/textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y<=1; ++y){
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y)*texelSize).r;
			shadow += currentDepth = bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	return shadow;

	return currentDepth - bias > closestDepth ? 1.0 : 0.0;*/
}

vec4 bilinearInterp(vec2 P){
	vec4 A = vec4(1, 0, 0, 1);
	vec4 B = vec4(0, 1, 0, 1);
	vec4 C = vec4(0, 0, 1, 1);
	vec4 D = vec4(1, 0, 1, 1);
	vec4 E = mix(A, B, P.s);
	vec4 F = mix(C, D, P.s);
	return mix(E, F, P.t);
}

float rand(vec2 co){
	return fract(sin(dot(co,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 P){
	vec2 Pi = floor(P);
	vec2 Pf = fract(P);
	float r11 = rand(Pi);
	float r12 = rand(Pi + vec2(0, 1));
	float r21 = rand(Pi + vec2(1, 0));
	float r22 = rand(Pi + vec2(1, 1));
	
	float u = smoothstep(0, 1, Pf.y);
	float v = smoothstep(0, 1, Pf.x);
	float r1 = mix(r11, r12, u);
	float r2 = mix(r21, r22, u);
	return mix(r1, r2, v);
}

float turbulence(vec2 P){
	return 1.0/2.0*noise(P) +
	1.0/4.0*noise(2*P) +
	1.0/8.0*noise(4*P);
}

void main(){     

	//vec4 diff = texture(diffuse_map, texCoord);
	//vec4 spec = texture(specular_map, texCoord);
	//vec3 norm = texture(normal_map, texCoord).rgb;

	//norm = NM*(2*norm-1);

	vec3 N = normalize(normal);
	//vec3 N = normalize(norm);
	
	//vec3 N = normalize(normal);
	//vec3 T = normalize(tangent);
	//vec3 B = normalize(bitangent);
	//mat3 TBN = mat3(T, B, N);
	//N = normalize(TBN*norm);
	
	vec4 ambient = material[0]*light[0];

	// Verificando se é luz direcional ou fixa
	vec3 L; 
	if(light[3].w == 0)
		L = normalize(light[3].xyz);
	else
		L = normalize(light[3].xyz - v);

	//vec4 Tex0 = texture(texture0, texCoord);
	//vec4 Tex1 = texture(texture1, texCoord);
	//vec4 Tex = mix(Tex0, Tex1, 0.5);
	vec4 diffuse = material[1]*light[1]*max(0, dot(L, N));

	vec3 E = normalize(-v);
	vec3 h = normalize(L + E);
	vec4 specular = material[2]*light[2]*pow(max(0, dot(h, N)), shininess);

	//float bias = max(0.05*(1.0-dot(N, L)), 0.005);
	//float shadow = calculateShadow(vLightSpace, bias);
	
	//FragColor = ambient + (1-shadow)*(diffuse + specular);
	FragColor = ambient + diffuse + specular;

	//FragColor = texture(texture1, texCoord);
}