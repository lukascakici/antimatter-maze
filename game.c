#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_USERS 10
#define MAX_SCORES 5
#define MAX_NAME_LENGTH 50
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define ROWS 6
#define COLS 10

typedef struct User {
    char name[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int scores[MAX_SCORES];
    int scoreCount;
} USER;

//function declarations---------------------------------------------------------------------------------------
void saveUsersToFile(USER users[], int userCount);
void loadUsersFromFile(USER users[], int *userCount);
void printTopScores(USER user);
void printGameInfo();
void playGame(char matrix[ROWS][COLS]);
void printMatrix(char matrix[ROWS][COLS]);
void moveUp(char matrix[ROWS][COLS], int *currentRow, const int *currentCol);
void moveDown(char matrix[ROWS][COLS], int *currentRow, const int *currentCol);
void moveLeft(char matrix[ROWS][COLS], const int *currentRow, int *currentCol);
void moveRight(char matrix[ROWS][COLS], const int *currentRow, int *currentCol);
void loadMatrixFromFile(char matrix[ROWS][COLS], const char *filename);
//-------------------------------------------------------------------------------------------------------------





//functions-------------------------------------------------------------------------------------------------
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
void printMatrix(char matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c\t", matrix[i][j]);
        }
        printf("\n");
    }
}
void printTopScores(USER user) {
    printf("Top Five Scores:\n");
    int i;

    for (i = 0; i < 5; ++i) {
        printf("%d. %d\n", i + 1, user.scores[i]);
    }
}

void moveUp(char matrix[ROWS][COLS], int *currentRow, const int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    if(matrix[(*currentRow - 1 + ROWS) % ROWS][*currentCol] != '1'){
        *currentRow = (*currentRow - 1 + ROWS) % ROWS;
    }
    matrix[*currentRow][*currentCol] = 'X';
}
void moveDown(char matrix[ROWS][COLS], int *currentRow, const int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    if(matrix[(*currentRow + 1) % ROWS][*currentCol] != '1'){
        *currentRow = (*currentRow + 1) % ROWS;
    }
    matrix[*currentRow][*currentCol] = 'X';
}
void moveLeft(char matrix[ROWS][COLS], const int *currentRow, int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    if(matrix[*currentRow][(*currentCol - 1 + COLS) % COLS] != '1'){
        *currentCol = (*currentCol - 1 + COLS) % COLS;
    }
    matrix[*currentRow][*currentCol] = 'X';
}
void moveRight(char matrix[ROWS][COLS], const int *currentRow, int *currentCol) {
    matrix[*currentRow][*currentCol] = '0';
    if(matrix[*currentRow][(*currentCol + 1) % COLS] != '1'){
        *currentCol = (*currentCol + 1) % COLS;
    }
    matrix[*currentRow][*currentCol] = 'X';
}

void loadUsersFromFile(USER users[], int *userCount) {
    FILE *file = fopen("users.bin", "rb");
    int i;

    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(1);
    }

    fread(userCount, sizeof(int), 1, file); // kullanici sayisini cekicez

    for (i = 0; i < *userCount; ++i) {
        fread(&users[i], sizeof(USER), 1, file); // her kullaniciyi tek tek cekicez
    }

    fclose(file);
}
void saveUsersToFile(USER users[], int userCount) {
    FILE *file = fopen("users.bin", "wb");
    int i;

    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(1);
    }

    fwrite(&userCount, sizeof(int), 1, file); // kullanici sayisini yazicaz

    for (i = 0; i < userCount; ++i) {
        fwrite(&users[i], sizeof(USER), 1, file); // her kullaniciyi tek tek yazicaz
    }

    fclose(file);
}


void playGame(char matrix[ROWS][COLS]) {

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

}


int findXPosition(const char *file_path) {
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Dosya açma hatası.\n");
        return 0;
    }

    char line[100];  // Satırı okumak için yeterli büyüklükte bir dizi
    int row = 1;     // Satır numarası

    // Satırları oku
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, " ");
        int column = 1;  // Sütun numarası

        // Satırdaki elemanları kontrol et
        while (token != NULL) {
            if (token[0] == 'X') {
                printf("X harfi bulundu! Satır: %d, Sütun: %d\n", row, column);
                fclose(file);
                return row, column;
            }

            token = strtok(NULL, " ");
            column++;
        }

        row++;
    }

    printf("X harfi bulunamadı.\n");
    fclose(file);
}
//---------------------------------------------------------------------------------------------------------------


int main() {
    USER users[MAX_USERS];
    int userCount = 0, i;

    char matrix[ROWS][COLS];

    loadMatrixFromFile(matrix, "map.txt");
    loadUsersFromFile(users, &userCount);
    findXPosition("map.txt");

    int choice = 0;
    int currentUserIndex = -1;

    while (choice != 3) {
        printf("\n");

        if (currentUserIndex != -1) {
            printf("1. View Top Scores\n");
            printf("2. Get Game Information\n");
            printf("3. Play the Game\n");
            printf("4. Logout (%s)\n", users[currentUserIndex].name);
        } else {
            printf("1. Login\n");
            printf("2. Create Account\n");
            printf("3. Exit\n");
        }

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (currentUserIndex != -1) {
                    printTopScores(users[currentUserIndex]);
                } else {
                    // Login
                    char enteredUsername[MAX_USERNAME_LENGTH];
                    char enteredPassword[MAX_PASSWORD_LENGTH];

                    printf("Username: ");
                    scanf("%s", enteredUsername);

                    printf("Password: ");
                    scanf("%s", enteredPassword);

                    for (i = 0; i < userCount; ++i) {
                        if (strcmp(enteredUsername, users[i].username) == 0 && strcmp(enteredPassword, users[i].password) == 0) {
                            printf("Login successful. Welcome, %s!\n", users[i].name);
                            currentUserIndex = i;
                            break;
                        }
                    }

                    if (currentUserIndex == -1) {
                        printf("Incorrect username or password. Please try again.\n");
                    }
                }
                break;
            case 2:
                if (userCount >= MAX_USERS) {
                    printf("Maximum number of users reached.\n");
                } else {
                    USER newUser;

                    // Get user information
                    printf("Name: ");
                    scanf("%s", newUser.name);

                    printf("Last Name: ");
                    scanf("%s", newUser.username);

                    // Check if the username is already in use
                    while (1) {
                        int usernameTaken = 0;
                        printf("Username: ");
                        scanf("%s", newUser.username);

                        for (i = 0; i < userCount; ++i) {
                            if (strcmp(newUser.username, users[i].username) == 0) {
                                printf("This username is already in use. Please choose another one.\n");
                                usernameTaken = 1;
                            }
                        }

                        if (!usernameTaken) {
                            break;
                        }
                    }

                    printf("Password: ");
                    scanf("%s", newUser.password);

                    // Add the new user to the array
                    users[userCount++] = newUser;

                    // Save users to file
                    saveUsersToFile(users, userCount);

                    printf("Account created successfully.\n");
                }
                break;
            case 3:
                if (currentUserIndex != -1) {
                    playGame(matrix);
                } else {
                    printf("Exiting...\n");
                }
                break;
            case 4:
                if (currentUserIndex != -1) {
                    // Logout
                    currentUserIndex = -1;
                    printf("Logged out.\n");
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    system("pause");
    return 0;
}