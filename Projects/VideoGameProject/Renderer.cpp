#include "Renderer.h"

void Renderer::handleUserInput() {
	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	glfwPollEvents();
	camera.keyControl(mainWindow.getKeys(), deltaTime);
	camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
}

void Renderer::drawObject(std::shared_ptr<RenderedObject> object) {
	object->useShader();
	if (firstLoop) { // TODO: make sure that you use the same shader for everything, because if you don't, this will need to be changed
		mainLight.useLight(
			uniform.AmbientIntensity,
			uniform.AmbientColor,
			uniform.DiffuseIntensity,
			uniform.Direction
		);
		glUniformMatrix4fv(
			uniform.Projection, 1, GL_FALSE, glm::value_ptr(projection)
		);
		glUniformMatrix4fv(
			uniform.View, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())
		);
		glUniform3f(
			uniform.EyePosition,
			camera.getCameraPosition().x,
			camera.getCameraPosition().y,
			camera.getCameraPosition().z
		);
		firstLoop = false;
	}
	object->useModel();
	object->useTexture();
	object->useMaterial();
	object->useMesh();

	glUseProgram(0);
}

Renderer::Renderer(int windowWidth, int windowHeight, Color lightColor, Direction lightDirection, float ambientLightIntensity, float directionalLightIntensity) {
	mainWindow = GLWindow(1366, 768);
	mainWindow.initialize();
	camera = Camera( // TODO: Get the initial position movespeed and turnspeed from a character object used as a parameter.
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		90.0f, 0.0f, 5.0f, 1.0f
	);
	mainLight = Light(
		lightColor.Red, lightColor.Green, lightColor.Blue,
		ambientLightIntensity,
		lightDirection.x, lightDirection.y, lightDirection.z,
		directionalLightIntensity
	);
	projection = glm::perspective(
		45.0f,
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
		0.1f,
		100.0f
	);
}

void Renderer::addObject(std::shared_ptr<RenderedObject> object) {
	drawableObjects.push_back(object);
}

void Renderer::draw() {
	handleUserInput();

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // R, G, B, Transparency
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: figure out if we need to do this once, or before each object. Not sure if we need different shaders for everything. 
	//uniform.Model = shaderList[0]->getModelLocation();
	//uniform.Projection = shaderList[0]->getProjectionLocation();
	//uniform.View = shaderList[0]->getViewLocation();
	//uniform.AmbientColor = shaderList[0]->getAmbientColorLocation();
	//uniform.AmbientIntensity = shaderList[0]->getAmbientIntensityLocation();
	//uniform.Direction = shaderList[0]->getDirectionLocation();
	//uniform.DiffuseIntensity = shaderList[0]->getDiffuseIntensityLocation();
	//uniform.EyePosition = shaderList[0]->getEyePositionLocation();
	//uniform.SpecularIntensity = shaderList[0]->getSpecularIntensityLocation();
	//uniform.Shininess = shaderList[0]->getShininessLocation();

	firstLoop = true;
	for (std::shared_ptr<RenderedObject> object : drawableObjects) {
		drawObject(object);
	}

	mainWindow.swapBuffers();
}

bool Renderer::shouldClose() {
	return mainWindow.getShouldClose();
}
