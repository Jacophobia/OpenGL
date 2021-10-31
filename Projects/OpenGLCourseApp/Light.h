#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(
		GLfloat red, 
		GLfloat green, 
		GLfloat blue, 
		GLfloat aIntensity,
		GLfloat xDirection,
		GLfloat yDirection,
		GLfloat zDirection, 
		GLfloat dIntensity
	);

	void useLight(
		GLfloat ambientIntensityLocation, 
		GLfloat ambientColorLocation,
		GLfloat diffuseIntensityLocation,
		GLfloat directionLocation
	);

	~Light();
private:
	glm::vec3 color;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

#endif

