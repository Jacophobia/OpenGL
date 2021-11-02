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

#include "Constants.h"

#include "Mesh.h"
#include "Shader.h"
#include "GLWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"




// need to use glm::mat4 model(1.0f); to initialize an identity matrix.
// old way of doing it is glm::mat4 model;
// when resetting a matrix to an identity matrix, use model = glm::mat(1.0f);

const float toRadians = 3.14159265f / 180.0f;

GLWindow mainWindow;
std::vector<std::unique_ptr<Mesh>> meshList;
std::vector<std::unique_ptr<Shader>> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
 

// Vertex Shader (not 100% sure what these do)
// Fragment shader
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";


void calculateAverageNormals(
	unsigned int* indices,
	unsigned int indiceCount,
	GLfloat * vertices,
	unsigned int verticeCount,
	unsigned int vertexLength,
	unsigned int normalOffset
) {
	for (size_t i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vertexLength;
		unsigned int in1 = indices[i + 1] * vertexLength;
		unsigned int in2 = indices[i + 2] * vertexLength;
		glm::vec3 v1(
			vertices[in1] - vertices[in0],
			vertices[in1 + 1] - vertices[in0 + 1],
			vertices[in1 + 2] - vertices[in0 + 2]
		);
		glm::vec3 v2(
			vertices[in2] - vertices[in0],
			vertices[in2 + 1] - vertices[in0 + 1],
			vertices[in2 + 2] - vertices[in0 + 2]
		);
		glm::vec3 normal = glm::cross(v1, v2);

		normal = glm::normalize(normal); // normalize unifies the length of our vectors so we can just compare the direction

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vertexLength; i++) {
		unsigned int nOffset = i * vertexLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}


void createObjects() {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//x      y     z      u      v      nx    ny    nz
		-1.0f, -1.0f, -0.6f,  0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f,  0.5f,  0.0f,  0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  -0.6f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f,  0.5f,  1.0f,  0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,   0.0f,  0.0f,   0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,   10.0f,  0.0f,   0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,    0.0f, 10.0f,   0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,    10.0f, 10.0f,   0.0f, -1.0f, 0.0f
	};

	calculateAverageNormals(indices, 12, vertices, 32, 8, 5);
	
	std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();
	obj1->createMesh(vertices, indices, 32, 12);
	meshList.push_back(std::move(obj1));

	std::unique_ptr<Mesh> obj2 = std::make_unique<Mesh>();
	obj2->createMesh(vertices, indices, 32, 12);
	meshList.push_back(std::move(obj2));

	std::unique_ptr<Mesh> obj3 = std::make_unique<Mesh>();
	obj3->createMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(std::move(obj3));
}

void createShaders() {
	auto shader1 = std::make_unique<Shader>();
	shader1->createFromFiles(vShader, fShader);
	shaderList.push_back(std::move(shader1));
}

int main() {
	// TODO: I need to do some research into the SOIL texture library for use
	// in this project. It is a different texture library that can apparently 
	// do quite a bit more. 

	mainWindow = GLWindow(1366, 768);
	mainWindow.initialize();

	createObjects();
	createShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 1.0f);

	brickTexture = Texture((char*)("Textures/brick.png"));
	brickTexture.loadAlphaTexture();

	dirtTexture = Texture((char*)("Textures/dirt.png"));
	dirtTexture.loadAlphaTexture();

	plainTexture = Texture((char*)("Textures/plain-white.png"));
	plainTexture.loadAlphaTexture();

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	//                           red   grn   blu  abrtns x      y     z     dbrgtns
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, -1.0, -2.0f, 1.0f); 

	unsigned int pointLightCount = 0;
	//                                                         x     y     z                
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  2.5f,  0.0f, 0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -5.0f,  2.5f, -1.0f, 0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[2] = PointLight(1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  5.0f,  2.5f,  0.0f, 0.3f, 0.2f, 0.1f);

	pointLightCount++;

	GLuint 
		uniformProjection = 0, 
		uniformModel = 0, 
		uniformView = 0, 
		uniformEyePosition = 0,
		uniformSpecularIntensity = 0,
		uniformShininess = 0;

	// This gives us a viewpoint so that we see things from a 3 dimensional 
	// perspective. Orthographic is the 2d default perspective
	// Params: viewpoint (angle from top to bottom of screen in relation to 
	// origin), Aspect ratio (width / height), front view (closest point to 
	// camera that you can still see), back view (furthest visible point 
	// that you can still see).
	glm::mat4 projection = glm::perspective(
		45.0f, 
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 
		0.1f, 
		100.0f
	);
	int rotation = 0;

	uniformModel = shaderList[0]->getModelLocation();
	uniformProjection = shaderList[0]->getProjectionLocation();
	uniformView = shaderList[0]->getViewLocation();
	uniformEyePosition = shaderList[0]->getEyePositionLocation();
	uniformSpecularIntensity = shaderList[0]->getSpecularIntensityLocation();
	uniformShininess = shaderList[0]->getShininessLocation();

	

	// loop until window is closed
	while (!mainWindow.getShouldClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// get and handle user input event
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window to a color of our choice
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // R G B Transparency
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		shaderList[0]->useShader();

		shaderList[0]->setDirectionalLight(&mainLight);
		shaderList[0]->setPointLights(pointLights, pointLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, -2.5f));
		model = glm::rotate(model, (rotation % 360) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); // usually do scale last so it doesn't affect translation
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		brickTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->renderMesh();
		glUseProgram(0);
		rotation += 1;


		shaderList[0]->useShader();
		uniformModel = shaderList[0]->getModelLocation();
		uniformProjection = shaderList[0]->getProjectionLocation();

		model = glm::mat4(1.0f);
		model = glm::rotate(model, (rotation % 360) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); // usually do scale last so it doesn't affect translation
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtTexture.useTexture();
		dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->renderMesh();


		glUseProgram(0);

		shaderList[0]->useShader();
		uniformModel = shaderList[0]->getModelLocation();
		uniformProjection = shaderList[0]->getProjectionLocation();

		model = glm::mat4(1.0f);
		//model = glm::rotate(model, (rotation % 360) * toRadians, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); // usually do scale last so it doesn't affect translation
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		plainTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->renderMesh();


		glUseProgram(0);


		// OpenGL works by always having two scenes, one that the 
		// user can see, and one that is being drawn. They are 
		// constantly being swapped to provide us with the moving
		// images that we see. Can be more than two, but this is 
		// atypical.
		mainWindow.swapBuffers();
	}

	return 0;
}