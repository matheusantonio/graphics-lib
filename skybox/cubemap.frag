#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube daybox;
uniform samplerCube nightbox;
uniform float time;

void main(){    

	vec4 day = texture(daybox, TexCoords);
	vec4 night = texture(nightbox, TexCoords);

    FragColor = mix(day,night,time);
}
