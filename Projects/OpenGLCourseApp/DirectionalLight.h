#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight :
    public Light
{
public:
    DirectionalLight();

	DirectionalLight(
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

    ~DirectionalLight();
private:
    glm::vec3 direction;
};

#endif