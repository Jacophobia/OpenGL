#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	
	movementSpeed = 5.0f;
	turnSpeed = 1.0f;

	update();
}

Camera::Camera(
		glm::vec3 initPosition, 
		glm::vec3 initUp, 
		GLfloat initYaw, 
		GLfloat initPitch,
		GLfloat startMoveSpeed,
		GLfloat startTurnSpeed
	) {
	position = initPosition;
	worldUp = initUp;
	yaw = initYaw;
	pitch = initPitch;

	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

glm::vec3 Camera::getCameraPosition() {
	return position;
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		position -= up * velocity;
	}
	if (keys[GLFW_KEY_SPACE]) {
		position += up * velocity;
	}

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();
}

Camera::~Camera() {

}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
