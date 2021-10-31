#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
	Camera();
	Camera(
		glm::vec3 initPosition, 
		glm::vec3 initUp, 
		GLfloat initYaw, GLfloat initPitch, 
		GLfloat startMoveSpeed, 
		GLfloat startTurnSpeed
	);

	glm::vec3 getCameraPosition();

	glm::mat4 calculateViewMatrix();

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	~Camera();
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};

#endif