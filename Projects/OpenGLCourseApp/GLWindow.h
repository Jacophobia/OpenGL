#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow {
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);

	int initialize();

	GLfloat getBufferWidth();

	GLfloat getBufferHeight();

	bool getShouldClose();

	bool* getKeys();

	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers();

	~GLWindow();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

#endif