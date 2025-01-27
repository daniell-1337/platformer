#include "input.h"

bool keys[5] = {0};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W)
        keys[0] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1 : 0;
    if (key == GLFW_KEY_A)
        keys[1] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1 : 0;
    if (key == GLFW_KEY_S)
        keys[2] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1 : 0;
    if (key == GLFW_KEY_D)
        keys[3] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1 : 0;
    if (key == GLFW_KEY_SPACE)
        keys[4] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1 : 0;
}

bool getKey(int id) {
    return keys[id];
}

