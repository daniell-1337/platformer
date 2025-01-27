#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

typedef struct {
    int startFrame;
    int endFrame;
    float frameWidth;
    float frameHeight;
    float sheetWidth;
    float sheetHeight;
    float frameTime;
    int loop;
} Animation;

void init_graphics(void);
GLFWwindow* init_window(const char* title, float width, float height);
void drawRect(float x, float y, float width, float height, int textureID, float tx, float ty, float tw, float th, float texWidth, float texHeight, int flipped);
void drawGrid(void);
void playAnimation(float x, float y, float width, float height, int textureID, Animation* anim, float deltaTime, int* currentFrame, float* timeAccumulator, int flipped);

#endif
