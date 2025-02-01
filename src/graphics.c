#include "graphics.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture textures[5] = {0, 0, 0};

float cameraX = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0.0f, 0.0f, width, height);
}

void loadTexture(const char* path, Texture* texture) {
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data) {
        printf("Failed to load texture\n");
    }

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

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

	texture->ID = textureID;
	texture->width = width;
	texture->height = height;
}

void loadTextures(void) {
	loadTexture("textures/blank.jpg", &textures[0]);
	loadTexture("textures/player.png", &textures[1]);
	loadTexture("levels/1/level.jpg", &textures[2]);
	loadTexture("textures/coin.png", &textures[3]);
	loadTexture("textures/coin_collect.png", &textures[4]);
}

void setCamera(float x) {
    cameraX = x;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(cameraX, cameraX + 256.0f, 0.0f, 144.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void init_graphics(void) {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to init GLAD\n");
	}

	glViewport(0.0f, 0.0f, 256.0f, 144.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(cameraX, cameraX + 256.0f, 0.0f, 144.0f, -1.0f, 1.0f);

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
	glfwSwapInterval(1);
	return window;
}

void drawRect(float x, float y, float width, float height, int textureID, float tx, float ty, float tw, float th, bool flipped) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[textureID].ID);

    float u1 = tx / textures[textureID].width;
    float v1 = ty / textures[textureID].height;
    float u2 = (tx + tw) / textures[textureID].width;
    float v2 = (ty + th) / textures[textureID].height;

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

void playAnimation(float x, float y, int textureID, Animation* anim, float deltaTime, bool flipped) {
    anim->timeAccumulator += deltaTime;

    if (anim->timeAccumulator >= anim->frameTime) {
        anim->timeAccumulator -= anim->frameTime;
        (anim->currentFrame)++;

        if (anim->currentFrame > anim->endFrame) {
            if (anim->loop) {
                anim->currentFrame = anim->startFrame;
            } else {
                anim->currentFrame = anim->endFrame;
            }
        }
    }
    int framesPerRow = anim->sheetWidth / anim->frameWidth;
    int frameX = (anim->currentFrame % framesPerRow) * anim->frameWidth;
    int frameY = (anim->currentFrame / framesPerRow) * anim->frameHeight;

    drawRect(x, y, anim->frameWidth, anim->frameHeight, textureID, frameX, frameY, anim->frameWidth, anim->frameHeight, flipped);
}
