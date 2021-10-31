#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
	compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertLocation, const char* fragLocation) {
	std::string vertexString = readFile(vertLocation);
	std::string fragmentString = readFile(fragLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

GLuint Shader::getProjectionLocation() {
	return uniformProjection;
}

GLuint Shader::getModelLocation() {
	return uniformModel;
}

GLuint Shader::getViewLocation() {
	return uniformView;
}

GLuint Shader::getAmbientIntensityLocation() {
	return uniformAmbientIntensity;
}

GLuint Shader::getAmbientColorLocation() {
	return uniformAmbientColor;
}

GLuint Shader::getDiffuseIntensityLocation() {
	return uniformDiffuseIntensity;
}

GLuint Shader::getDirectionLocation() {
	return uniformDirection;
}

GLuint Shader::getSpecularIntensityLocation() {

	return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation() {
	return uniformShininess;
}

GLuint Shader::getEyePositionLocation() {
	return uniformEyePosition;
}

void Shader::useShader() {
	glUseProgram(shaderID);
}

void Shader::clearShader() {
	glUseProgram(0);
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader() {
	clearShader();
}

std::string Shader::readFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);
	if (!fileStream.is_open()) {
		printf("Failed to read file. %s doesn't exist.", fileLocation);
	}
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();

	return content;
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Unable to create shader");
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	// We can't get much info from the graphics card, so we will 
	// create a log that contains any compile-related info from 
	// the graphics card that we can get.
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// creates executables on the graphics card
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.color");
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
}

void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	// Modifies the code in memory
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}
