#include "level.h"

enum Type {
    AIR,
    GROUND,
    COIN
};

enum Type cells[32][9];

Animation coinAnim = (Animation){0, 6, 0, 16, 16, 96, 16, 0.3f, 0.0f, 1};

void init_level(const char* filePath) {
    FILE * fptr;
    fptr = fopen(filePath, "r");

    if (fptr == NULL) printf("Level file not found\n");
    
    char line[100];
    int row = 0;
    int col;
    while (row < 9 && fgets(line, sizeof(line), fptr)) {
        col = 0;
        char* token = strtok(line, ",");
        while (token != NULL && col < 32) {
            cells[col][row] = atoi(token);
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    fclose(fptr);
    printf("%s loaded.\n", filePath);
}

void updateLevel(float deltaTime) {
    drawRect(0, 0, 512, 160, 2, 0, 0, 512, 160, 0);
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 9; j++) {
            if (getCell(i, j) == COIN) {
                playAnimation(i * 16, (8 - j) * 16, 3, &coinAnim, deltaTime, 0);
            }
        }
    }
}

int getCell(int x, int y) {
    return cells[x][y];
}

void setCell(int x, int y, int type) {
    cells[x][y] = type;
}
