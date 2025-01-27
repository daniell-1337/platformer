#ifndef INPUT_H
#define INPUT_H
#include <GLFW/glfw3.h>
#include <stdbool.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool getKey(int id);

#endif
