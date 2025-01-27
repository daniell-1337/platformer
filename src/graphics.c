#include "graphics.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

GLuint textures[3];

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0.0f, 0.0f, width, height);
}

GLuint loadTexture(const char* path) {
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data) {
        printf("Failed to load texture\n");
        return 0;
    }

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (nrChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else if (nrChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	stbi_image_free(data);
	return texture;
}

void loadTextures(void) {
    textures[0] = loadTexture("textures/blank.jpg");
    textures[1] = loadTexture("textures/player.png");
    textures[2] = loadTexture("levels/1/level.jpg");
}

void init_graphics(void) {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to init GLAD\n");
	}

	glViewport(0.0f, 0.0f, 256.0f, 144.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 256.0f, 0.0f, 144.0f, -1.0f, 1.0f);

	stbi_set_flip_vertically_on_load(1);
	loadTextures();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLFWwindow* init_window(const char* title, float width, float height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		fprintf(stderr, "Failed load WINDOW\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

void drawRect(float x, float y, float width, float height, int textureID, float tx, float ty, float tw, float th, float texWidth, float texHeight, int flipped) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[textureID]);

    float u1 = tx / texWidth;
    float v1 = ty / texHeight;
    float u2 = (tx + tw) / texWidth;
    float v2 = (ty + th) / texHeight;

    GLfloat vertices[] = {
        0.0f, 0.0f, u1, v1,
        1.0f, 0.0f, u2, v1,
        1.0f, 1.0f, u2, v2,
        0.0f, 1.0f, u1, v2
    };

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    if (flipped) {
        glScalef(-1.0f, 1.0f, 1.0f);
        glTranslatef(-width, 0.0f, 0.0f);
    }
    
    glScalef(width, height, 1.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 4 * sizeof(float), &vertices[0]);
    glTexCoordPointer(2, GL_FLOAT, 4 * sizeof(float), &vertices[2]);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawGrid(void) {
	glBegin(GL_LINES);
		for (float x = 0.0f; x <= 256.0f; x += 16.0f) {
			glVertex2f(x, 0.0f);
			glVertex2f(x, 144.0f);
		}
		for (float y = 0.0f; y <= 144.0f; y += 16.0f) {
			glVertex2f(0.0f, y);
			glVertex2f(256.0f, y);
		}
	glEnd();
}

void playAnimation(float x, float y, float width, float height, int textureID, Animation* anim, float deltaTime, int* currentFrame, float* timeAccumulator, int flipped) {
    *timeAccumulator += deltaTime;

    if (*timeAccumulator >= anim->frameTime) {
        *timeAccumulator -= anim->frameTime;
        (*currentFrame)++;

        if (*currentFrame > anim->endFrame) {
            if (anim->loop) {
                *currentFrame = anim->startFrame;
            } else {
                *currentFrame = anim->endFrame;
            }
        }
    }
    int framesPerRow = anim->sheetWidth / anim->frameWidth;
    int frameX = (*currentFrame % framesPerRow) * anim->frameWidth;
    int frameY = (*currentFrame / framesPerRow) * anim->frameHeight;

    drawRect(x, y, width, height, textureID, frameX, frameY, anim->frameWidth, anim->frameHeight, anim->sheetWidth, anim->sheetHeight, flipped);
}
