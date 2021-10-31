#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
public:
	Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode);

	void createFromFiles(const char* vertLocation, const char* fragLocation);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColorLocation();
	GLuint getDiffuseIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();
	GLuint getEyePositionLocation();

	void useShader();

	void clearShader();

	~Shader();
private:
	GLuint 
		shaderID, 
		uniformProjection, 
		uniformModel, 
		uniformView, 
		uniformAmbientIntensity, 
		uniformAmbientColor, 
		uniformDiffuseIntensity, 
		uniformDirection, 
		uniformEyePosition, 
		uniformSpecularIntensity, 
		uniformShininess;

	std::string readFile(const char* fileLocation);
	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif

