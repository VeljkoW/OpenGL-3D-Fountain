#include "water.h"
#include <iostream>
#include <sstream>
#include <fstream>

Water::Water()
{
    float vertices[] = {          
         3.0f, 0.01f, -3.0f,    0.0f, 0.3f, 0.8f, 1.0f, // donji desno
        -3.0f, 0.01f, -3.0f,    0.0f, 0.3f, 0.8f, 1.0f, // donji levo
         3.0f, 0.01f,  3.0f,    0.0f, 0.3f, 0.8f, 1.0f, // gornji desno

        -3.0f, 0.01f, -3.0f,    0.0f, 0.3f, 0.8f, 1.0f, // donji levo
        -3.0f, 0.01f,  3.0f,    0.0f, 0.3f, 0.8f, 1.0f, // gornji levo
         3.0f, 0.01f,  3.0f,    0.0f, 0.3f, 0.8f, 1.0f  // gornji desno
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
}
Water::~Water()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}
void Water::render(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glUseProgram(0);
}

unsigned int Water::compileShader(GLenum type, const char* path) {
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

unsigned int Water::createShaderProgram(const char* vsPath, const char* fsPath) {
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