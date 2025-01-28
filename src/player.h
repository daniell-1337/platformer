#ifndef PLAYER_H
#define PLAYER_H
#include "graphics.h"
#include <stdbool.h>
#include "input.h"
#include "level.h"

typedef struct {
    float x, y;
    float width, height;
    float velocity;
    float jumpVelocity;
    bool doubleJumpAvailable;
    bool flipped;
    Animation anim;
} Player;

void init_player(void);
void updatePlayer(float deltaTime);

#endif