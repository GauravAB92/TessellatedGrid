#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>

#include "OpenGLBase.h"


class Pipeline {
private:
	GLuint vertexShader = 0;
	GLuint controlShader = 0;
	GLuint evaluationShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;

	GLuint program = 0;
	bool ready = false;

	std::unordered_map<const char*, GLint> uniformLocationCache;

	std::string readFileAsString(const char* filePath);

	void compileShaderFile(GLuint shader, const char* filePath);
	void linkShaderProgram(GLuint program);

public:
	void setShader(GLenum shaderType, const char* filePath);
	void removeShader(GLenum shaderType);

	void bind();
	GLint getLocation(const char* uniformName);

	void cleanup();
};

