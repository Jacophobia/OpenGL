#include "GLWindow.h"

GLWindow::GLWindow() {
	width = 800;
	height = 600;
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int GLWindow::initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		printf("GLFW failed to initialize.");
		glfwTerminate();
		return 1;
	}

	// Setup glfw window properties
	// OpenGL version to start
	// Using 3.3 so we will set major to the first 3 and minor 
	// to the 3 found after the decimal
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// This means that we won't be backward compatible, but will be 
	// forward compatible
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Constructs the window where our visuals will appear
	mainWindow = glfwCreateWindow(
		width,
		height,
		"Window Name",
		NULL,
		NULL
	);

	if (!mainWindow) {
		printf("GLFW failed to create a window.");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	// Buffer holds opengl info as its passed to the window
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for glew to use
	// If you have multiple windows, you can use this method to 
	// control which one is the main window. 
	glfwMakeContextCurrent(mainWindow);

	// Handle user input (key + mouse)
	createCallbacks();

	// Lock user controls so mouse stays on screen
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features (new stuff)
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW was unable to initialize");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// This will tell OpenGL to look at the depth of all objects to decide
	// what should overlap what.
	glEnable(GL_DEPTH_TEST);

	// Set a viewport size
	// Starts at top left and goes down to bottom right
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

GLfloat GLWindow::getBufferWidth() {
	return (GLfloat) bufferWidth;
}

GLfloat GLWindow::getBufferHeight() {
	return (GLfloat) bufferHeight;
}

bool GLWindow::getShouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

bool* GLWindow::getKeys() {
	return keys;
}

GLfloat GLWindow::getXChange() {
	GLfloat change = xChange;
	xChange = 0.0f;
	return change;
}

GLfloat GLWindow::getYChange() {
	GLfloat change = yChange;
	yChange = 0.0f;
	return change;
}

void GLWindow::swapBuffers() {
	glfwSwapBuffers(mainWindow);
}

GLWindow::~GLWindow() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void GLWindow::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void GLWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
			//printf("Pressed the %d key.\n", key);
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
			//printf("Released the %d key.\n", key);
		}
	}
}

void GLWindow::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//printf("X: %.6f, Y: %.6f\n", theWindow->xChange, theWindow->yChange);
}
