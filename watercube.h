#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class WaterCube {
private:
    unsigned int VAO, VBO, shaderProgram;
    unsigned int modelLoc, viewLoc, projectionLoc;
    glm::mat4 model;

    glm::vec3 position;
    glm::vec3 velocity;
    float rotation;
    float rotationSpeed;
    bool visible;

    unsigned int compileShader(GLenum type, const char* path);
    unsigned int createShaderProgram(const char* vsPath, const char* fsPath);

public:
    WaterCube();
    ~WaterCube();

    void render(const glm::mat4& view, const glm::mat4& projection);
    void update(float deltaTime);
    void launch(const glm::vec3& direction, float speed, float spinSpeed);
    bool isVisible() const { return visible; }
};