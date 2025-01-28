#include "player.h"

Player player;
static bool jumpPressed = 0;

void init_player(void) {
    player.x = 16.0f;
    player.y = 32.0f;
    player.width = 16.0f;
    player.height = 16.0f;
    player.velocity = 40.0f;
    player.jumpVelocity = 0.0f;
    player.doubleJumpAvailable = 1;
    player.flipped = 0;
    player.anim = (Animation){0, 3, 0, 16, 16, 64, 32, 0.1f, 0.0f, 1};
}

int checkCollision(float x, float y, float width, float height) {
    int left = (int)(x / 16);
    int right = (int)((x + width - 1) / 16);
    int top = 8 - (int)(y / 16);
    int bottom = 8 - (int)((y + height - 1) / 16);

    for (int i = left; i <= right; i++) {
        for (int j = bottom; j <= top; j++) {
            if (getCell(i, j) == 1) return 1;
        }
    }
    return 0;
}

void updatePlayer(float deltaTime) {
    float newX = player.x;
    float newY = player.y;
    bool moving = 0;
    if (getKey(1)) { 
        newX -= player.velocity * deltaTime;
        moving = 1;
        player.flipped = 1;
    }
    if (getKey(3)) { 
        newX += player.velocity * deltaTime;
        moving = 1;
        player.flipped = 0;
    }

    int onGround = checkCollision(player.x, player.y - 1, player.width, player.height);

    if (onGround) {
        player.jumpVelocity = 0.0f;
        player.doubleJumpAvailable = 1;
    }
    
    if (getKey(4) && !jumpPressed) {
        jumpPressed = 1;
        if (onGround || player.doubleJumpAvailable) {
            player.jumpVelocity = 3.0f;
            if (!onGround) player.doubleJumpAvailable = 0;
        }
    }
    if (!getKey(4)) {
        jumpPressed = 0;
    }
    
    player.jumpVelocity += -13.0f * deltaTime;
    newY += player.jumpVelocity;

    if (!checkCollision(newX, player.y, player.width, player.height)) player.x = newX;
    if (!checkCollision(player.x, newY, player.width, player.height)) {
        player.y = newY;
    } else {
        player.jumpVelocity = 0.0f;
    }
    
    player.anim.startFrame = moving ? 0 : 4;
    player.anim.endFrame = moving ? 3 : 7;
    
    playAnimation(player.x, player.y, 1, &player.anim,deltaTime, player.flipped);
}