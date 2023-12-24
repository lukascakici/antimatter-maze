#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_SCORES 5
#define MAX_NAME_LENGTH 50
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

typedef struct User {
    char name[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int scores[MAX_SCORES];
    int scoreCount;
} USER;

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

void printTopScores(USER user) {
    printf("Top Five Scores:\n");
    int i;

    for (i = 0; i < user.scoreCount; ++i) {
        printf("%d. %d\n", i + 1, user.scores[i]);
    }
}

void printGameInfo() {
    printf("Game Information:\n");
    printf("This game is a lot of fun!\n");
    // Add other information about the game.
}

void playGame() {
    printf("Starting the Game...\n");
    // Add the game start code here.
}

int main() {
    USER users[MAX_USERS];
    int userCount = 0, i;

    loadUsersFromFile(users, &userCount);

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
                if (currentUserIndex != -1) {
                    printGameInfo();
                } else {
                    // Create Account
                    if (userCount >= MAX_USERS) {
                        printf("Maximum number of users reached.\n");
                    } else {
                        USER newUser;

                        printf("Name: ");
                        scanf("%s", newUser.name);

                        printf("Last Name: ");
                        scanf("%s", newUser.username);

                        for (i = 0; i < userCount; ++i) {
                            if (strcmp(newUser.username, users[i].username) == 0) {
                                printf("This username is already in use. Please choose another one.\n");
                                break;
                            }
                        }

                        printf("Username: ");
                        scanf("%s", newUser.username);

                        printf("Password: ");
                        scanf("%s", newUser.password);

                        users[userCount++] = newUser;
                        saveUsersToFile(users, userCount);

                        printf("Account created successfully.\n");
                    }
                }
                break;
            case 3:
                if (currentUserIndex != -1) {
                    playGame();
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

    return 0;
}
