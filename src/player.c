#include "player.h"

Player player;

void init_player(void) {
    player.x = 16.0f;
    player.y = 32.0f;
    player.width = 16.0f;
    player.height = 16.0f;
    player.velocityY = 0.0f;
    player.doubleJumpAvailable = 1;
    player.flipped = 0;
    player.stage = 1;
    player.onGround = 1;
    player.anim = (Animation){4, 7, 4, 16, 16, 64, 32, 0.1f, 0.0f, 1};
}

bool collision(float x, float y, float width, float height) {
    int left = (int)(x / 16);
    int right = (int)((x + width - 1) / 16);
    int top = 8 - (int)(y / 16);
    int bottom = 8 - (int)((y + height - 1) / 16);

    for (int i = left; i <= right; i++) {
        for (int j = bottom; j <= top; j++) {
            if (getCell(i, j) == 1) return 1;
            if (getCell(i, j) == 2) setCell(i, j, 0);
        }
    }
    return 0;
}

void updatePlayer(float deltaTime) {
    float velocityX = 0.0f;
    player.velocityY += -290.0f * deltaTime; 

    if (player.stage == 1 && player.x > 256.0f) {
        setCamera(256.0f);
        player.stage = 2;
    } else if (player.stage == 2 && player.x < 256.0f) {
        setCamera(0.0f);
        player.stage = 1;
    }

    playAnimation(player.x, player.y, 1, &player.anim, deltaTime, player.flipped);

    bool moving = 0;
    if (getKey(1)) { 
        velocityX = -30.0f * deltaTime;
        player.flipped = 1;
        moving = 1;
    }
    if (getKey(3)) { 
        velocityX = 30.0f * deltaTime;
        player.flipped = 0;
        moving = 1;
    }
    player.anim.startFrame = moving ? 0 : 4;
    player.anim.endFrame = moving ? 3 : 7;
    
    if (getKey(4)) {
        if (player.onGround) {
            player.velocityY = 110.0f;
            player.onGround = 0;
            player.doubleJumpAvailable = 1;
        } else if (player.doubleJumpAvailable) {
            player.velocityY = 110.0f;
            player.doubleJumpAvailable = 0;
        }
    }

    player.x += velocityX;
    if (collision(player.x, player.y, player.width, player.height)) player.x -= velocityX;

    player.y += player.velocityY * deltaTime;
    if (collision(player.x, player.y, player.width, player.height)) {
        if (player.velocityY < 0) {
            player.onGround = 1;
            player.doubleJumpAvailable = 1;
        }
        player.y -= player.velocityY * deltaTime;
        player.velocityY = 0;
    } else {
        player.onGround = 0;
    }
    printf("%i\n", player.doubleJumpAvailable);
}