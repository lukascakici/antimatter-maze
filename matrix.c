#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ROWS 6
#define COLS 10

typedef struct {

}GAME;

void printMatrix(char matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void moveUp(char matrix[ROWS][COLS], int *currentRow, int *currentCol, int *counter) {
    matrix[*currentRow][*currentCol] = '0';
    *currentRow = (*currentRow - 1 + ROWS) % ROWS;
    if (matrix[*currentRow][*currentCol] == 'E') {
        (*counter)++;
    }
    if(matrix[*currentRow][*currentCol] == 'e'){
        (*counter)++;
    }
    matrix[*currentRow][*currentCol] = 'X';
}

void moveDown(char matrix[ROWS][COLS], int *currentRow, int *currentCol, int *counter) {
    matrix[*currentRow][*currentCol] = '0';
    *currentRow = (*currentRow + 1) % ROWS;
    if (matrix[*currentRow][*currentCol] == 'E' || matrix[*currentRow][*currentCol] == 'e') {
        (*counter)++;
    }
    matrix[*currentRow][*currentCol] = 'X';
}

void moveLeft(char matrix[ROWS][COLS], int *currentRow, int *currentCol, int *counter) {
    matrix[*currentRow][*currentCol] = '0';
    *currentCol = (*currentCol - 1 + COLS) % COLS;
    if (matrix[*currentRow][*currentCol] == 'E' || matrix[*currentRow][*currentCol] == 'e') {
        (*counter)++;
    }
    matrix[*currentRow][*currentCol] = 'X';
}

void moveRight(char matrix[ROWS][COLS], int *currentRow, int *currentCol, int *counter) {
    matrix[*currentRow][*currentCol] = '0';
    *currentCol = (*currentCol + 1) % COLS;
    if (matrix[*currentRow][*currentCol] == 'E' || matrix[*currentRow][*currentCol] == 'e') {
        (*counter)++;
    }
    matrix[*currentRow][*currentCol] = 'X';
}

void loadMatrixFromFile(char matrix[ROWS][COLS], const char *filename) {
    int i, j;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(1);
    }

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            fscanf(file, " %c", &matrix[i][j]);
        }
    }

    fclose(file);
}

int main() {
    char matrix[ROWS][COLS];

    loadMatrixFromFile(matrix, "map.txt");

    int currentRow = 2; // Starting row index of 'X'
    int currentCol = 1; // Starting column index of 'X'
    int counter = 0;     // Counter for elements passed by

    char direction;

    printf("Initial Matrix:\n");
    printMatrix(matrix);

    do {
        printf("\nEnter direction (u/d/l/r) or 'q' to quit: ");
        scanf(" %c", &direction);

        switch (direction) {
            case 'w':
                moveUp(matrix, &currentRow, &currentCol, &counter);
                break;
            case 's':
                moveDown(matrix, &currentRow, &currentCol, &counter);
                break;
            case 'a':
                moveLeft(matrix, &currentRow, &currentCol, &counter);
                break;
            case 'd':
                moveRight(matrix, &currentRow, &currentCol, &counter);
                break;
            case 'q':
                printf("Quitting the program.\n");
                break;
            default:
                printf("Invalid direction. Please enter 'w', 'a', 's', 'd', or 'q'.\n");
                break;
        }

        if (direction != 'q') {
            printf("Updated Matrix:\n");
            printMatrix(matrix);
            printf("Elements passed by: %d\n", counter);
        }

    } while (direction != 'q');

    return 0;
}