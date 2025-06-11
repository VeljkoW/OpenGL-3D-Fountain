#include "watercube.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

WaterCube::WaterCube() {
    float s = 0.15f;
    float vertices[] = {
        // prednja
        -s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
         s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
         s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
         s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,

        // zadnja
        -s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,

        // leva
        -s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
        -s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,

        // desna
         s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
         s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
         s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,

         // dole
         -s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
          s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
          s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
          s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
         -s, -s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
         -s, -s, -s,  0.0f, 0.3f, 0.8f, 1.0f,

         // gore
         -s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         -s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
          s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
          s,  s,  s,  0.0f, 0.3f, 0.8f, 1.0f,
          s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
         -s,  s, -s,  0.0f, 0.3f, 0.8f, 1.0f,
    };
    unsigned int stride = (3 + 4) * sizeof(float);

    shaderProgram = createShaderProgram("basic.vert", "basic.frag");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    modelLoc = glGetUniformLocation(shaderProgram, "uM");
    viewLoc = glGetUniformLocation(shaderProgram, "uV");
    projectionLoc = glGetUniformLocation(shaderProgram, "uP");

    model = glm::mat4(1.0f);

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    velocity = glm::vec3(0.0f);
    rotation = 0.0f;
    rotationSpeed = 0.0f;
    visible = false;
}

WaterCube::~WaterCube() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

void WaterCube::render(const glm::mat4& view, const glm::mat4& projection) {
    if (!visible) return;
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);
}


unsigned int WaterCube::compileShader(GLenum type, const char* path) {
    std::ifstream file(path);
    std::stringstream ss;
    if (file.is_open()) {
        ss << file.rdbuf();
        file.close();
    }
    else {
        std::cerr << "Greska otvaranja sejder fajla: " << path << std::endl;
        return 0;
    }
    std::string source = ss.str();
    const char* src = source.c_str();

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Greska kompajliranja sejdera: " << log << std::endl;
    }
    return shader;
}

unsigned int WaterCube::createShaderProgram(const char* vsPath, const char* fsPath) {
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vsPath);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fsPath);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void WaterCube::launch(const glm::vec3& direction, float speed, float spinSpeed) {
    position = glm::vec3(0.0f, 5.5f, 0.0f);
    velocity = glm::normalize(direction) * speed;
    rotation = 0.0f;
    rotationSpeed = spinSpeed;
    visible = true;
}
void WaterCube::update(float deltaTime) {
    if (!visible) return;

    // Gravitacija
    velocity.y -= 9.81f * deltaTime;

    // Update pozicije
    position += velocity * deltaTime;

    // Update rotacije
    rotation += rotationSpeed * deltaTime;

	// Sakrivanje ako je ispod poda
    if (position.y <= 0.0f) {
        visible = false;
        position.y = 0.0f;
    }

    // Update model matricu
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
	model = glm::rotate(model, rotation, glm::vec3(1.0f, 1.0f, 1.0f)); // Rotacija oko svih osa
}