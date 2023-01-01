#include "OpenGLUtils.h"

std::string Pipeline::readFileAsString(const char* path)
{
	std::string fileline;
	std::ifstream in;
	std::stringstream ss;

	in.open(path);
	assert(!in.fail());

	while (!in.eof())
	{
		std::getline(in, fileline);

		ss << fileline << "\n";
	}

	in.close();
	return ss.str();
}

void Pipeline::compileShaderFile(GLuint shader, const char* filePath)
{
	std::string source = readFileAsString(filePath);
	const char* temp = source.c_str();

	glShaderSource(shader, 1, &temp, nullptr);
	glCompileShader(shader);

	GLint compileStatus;
	GLint infoLogLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE) 
	{

		std::cerr << "Unable to compile shader " << filePath << ":\n";

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog(infoLogLength + 1);

		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data());
		infoLog[infoLogLength] = '\0';

		std::cerr << infoLog.data() << "\n";
	}

}

GLint Pipeline::getLocation(const char* uniformName) 
{
	if (!this->ready) {
		this->bind();
	}

	if ( // cache miss, query uniform location and add to cache
		this->uniformLocationCache.find(uniformName) ==
		this->uniformLocationCache.end()
		) {
		GLint location = glGetUniformLocation(this->program, uniformName);

		if (location == -1) {

			//THis is really annoying when shader is optimized
			//turning off for now
			//std::cerr << "Pipeline.getLocation: unable to find " <<
				//uniformName << "\n"
				//;

			return location;
		}

		this->uniformLocationCache[uniformName] = location;
	}

	return this->uniformLocationCache[uniformName];
}

void Pipeline::bind() 
{
	if (!this->ready) {
		this->linkShaderProgram(this->program);

		this->ready = true;
	}

	if (this->program == 0) {
		this->program = glCreateProgram();
	}

	glUseProgram(this->program);
}


void Pipeline::linkShaderProgram(GLuint program)
{
	glLinkProgram(program);

	GLint linkStatus;
	GLint infoLogLength;

	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) {
		std::cerr << "Unable to link program:\n";

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> infoLog(infoLogLength + 1);

		glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());
		infoLog[infoLogLength] = '\0';

		std::cerr << infoLog.data() << "\n";
	}
}


void Pipeline::removeShader(GLenum shaderType) {
	GLuint* shaderReference = nullptr;

	if (shaderType == GL_VERTEX_SHADER) {
		shaderReference = &this->vertexShader;
	}
	else if (shaderType == GL_TESS_CONTROL_SHADER) {
		shaderReference = &this->controlShader;
	}
	else if (shaderType == GL_TESS_EVALUATION_SHADER) {
		shaderReference = &this->evaluationShader;
	}
	else if (shaderType == GL_GEOMETRY_SHADER) {
		shaderReference = &this->geometryShader;
	}
	else if (shaderType == GL_FRAGMENT_SHADER) {
		shaderReference = &this->fragmentShader;
	}

	if (shaderReference == nullptr) {
		std::cerr << "Pipeline.removeShader: invalid shader type specified\n";
		return;
	}

	if ((*shaderReference) == 0) {
		return;
	}

	glDetachShader(this->program, (*shaderReference));
	glDeleteShader((*shaderReference));

	(*shaderReference) = 0;

	this->uniformLocationCache.clear();
	this->ready = false;
}


void Pipeline::setShader(GLenum shaderType, const char* filePath) {
	GLuint* shaderReference = nullptr;

	if (this->program == 0) {
		this->program = glCreateProgram();
	}

	if (shaderType == GL_VERTEX_SHADER) {
		shaderReference = &this->vertexShader;
	}
	else if (shaderType == GL_TESS_CONTROL_SHADER) {
		shaderReference = &this->controlShader;
	}
	else if (shaderType == GL_TESS_EVALUATION_SHADER) {
		shaderReference = &this->evaluationShader;
	}
	else if (shaderType == GL_GEOMETRY_SHADER) {
		shaderReference = &this->geometryShader;
	}
	else if (shaderType == GL_FRAGMENT_SHADER) {
		shaderReference = &this->fragmentShader;
	}

	if (shaderReference == nullptr) {
		std::cerr << "Pipeline.setShader: invalid shader type specified\n";
		return;
	}

	if ((*shaderReference) == 0) {
		(*shaderReference) = glCreateShader(shaderType);

		glAttachShader(this->program, (*shaderReference));
	}

	this->compileShaderFile((*shaderReference), filePath);

	this->uniformLocationCache.clear();
	this->ready = false;
}


void Pipeline::cleanup() {
	this->uniformLocationCache.clear();
	this->ready = false;

	if (this->program != 0) {
		if (this->vertexShader != 0) {
			glDetachShader(this->program, this->vertexShader);
			glDeleteShader(this->vertexShader);

			this->vertexShader = 0;
		}
		if (this->controlShader != 0) {
			glDetachShader(this->program, this->controlShader);
			glDeleteShader(this->controlShader);

			this->controlShader = 0;
		}
		if (this->evaluationShader != 0) {
			glDetachShader(this->program, this->evaluationShader);
			glDeleteShader(this->evaluationShader);

			this->evaluationShader = 0;
		}
		if (this->geometryShader != 0) {
			glDetachShader(this->program, this->geometryShader);
			glDeleteShader(this->geometryShader);

			this->geometryShader = 0;
		}
		if (this->fragmentShader != 0) {
			glDetachShader(this->program, this->fragmentShader);
			glDeleteShader(this->fragmentShader);

			this->fragmentShader = 0;
		}

		glDeleteProgram(this->program);
		this->program = 0;
	}
}








