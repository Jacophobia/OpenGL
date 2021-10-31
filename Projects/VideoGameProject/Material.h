#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();
private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

#endif

