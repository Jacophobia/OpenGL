#ifndef RENDERER_H
#define RENDERER_H
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <memory>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "GLWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

#include "RenderedObject.h"

struct Color {
	float Red = 1.0f, Green = 1.0f, Blue = 1.0f;
};

struct Direction {
	float x = 0.0f, y = -1.0f, z = 0.0f;
};

struct Uniforms {                              
	GLuint Projection = 0;
	GLuint Model = 0;
	GLuint View = 0;
	GLuint AmbientIntensity = 0;
	GLuint AmbientColor = 0;
	GLuint Direction = 0;
	GLuint DiffuseIntensity = 0;
	GLuint EyePosition = 0;
	GLuint SpecularIntensity = 0;
	GLuint Shininess = 0;
};

class Renderer {
private:
	// Attributes
	std::vector<std::shared_ptr<RenderedObject>> drawableObjects;

	GLWindow mainWindow;
	Camera camera;
	Light mainLight;
	Uniforms uniform;
	glm::mat4 projection;

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;
	GLfloat now = 0.0f;
	bool firstLoop;
	
	// Methods
	void handleUserInput();
	void drawObject(std::shared_ptr<RenderedObject> object);

public:
	// Runs all of the one-time operations that need to take place to render stuff
	//     * Window creation
	//     * Creating a default light source
	//	   * Initialize needed uniform variables
	//     * Create a viewpoint/projection
	Renderer(
		int windowWidth, 
		int windowHeight, 
		Color lightColor, 
		Direction lightDirection, 
		float ambientLightIntensity, 
		float directionalLightIntensity
	);

	// TODO: add load texture, load materials to drawable objects
	void addObject(std::shared_ptr<RenderedObject> object);

	// Essentially the game loop, needs to be run in a while loop that closes 
	// when the shouldClose method returns true
	void draw();

	// Calls GLWindow.getShouldClose() to determine if the game should end.
	// Logic can be added to determine other reasons the game should end.
	bool shouldClose();

	~Renderer();
};

#endif