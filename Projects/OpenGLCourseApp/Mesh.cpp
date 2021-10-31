#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::createMesh(
		GLfloat* vertices, 
		unsigned int* indices, 
		unsigned int numOfVertices, 
		unsigned int numOfIndices
	) {
	indexCount = numOfIndices;

	// Here we are generating an ID which we will use later to tell 
	// the graphics card which location in it's memory we are 
	// referring to when we want to make edits since we cannot 
	// modify info on the gpu directly.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);
	// Indent so that we know which VAO we are working with.
	// We all work we do is assumed to be in the context of this
	// VAO until we unbind it.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices[0]) * numOfVertices,
		vertices,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*) (sizeof(vertices[0]) * (3)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * (5)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::renderMesh() {
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glUseProgram(0);
}

void Mesh::clearMesh() {
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

Mesh::~Mesh() {
	clearMesh();
}
