#define _CRT_SECURE_NO_WARNINGS
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include "floor.h"
#include "watercube.h"
#include "fountainbase.h"
#include "water.h"
#include "middlepiece.h"
#include "text.h"

float fov = 45.0f;
float cameraAngle = 0.0f;
float cameraRadius = 15.0f;
float cameraHeight = 7.0f;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 10.0f) fov = 10.0f;
    if (fov > 90.0f) fov = 90.0f;
}

int main(void)
{
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    unsigned int wWidth = mode->width;
    unsigned int wHeight = mode->height;

    const char wTitle[] = "Fountain";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, glfwGetPrimaryMonitor(), NULL);
    
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window. Terminating." << std::endl;
        glfwTerminate();
        return 2;
    }
    else {
        std::cout << "GLFW window created successfully." << std::endl;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scrollCallback);
    
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return 3;
    }
    else {
        std::cout << "GLEW initialized successfully." << std::endl;
    }
	glEnable(GL_CULL_FACE);     // Omoguci uklanjanje lica
	glCullFace(GL_BACK);        // Podesi da uklanja back face
    glFrontFace(GL_CCW);        // Podesi da je front face counter clockwise
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

    Floor floor = Floor(15.0f);
	Water water = Water();
	FountainBase fountainBase = FountainBase();
	MiddlePiece middlePiece = MiddlePiece();


    WaterCube waterCube1 = WaterCube();
    WaterCube waterCube2 = WaterCube();
    WaterCube waterCube3 = WaterCube();
    WaterCube waterCube4 = WaterCube();
    WaterCube waterCube5 = WaterCube();

    Text NameAndIndex = Text("Korela Serif.otf", 25);
    std::string label = "Veljko Vulin RA69/2021";
    float scale = 1.0f;
    float margin = 20.0f;

    float textWidth = label.length() * 20.0f * scale * 0.6f; 

    float x = wWidth - textWidth - margin;
    float y = margin;

    auto lastTime = std::chrono::high_resolution_clock::now();

    float waterTimer = 0;
    while (!glfwWindowShouldClose(window))
    {
		//Logika za FPS kontrolu
        auto frameStart = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);    //sky color

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

		//Rotacija kamere levo-desno i gore-dole
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraAngle -= 0.05f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraAngle += 0.05f; 
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraHeight += 0.4f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraHeight -= 0.4f;

        if (cameraHeight < 1.0f) cameraHeight = 1.0f;
        if (cameraHeight > 20.0f) cameraHeight = 20.0f;

        //Testiranje dubine
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            glEnable(GL_DEPTH_TEST);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            glDisable(GL_DEPTH_TEST);
        }
		//Testiranje uklanjanja lica
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            glEnable(GL_CULL_FACE);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            glDisable(GL_CULL_FACE);
        }

		//Lansiranje kocki vode
        if (!waterCube1.isVisible()) 
        {
            glm::vec3 dir = glm::normalize(glm::vec3(0.2f, 1.0f, 0.2f));
            waterCube1.launch(dir, 6.0f, 8.0f);
        }
        if(!waterCube2.isVisible() && waterTimer > 1) 
        {
            glm::vec3 dir = glm::normalize(glm::vec3(-0.2f, 1.0f, -0.2f));
            waterCube2.launch(dir, 6.0f, 8.0f);
		}
        if (!waterCube3.isVisible() && waterTimer > 2)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(0.2f, 1.0f, -0.2f));
            waterCube3.launch(dir, 6.0f, 8.0f);
        }
        if (!waterCube4.isVisible() && waterTimer > 3)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(-0.2f, 1.0f, 0.2f));
            waterCube4.launch(dir, 6.0f, 8.0f);
		}
        if(!waterCube5.isVisible() && waterTimer >= 4)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(0.25f, 1.0f, 0.2f));
            waterCube5.launch(dir, 6.0f, 8.0f);
		}
        if (waterTimer < 4)
        {
            waterTimer += 0.03;
        }


        float camX = sin(cameraAngle) * cameraRadius;
        float camZ = cos(cameraAngle) * cameraRadius;
        glm::vec3 cameraPos = glm::vec3(camX, cameraHeight, camZ);

        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraTarget,
            cameraUp
        );
        glm::mat4 projection = glm::perspective(
            glm::radians(fov),
            (float)wWidth / (float)wHeight,
            0.1f,
            100.0f
        );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //scena
        floor.render(view,projection);
        water.render(view, projection);
		fountainBase.render(view, projection);
        middlePiece.render(view, projection);

        //tekst
        NameAndIndex.Render(label, x, y, scale, glm::vec3(1.0f, 1.0f, 1.0f), wWidth, wHeight);

        //kocke vode
        waterCube1.update(deltaTime);
        waterCube1.render(view, projection);
		waterCube2.update(deltaTime);
		waterCube2.render(view, projection);
		waterCube3.update(deltaTime);
		waterCube3.render(view, projection);
		waterCube4.update(deltaTime);
		waterCube4.render(view, projection);
		waterCube5.update(deltaTime);
		waterCube5.render(view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();


		// FPS kontrola
        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> frameDuration = frameEnd - frameStart;
        float msPerFrame = 1000.0f / 60.0f; // 16.67 ms

        if (frameDuration.count() < msPerFrame) {
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(msPerFrame - frameDuration.count())));
        }
    }

    glfwTerminate();
    return 0;
}
