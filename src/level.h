#ifndef LEVEL_H
#define LEVEL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

void init_level(const char* filePath); 
void updateLevel(float deltaTime);
int getCell(int x, int y);
void setCell(int x, int y, int type);

#endif
