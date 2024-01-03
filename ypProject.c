#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>


#define MAX_USERS 10
#define MAX_NAME_LENGTH 30
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_SCORES 5

// kullanici bilgilerini tutan structure yapisi
typedef struct User {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int scores[MAX_SCORES];
    int scoreCount;
} USER;

//oyunda kullanmak icin oyun haritasini belirten struct yapisi
typedef struct {
    char **matrix; // 2 boyutlu matris
    int rows; //haritanin satir sayisi
    int cols; //haritanin sutun sayisi
    int userRow; //o an kullanicinin oldugu satir
    int userCol; //o an kullanicinin oldugu sutun
    int collectedP; // toplanan Proton(P) sayisi
    int collectedp; // toplanan karsit proton(p) sayisi
    int collectede; // toplanan elektron(e) sayisi
    int collectedE; // toplanan karsit elektron(E) sayisi
    int antihydrogen; //karist proton ve karsit elektronlarla olusturulan karsit hidrojen sayisi
    time_t startTime; //zaman limiti icin oyun basladigi anki sureyi temsil ediyor
    int timeLimit;  // belirledigimiz sure limiti
    int autoplay; //otomatik oynama modunun secilip secilmediginin belirteci
    int currentScore;
} GameMap;



// function declarations
int showMainMenu(); //ana menuyu bastiran fonksiyon
void login(USER users[], int *loggedInUser, int *userCount); //giris yapma fonksiyonu
int showLoggedInMenu(USER users[], int currentUserIndex); //giris yapildiktan sonraki menuyu bastiran fonksiyon
void createAccount(USER users[], int *userCount); // hesap olusturma fonksiyonu
void readUserData(USER users[], int *userCount); //dosyadan kullanici bilgileri okuma fonksiyonu
void writeUserData(const USER users[], int userCount); //dosyaya kullanici bilgilerini yazma fonksiyonu
int play(); //oyunu oynama fonksiyonu
bool moveUser(GameMap *gameMap, char move); //harita uzerinde hareket etme fonksiyonu
void initializeMap(GameMap *gameMap, const char *filename); //dosyadan okunan haritayi initialize eden fonksiyon
void displayMap(const GameMap *gameMap); //haritayi bastiran fonksiyon
void freeMap(GameMap *gameMap); //dinamik olarak bellekte ayrilan yerlerin temizlenmesi icin gereken fonksiyon
void printResults(const GameMap *gameMap); //toplanan parcaciklar ve olusturulan karsit hidrojen sayisini bastiran fonksiyon
char autoplayMove(const GameMap *gameMap); //otomatik oynama modu
void updateScore(USER *user, int score);




int main() {
    USER users[MAX_USERS];  // kullanicilari tuttugumuz array
    int loggedInUser = -1;  // kullanicinin giris yapip yapmadigini belirten index
    int userCount = 0;  // kayitli kullanici sayisi(program her calistirildiginda dosyadan okunan degere gore guncellenir)

    readUserData(users, &userCount);

    int choice;
    do {
        if (loggedInUser != -1) {
            int logoutFlag = showLoggedInMenu(users, loggedInUser);
            if (logoutFlag) {
                loggedInUser = -1;
            }
        } else {
            choice = showMainMenu();

            switch (choice) {
                case 1:
                    login(users, &loggedInUser, &userCount);
                    break;
                case 2:
                    createAccount(users, &userCount);
                    writeUserData(users, userCount);
                    break;
                case 3:
                    printf("Exiting the game. Goodbye!\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } while (choice != 3);

    return 0;
}

int showMainMenu() {
    int choice;
    printf("===== Antimatter Maze Main Menu =====\n");
    printf("1. Login\n");
    printf("2. Create Account\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

void login(USER users[], int *loggedInUser, int *userCount) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int i;

    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    // Check if the entered credentials match any user account
    for (i = 0; i < *userCount; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            *loggedInUser = i;
            printf("Login successful! Welcome, %s!\n", username);
            return;
        }
    }

    printf("Invalid username or password. Please try again.\n");
}

void createAccount(USER users[], int *userCount) {
    if (*userCount == MAX_USERS) {
        printf("Maximum number of users reached. Cannot create more accounts.\n");
        return;
    }

    int i;
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    // Ask for user's name and surname
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your surname: ");
    scanf("%s", surname);

    printf("Enter a new username: ");
    scanf("%s", username);

    // Check if the username already exists
    for (i = 0; i < *userCount; ++i) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Please choose another username.\n");
            return;
        }
    }

    printf("Enter a password: ");
    scanf("%s", password);

    // Initialize additional user information
    strcpy(users[*userCount].name, name);
    strcpy(users[*userCount].surname, surname);
    strcpy(users[*userCount].username, username);
    strcpy(users[*userCount].password, password);
    users[*userCount].scoreCount = 0;  // Initialize score count

    *userCount += 1;

    printf("Account created successfully! You can now login.\n");
}

int showLoggedInMenu(USER users[], int currentUserIndex) {
    int choice;

    printf("\n");
    printf("1. View Top Scores\n");
    printf("2. Get Game Information\n");
    printf("3. Play the Game\n");
    printf("4. Logout (%s)\n", users[currentUserIndex].name);

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // Add code for viewing top scores
            break;
        case 2:
            // Add code for getting game information
            break;
        case 3:
            play(); //oyunu baslatma fonksiyonu
            break;
        case 4:
            printf("Logging out. Goodbye, %s!\n", users[currentUserIndex].name);
            return 1;
        default:
            printf("Invalid choice. Please try again.\n");
    }

    return 0;  // No logout, continue with the session
}

void readUserData(USER users[], int *userCount) {
    int i;
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        return;
    }

    while (fscanf(file, "%s %s %s %s %d", users[*userCount].name, users[*userCount].surname, users[*userCount].username, users[*userCount].password, &users[*userCount].scoreCount) == 5) {
        // Read user scores
        for (i = 0; i < users[*userCount].scoreCount; ++i) {
            fscanf(file, "%d", &users[*userCount].scores[i]);
        }

        (*userCount)++;
        if (*userCount >= MAX_USERS) {
            break;
        }
    }

    fclose(file);
}

void writeUserData(const USER users[], int userCount) {
    int i, j;
    FILE *file = fopen("user_data.txt", "w");
    if (file == NULL) {
        printf("Error opening user_data.txt for writing.\n");
        return;
    }

    for (i = 0; i < userCount; ++i) {
        fprintf(file, "%s %s %s %s %d", users[i].name, users[i].surname, users[i].username, users[i].password, users[i].scoreCount);

        // Write user scores
        for (j = 0; j < users[i].scoreCount; ++j) {
            fprintf(file, " %d", users[i].scores[j]);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

void initializeMap(GameMap *gameMap, const char *filename) {
    int i, j;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // satir ve sutun sayisinin okuma
    if (fscanf(file, "%d %d", &gameMap->rows, &gameMap->cols) != 2) {
        printf("Error reading dimensions from the file.\n");
        exit(EXIT_FAILURE);
    }

    // matris icin bellekte yer ayirma
    gameMap->matrix = (char **)malloc(gameMap->rows * sizeof(char *));
    for (i = 0; i < gameMap->rows; i++) {
        gameMap->matrix[i] = (char *)malloc(gameMap->cols * sizeof(char));
    }

    // Matrisi dosyadan okuma
    for (i = 0; i < gameMap->rows; i++) {
        for (j = 0; j < gameMap->cols; j++) {
            if (fscanf(file, " %c", &gameMap->matrix[i][j]) != 1) {
                printf("Error reading matrix from the file.\n");
                exit(EXIT_FAILURE);
            }

            // x ile gosterilen noktayi kullanicinin baslangic noktasi yapma
            if (gameMap->matrix[i][j] == 'X') {
                gameMap->userRow = i;
                gameMap->userCol = j;
            }
        }
    }

    fclose(file);
    gameMap->startTime = time(NULL);
    gameMap->timeLimit = 300;  // oyun suresinin saniye cinsinden ne kadar olacagini ayarlama

}

void displayMap(const GameMap *gameMap) {
    printf("\n");
    int i, j;
    for (i = 0; i < gameMap->rows; i++) {
        for (j = 0; j < gameMap->cols; j++) {
            printf("%c ", gameMap->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool moveUser(GameMap *gameMap, char move) {
    int newRow = gameMap->userRow;
    int newCol = gameMap->userCol;

    switch (move) {
        case 'W':
        case 'w':
        case 72:// yukari yonlu ok tusu
            if(gameMap->userRow != 0){
                newRow--;
            }else{
                printf("Invalid move. Stay within the boundaries or avoid walls.\n");
            }
            break;
        case 'A':
        case 'a':
        case 75:  // sol yonlu ok tusu
            if(gameMap->userCol != 0){
                newCol--;
            }else{
                printf("Invalid move. Stay within the boundaries or avoid walls.\n");
            }
            break;
        case 'S':
        case 's':
        case 80:  // asagi yonlu ok tusu
            if(gameMap->userRow != gameMap->rows-1){
                newRow++;
            }else{
                printf("Invalid move. Stay within the boundaries or avoid walls.\n");
            }
            break;
        case 'D':
        case 'd':
        case 77:  // sag yonlu ok tusu
            if(gameMap->userCol != gameMap->cols){
                newCol++;
            }else{
                printf("Invalid move. Stay within the boundaries or avoid walls.\n");
            }
            break;
        default:
            printf("Invalid move. Use W/A/S/D or arrow keys.\n");
            return true;
    }

    if(gameMap->matrix[newRow][newCol] == 'P'){
        gameMap->collectedP++;
    }
    if(gameMap->matrix[newRow][newCol] == 'p'){
        gameMap->collectedp++;
    }
    if(gameMap->matrix[newRow][newCol] == 'E'){
        gameMap->collectedE++;
    }
    if(gameMap->matrix[newRow][newCol] == 'e'){
        gameMap->collectede++;
    }

    // kullanicinin harita sinirlarindan disari cikmamasi ve duvarlardan gecmemesi icin kontrol
    if (newRow >= 0 && newRow < gameMap->rows && newCol >= 0 && newCol < gameMap->cols &&
        gameMap->matrix[newRow][newCol] != '1') {
        char currentCell = gameMap->matrix[newRow][newCol];

        // kullanicinin oldugu noktayi guncellemek
        gameMap->matrix[gameMap->userRow][gameMap->userCol] = ' ';
        gameMap->userRow = newRow;
        gameMap->userCol = newCol;
        gameMap->matrix[gameMap->userRow][gameMap->userCol] = 'X';

        // kullanici karadelik ya da cikis noktasina ulasti mi diye kontrol etme
        if (currentCell == 'K' || currentCell == 'C') {
            printf("Congratulations! You reached the exit.\n");
            return false; // oyun biter
        }
    } else {
        printf("Invalid move. Stay within the boundaries or avoid walls.\n");
    }

    return true; //oyun devam eder
}

void freeMap(GameMap *gameMap) {

    int i;
    for (i = 0; i < gameMap->rows; i++) {
        free(gameMap->matrix[i]);
    }
    free(gameMap->matrix);
}

int play() {
    GameMap gameMap;

    int choice;
    printf("Choose your play mode:\n");
    printf("1. Play manually\n");
    printf("2. Autoplay\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    initializeMap(&gameMap, "map.txt");

    gameMap.collectedP = 0;
    gameMap.collectedp = 0;
    gameMap.collectedE = 0;
    gameMap.collectede = 0;

    //kullanicinin oynama sekli secimi
    if (choice == 2) {
        gameMap.autoplay = 1;  // otomatik oynama modunu aktiflestir
    }else{
        gameMap.autoplay = 0;
    }

    // Game loop
    char move;
    do{
        displayMap(&gameMap);

        if (gameMap.autoplay) {
            move = autoplayMove(&gameMap);
            //100.000 = 0.1 saniye olacak şekilde
            usleep(700000);
        } else {
            printf("Enter your move (W/A/S/D): ");
            scanf(" %c", &move);
        }

        // sure sinirini kontrol etme
        time_t currentTime = time(NULL);
        if (currentTime - gameMap.startTime >= gameMap.timeLimit) {
            printf("Time's up!\n");
            break; // eger sure dolduysa bir sonraki harekette oyunu bitir
        }

    }while(moveUser(&gameMap, move));

    printResults(&gameMap);

    //updateScore(user, gameMap.currentScore);

    freeMap(&gameMap);

    return 0;
}

char autoplayMove(const GameMap *gameMap) {
    // rastgele bir yön seçip ilerleyecek
    char directions[] = {'W', 'A', 'S', 'D'};
    int numDirections = sizeof(directions) / sizeof(directions[0]);
    char nextMove = directions[rand() % numDirections];

    return nextMove;
}

void printResults(const GameMap *gameMap) {
    printf("\n");
    printf("Collected 'P': %d\n", gameMap->collectedP);
    printf("Collected 'p': %d\n", gameMap->collectedp);
    printf("Collected 'E': %d\n", gameMap->collectedE);
    printf("Collected 'e': %d\n", gameMap->collectede);

    // karsit protondan, proton sayisini ; karsit elektrondan elektron sayisini cikart
    int remainingp = gameMap->collectedp - gameMap->collectedP;
    if((gameMap->collectedp - gameMap->collectedP) < 0){
        remainingp = 0;
    }
    int remainingE = gameMap->collectedE - gameMap->collectede;
    if((gameMap->collectedE - gameMap->collectede) < 0){
        remainingE = 0;
    }

    // kalan karsit elektron ve karsit proton sayilarina gore karsit hidrojen sayisini belirle
    int antihydrogen = remainingp < remainingE ? remainingp : remainingE;

    printf("\nAntihydrogen created: %d\n", antihydrogen);
}