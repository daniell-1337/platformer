#include "player.h"

Player player;

static int jumpPressed = 0;

void init_player(void) {
    player.x = 16.0f;
    player.y = 32.0f;
    player.width = 16.0f;
    player.height = 16.0f;
    player.velocity = 40.0f;
    player.gravity = -13.0f;
    player.jumpVelocity = 0.0f;
    player.isJumping = 0;
    player.coyoteTime = 0.1f;
    player.coyoteTimer = 0.0f;
    player.anim = (Animation){4, 7, 16, 16, 64, 32, 0.1f, 1};
    player.timeAccumulator = 0.0f;
    player.currentFrame = 0;
    player.flipped = 0;
    player.canDoubleJump = 1;
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

    int moving = 0;
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
        player.coyoteTimer = player.coyoteTime;
        player.isJumping = 0;
        player.canDoubleJump = 1;
        if (player.jumpVelocity < 0) player.jumpVelocity = 0.0f;
    } else {
        player.coyoteTimer -= deltaTime;
    }

    if (!getKey(4)) {
        jumpPressed = 0;
    }

    if (getKey(4) && !jumpPressed) {
        jumpPressed = 1;
        
        if (onGround || player.coyoteTimer > 0.0f) {
            player.jumpVelocity = 3.0f;
            player.isJumping = 1;
            player.coyoteTimer = 0.0f;
        } else if (player.isJumping && player.canDoubleJump) {
            player.jumpVelocity = 3.0f;
            player.isJumping = 1;
            player.canDoubleJump = 0;
        }
    }

    player.jumpVelocity += player.gravity * deltaTime;
    newY += player.jumpVelocity;

    if (!checkCollision(newX, player.y, player.width, player.height)) {
        player.x = newX;
    }

    if (!checkCollision(player.x, newY, player.width, player.height)) {
        player.y = newY;
    } else {
        if (player.jumpVelocity < 0) {
            player.y = (int)((player.y + player.height) / 16) * 16 - player.height;
        }
        player.jumpVelocity = 0.0f;
        player.isJumping = 0;
    }
    
    if (moving) {
        player.anim.startFrame = 0;
        player.anim.endFrame = 3;
    } else {
        player.anim.startFrame = 4;
        player.anim.endFrame = 7;
    }
    playAnimation(player.x, player.y, player.width, player.height, 1, &player.anim,
    deltaTime, &player.currentFrame, &player.timeAccumulator, player.flipped);
}