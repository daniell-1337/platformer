#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int startFrame;
    int endFrame;
    int currentFrame;
    float frameWidth;
    float frameHeight;
    float sheetWidth;
    float sheetHeight;
    float frameTime;
    float timeAccumulator;
    bool loop;
} Animation;

void init_graphics(void);
GLFWwindow* init_window(const char* title, float width, float height);
void drawRect(float x, float y, float width, float height, int textureID, float tx, float ty, float tw, float th, float texWidth, float texHeight, bool flipped);
void drawGrid(void);
void playAnimation(float x, float y, int textureID, Animation* anim, float deltaTime, bool flipped);

#endif
