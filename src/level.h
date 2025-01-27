#ifndef LEVEL_H
#define LEVEL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

void init_level(const char* filePath); 
void updateLevel(void);
int getCell(int x, int y);

#endif
