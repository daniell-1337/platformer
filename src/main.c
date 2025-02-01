#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "graphics.h"
#include "level.h"
#include "player.h"
#include "input.h"

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed load GLFW\n");
        return -1;
    }
	
    GLFWwindow* window = init_window("GLFW window", 256.0f, 144.0f);
    glfwSetKeyCallback(window, key_callback);
    init_graphics();
    init_level("levels/1/level.dat");
    init_player();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = (float)(currentFrame - lastFrame);
        lastFrame = currentFrame;

        glfwPollEvents();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        updateLevel(deltaTime);
        updatePlayer(deltaTime);

        glfwSwapBuffers(window);
	}
    glfwTerminate();
    return 0;
}
