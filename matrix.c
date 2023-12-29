#include <stdio.h>
#include <stdlib.h>


#define ROWS 6
#define COLS 10

typedef struct {

}MAP;


/*
int rowColNum() {
    FILE *dosya;
    char karakter;
    int satir_sayisi = 1, eleman_sayisi = 0, sutun_sayisi;

    // Dosyayı aç
    dosya = fopen("map.txt", "r");

    // Dosya açıldıysa işlemleri yap
    if (dosya != NULL) {
        while ((karakter = fgetc(dosya)) != EOF) {
            // Karakter '\n' (newline) ise satır sayısını artır
            if (karakter == '\n') {
                satir_sayisi++;
            }
                // Boşluk, tab veya yeni satır olmayan diğer karakterlerse eleman sayısını artır
            else if (karakter != ' ' && karakter != '\t') {
                eleman_sayisi++;
            }
        }

        sutun_sayisi = eleman_sayisi / satir_sayisi;
        // Dosyayı kapat
        fclose(dosya);


    } else {
        // Dosya açılamadıysa hata mesajını yazdır
        printf("Dosya acilamadi!\n");
    }
}
*/




void printMatrix(char matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void moveUp(char matrix[ROWS][COLS], int *currentRow, const int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    *currentRow = (*currentRow - 1 + ROWS) % ROWS;
    matrix[*currentRow][*currentCol] = 'X';
}

void moveDown(char matrix[ROWS][COLS], int *currentRow, const int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    *currentRow = (*currentRow + 1) % ROWS;
    matrix[*currentRow][*currentCol] = 'X';
}

void moveLeft(char matrix[ROWS][COLS], const int *currentRow, int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    *currentCol = (*currentCol - 1 + COLS) % COLS;
    matrix[*currentRow][*currentCol] = 'X';
}

void moveRight(char matrix[ROWS][COLS], const int *currentRow, int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    *currentCol = (*currentCol + 1) % COLS;
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

/*
int** matrix(int satir, int sutun) {
    int** matrix = (int **)malloc(satir * sizeof(int *));
    for (int i = 0; i < satir; i++) {
        matrix[i] = (int *)malloc(sutun * sizeof(int));
    }
    return matrix;
}

void freeMat(int** matrix, int satir) {
    for (int i = 0; i < satir; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
*/


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
                moveUp(matrix, &currentRow, &currentCol);
                break;
            case 's':
                moveDown(matrix, &currentRow, &currentCol);
                break;
            case 'a':
                moveLeft(matrix, &currentRow, &currentCol);
                break;
            case 'd':
                moveRight(matrix, &currentRow, &currentCol);
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