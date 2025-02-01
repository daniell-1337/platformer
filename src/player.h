#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>
#include <math.h>
#include "graphics.h"
#include "input.h"
#include "level.h"

typedef struct {
    float x, y;
    float width, height;
    float velocityY;
    bool doubleJumpAvailable;
    bool flipped;
    bool onGround;
    int stage;
    Animation anim;
} Player;

void init_player(void);
void updatePlayer(float deltaTime);

#endif