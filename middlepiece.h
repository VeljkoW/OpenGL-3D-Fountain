#ifndef MIDDLEPIECE_H
#define MIDDLEPIECE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MiddlePiece {
public:
	MiddlePiece();
	~MiddlePiece();
	void render(const glm::mat4& view, const glm::mat4& projection);

private:
	GLuint VAO, VBO;
	GLuint shaderProgram;
	unsigned int modelLoc, viewLoc, projectionLoc;
	glm::mat4 model;

	unsigned int compileShader(GLenum type, const char* path);
	unsigned int createShaderProgram(const char* vsPath, const char* fsPath);
};

#endif
