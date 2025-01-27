#include "level.h"

enum Type {
    AIR,
    GROUND
};

typedef struct {
    enum Type type;
} Cell;

Cell cells[16][9] = {0};

void init_level(const char* filePath) {
    FILE * fptr;
    fptr = fopen(filePath, "r");

    if (fptr == NULL) printf("Level file not found\n");
    
    char line[64];
    int row = 0;
    int col;
    while (row < 9 && fgets(line, sizeof(line), fptr)) {
        col = 0;
        char* token = strtok(line, ",");
        while (token != NULL && col < 16) {
            cells[col][row].type = atoi(token);
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    fclose(fptr);
    printf("%s loaded.\n", filePath);
}

void updateLevel(void) {
    drawRect(0, 0, 256, 256, 2, 0, 0, 256, 256, 256, 256, 0);
}

int getCell(int x, int y) {
    return cells[x][y].type;
}
