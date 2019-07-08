#ifndef CGUFF_H
#define CGUFF_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Vector.h"
#include "Matrix.h"

namespace cguff{

	unsigned int loadShader(const char* file, GLenum type);

	unsigned int createShaderProgram(const char* vertexShaderFile, const char* fragShaderFile);

	inline int currentProgram(){
		int program;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);	
		return program;
	}

	inline int getLocation(std::string var){
		return glGetUniformLocation(currentProgram(), var.c_str());
	}

	inline void setUniform(std::string var, int v){
		int loc = getLocation(var); 
		if(loc >= 0)
			glUniform1i(loc, v);
	}
	inline void setUniform(std::string var, float v){
		int loc = getLocation(var); 
		if(loc >= 0)
			glUniform1f(loc, v);
	}
	inline void setUniform(std::string var, mat3 M){
		int loc = getLocation(var); 
		if(loc >= 0)
			glUniformMatrix3fv(loc, 1, true, (float*)&M);
	}

	inline void setUniform(std::string var, mat4 M){
		int loc = getLocation(var); 
		if(loc >= 0)
			glUniformMatrix4fv(loc, 1, true, (float*)&M);
	}

	inline void setUniform(std::string var, vec4 v){
		int loc = getLocation(var); 
		if(loc >= 0)
			glUniform4fv(loc, 1, (float*)&v);
	}


	inline void VertexAttribPointer(
		const char* var, 
		GLint size, 
		GLenum type, 
		GLboolean normalized, 
		GLsizei stride, 
		const GLvoid* pointer
		){
		int location = glGetAttribLocation(currentProgram(), var);
		glVertexAttribPointer(location, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(location);
	}


};

#endif
