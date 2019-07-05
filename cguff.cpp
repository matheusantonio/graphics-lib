#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "cguff.h"

namespace cguff{

	std::string loadSource(const char* file){
		std::ifstream in(file);
		std::stringstream buffer;
		buffer << in.rdbuf();
		return buffer.str();
	}

	unsigned int loadShader(const char* file, GLenum type){
		std::string s = loadSource(file);
		const char* source = s.c_str();
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if(!success){
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			const char* shaderType = 
				(type==GL_VERTEX_SHADER)? "VERTEX": 
				(type==GL_FRAGMENT_SHADER)? "FRAGMENT": 
				"UNKNOWN";
			std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return shader;
	}

	unsigned int createShaderProgram(const char* vertexShaderFile, const char* fragShaderFile){
		unsigned int shaderProgram = glCreateProgram();

		unsigned int vertexShader = loadShader(vertexShaderFile, GL_VERTEX_SHADER);
		unsigned int fragmentShader = loadShader(fragShaderFile, GL_FRAGMENT_SHADER);

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);
		return shaderProgram;
	}
};

