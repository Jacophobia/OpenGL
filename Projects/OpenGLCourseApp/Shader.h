#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "Constants.h"

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

	void setDirectionalLight(DirectionalLight* dlLight);

	void setPointLights(PointLight* pLights, unsigned int lightCount);

	void useShader();

	void clearShader();

	~Shader();
private:
	int pointLightCount;

	GLuint 
		shaderID, 
		uniformProjection, 
		uniformModel, 
		uniformView, 
		uniformEyePosition, 
		uniformSpecularIntensity, 
		uniformShininess, 
		uniformPointLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	std::string readFile(const char* fileLocation);
	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif

