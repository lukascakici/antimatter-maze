#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>


#define MAX_USERS 10
#define MAX_NAME_LENGTH 30
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_SCORES 5
#ifdef _WIN32
    #define CLEAR_CONSOLE system("cls")
#elif defined(__APPLE__) || defined(__MACH__)
    #define CLEAR_CONSOLE system("clear")
#endif


// kullanici bilgilerini tutan structure yapisi
typedef struct User {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int score;
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
} GameMap;



// function declarations
int showMainMenu(); //ana menuyu bastiran fonksiyon
void login(USER users[], int *loggedInUser, int *userCount); //giris yapma fonksiyonu
int showLoggedInMenu(USER users[], int currentUserIndex); //giris yapildiktan sonraki menuyu bastiran fonksiyon
void createAccount(USER users[], int *userCount); // hesap olusturma fonksiyonu
void readUserData(USER users[], int *userCount); //dosyadan kullanici bilgileri okuma fonksiyonu
void writeUserData(const USER users[], int userCount); //dosyaya kullanici bilgilerini yazma fonksiyonu
int play(USER *user); //oyunu oynama fonksiyonu
bool moveUser(GameMap *gameMap, char move); //harita uzerinde hareket etme fonksiyonu
void initializeMap(GameMap *gameMap, const char *filename); //dosyadan okunan haritayi initialize eden fonksiyon
void displayMap(const GameMap *gameMap); //haritayi bastiran fonksiyon
void freeMap(GameMap *gameMap); //dinamik olarak bellekte ayrilan yerlerin temizlenmesi icin gereken fonksiyon
void printResults(GameMap *gameMap); //toplanan parcaciklar ve olusturulan karsit hidrojen sayisini bastiran fonksiyon
char autoplayMove(const GameMap *gameMap); //otomatik oynama modu
void info(); // oyun hakkinda bilgileri ekrana bastirir
void printTopScores(); //en yuksek 5 skoru bastirir
int calculateScore(GameMap gameMap); //skoru hesaplama fonksiyonu
void writeScoreToFile(const USER *user); //kullanicinin adi ve skorunu scores.txt dosyasina yazdirir


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
                    info();
                    break;
                case 4:
                    printf("Exiting the game. Goodbye!\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } while (choice != 4);

    system("pause");
    return 0;
}

void writeScoreToFile(const USER *user) {
    FILE *file;

    //dosyayi ac
    file = fopen("scores.txt", "a");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    //kullanici adi ve skoru dosyaya yaz
    fprintf(file, "%s %d\n", user->username, user->score);

    //veri kaybi olmamasi icin dosyayi kapa
    fclose(file);
}

void info(){
    CLEAR_CONSOLE;
    printf("WELCOME TO THE ANTIMATTER MAZE!\n");
    printf("The paths in the maze contain the following particles.\n");
    printf("P : proton\n"
           "p : anti-proton\n"
           "e : electron\n"
           "E : anti-electron\n");
    printf("To produce anti-hydrogen, you only need to have p and E particles at the exit of the maze.\n");
    printf("If a particle and its antiparticle come together, they annihilate each other.\n");
    printf("Black holes are indicated by K. If you pass through black holes, the game ends\n");
    printf("Walls are indicated by 1 and it is not possible to pass through the walls\n");
    printf("To move around the maze use 'w' 'a' 's' 'd' keys. Or if you want to quit use 'q'.\n");
    usleep(10000000); // yazilanlarin okunmasi icin belli bir sure bekle.
}

int showMainMenu() {
    int choice;
    printf("\n===== Antimatter Maze Main Menu =====\n");
    printf("1. Login\n");
    printf("2. Create Account\n");
    printf("3. Get Game Information\n");
    printf("4. Exit\n");
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

    //girilen bilgilerle eslesen kullanici var mi yok mu kontrol et
    for (i = 0; i < *userCount; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            *loggedInUser = i;
            printf("\nLogin successful! Welcome, %s!\n", username);
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

    // kullanicidan bilgileri iste
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your surname: ");
    scanf("%s", surname);

    printf("Enter a new username: ");
    scanf("%s", username);

    // kullanici adi kullaniliyorsa tekrar iste
    for (i = 0; i < *userCount; ++i) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Please choose another username.\n");
            printf("Enter a new username: ");
            scanf("%s", username);
        }
    }

    printf("Enter a password: ");
    scanf("%s", password);

    //girilen bilgilerle kullanici kaydi olustur
    strcpy(users[*userCount].name, name);
    strcpy(users[*userCount].surname, surname);
    strcpy(users[*userCount].username, username);
    strcpy(users[*userCount].password, password);

    *userCount += 1;

    printf("Account created successfully! You can now login.\n");
}

int showLoggedInMenu(USER users[], int currentUserIndex) {
    int choice;

    printf("\n");
    printf("1. View Top Scores\n");
    printf("2. Play the Game\n");
    printf("3. Logout (%s)\n", users[currentUserIndex].name);

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printTopScores(); //en yuksek 5 skoru bastir
            break;
        case 2:
            play(&users[currentUserIndex]); //oyunu baslatma fonksiyonu
            break;
        case 3:
            printf("Logging out. Goodbye, %s!\n", users[currentUserIndex].name);
            return 1;
        default:
            printf("Invalid choice. Please try again.\n");
    }

    return 0;
}

void readUserData(USER users[], int *userCount) {
    int i;
    FILE *file = fopen("user_data.txt", "rb");
    if (file == NULL) {
        return;
    }

    while (fscanf(file, "%s %s %s %s", users[*userCount].name, users[*userCount].surname, users[*userCount].username, users[*userCount].password) == 4) {
        (*userCount)++;
        if (*userCount >= MAX_USERS) {
            break;
        }
    }

    fclose(file);
}

void writeUserData(const USER users[], int userCount) {
    int i;
    FILE *file = fopen("user_data.txt", "wb");
    if (file == NULL) {
        printf("Error opening user_data.txt for writing.\n");
        return;
    }

    for (i = 0; i < userCount; ++i) {
        fprintf(file, "%s %s %s %s\n", users[i].name, users[i].surname, users[i].username, users[i].password);
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
            if(gameMap->userRow != 0){ //kullanicinin haritanin disina cikmamasi ve bellek hatasi yaratmamasi icin kontrol
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
        case 'Q':
        case 'q':
            return false;
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
            printf("\nCongratulations! You reached the exit.\n");
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

int play(USER *user) {

    GameMap gameMap;

    int choice;
    printf("\nChoose your play mode:\n");
    printf("1. Play manually\n");
    printf("2. Autoplay\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    initializeMap(&gameMap, "map.txt");

    gameMap.collectedP = 0;
    gameMap.collectedp = 0;
    gameMap.collectedE = 0;
    gameMap.collectede = 0;
    gameMap.antihydrogen = 0;

    //kullanicinin oynama sekli secimi
    if (choice == 2) {
        gameMap.autoplay = 1;  // otomatik oynama modunu aktiflestir
    }else{
        gameMap.autoplay = 0;
    }

    CLEAR_CONSOLE;

    // Game loop
    char move;
    do{
        displayMap(&gameMap);

        if (gameMap.autoplay) {
            move = autoplayMove(&gameMap);
            //1.000.000 = 1 saniye olacak şekilde
            usleep(700000);
        } else {
            printf("Enter your move (W/A/S/D) or enter 'Q' to exit: ");
            scanf(" %c", &move);
        }

        CLEAR_CONSOLE;

        // sure sinirini kontrol etme
        time_t currentTime = time(NULL);
        if (currentTime - gameMap.startTime >= gameMap.timeLimit) {
            printf("Time's up!\n");
            break; // eger sure dolduysa bir sonraki harekette oyunu bitir
        }

    }while(moveUser(&gameMap, move));

    printResults(&gameMap);

    user->score = calculateScore(gameMap);
    writeScoreToFile(user);

    freeMap(&gameMap);

    return 0;
}

char autoplayMove(const GameMap *gameMap) {
    // rastgele bir yon secicek
    char directions[] = {'W', 'A', 'S', 'D'};

    char nextMove = directions[rand() % 4];

    //eger rastgele secilen yon yukari ise yukariyi kontrol et, duvar varsa yukari haric bir sey gelene kadar tekrar sectir
    if(nextMove == 'W'){
        if(gameMap->matrix[gameMap->userRow - 1][gameMap->userCol] != '1'){
            nextMove = 'W';
        }else{
            do {
                nextMove = directions[rand() % 4];
            }while(nextMove == 'W');

        }
    }
    //eger rastgele secilen yon sola ise solu kontrol et, duvar varsa sol haric bir sey gelene kadar tekrar sectir
    if(nextMove == 'A'){
        if(gameMap->matrix[gameMap->userRow][gameMap->userCol - 1] != '1'){
            nextMove = 'A';
        }else{
            do {
                nextMove = directions[rand() % 4];
            }while(nextMove == 'A');
        }
    }
    //eger rastgele secilen yon asagi ise asagiyi kontrol et, duvar varsa asagi haric bir sey gelene kadar tekrar sectir
    if(nextMove == 'S'){
        if(gameMap->matrix[gameMap->userRow + 1][gameMap->userCol] != '1'){
            nextMove = 'S';
        }else{
            do {
                nextMove = directions[rand() % 4];
            }while(nextMove == 'S');
        }
    }
    //eger rastgele secilen yon saga ise sagı kontrol et, duvar varsa sag haric bir sey gelene kadar tekrar sectir
    if(nextMove == 'D'){
        if(gameMap->matrix[gameMap->userRow][gameMap->userCol + 1] != '1'){
            nextMove = 'D';
        }else{
            do {
                nextMove = directions[rand() % 4];
            }while(nextMove == 'D');
        }
    }

    return nextMove;
}

void printResults(GameMap *gameMap) {
    CLEAR_CONSOLE;
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
    gameMap->antihydrogen = antihydrogen;

    printf("\nAntihydrogen created: %d\n", antihydrogen);
}

void printTopScores() {
    FILE *file;

    // dosyayi ac
    file = fopen("scores.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    USER user;
    USER topScores[MAX_SCORES];
    int i, j;

    for (i = 0; i < MAX_SCORES; i++) {
        strcpy(topScores[i].username, "");
        topScores[i].score = 0;
    }

    // tum satirlari oku ve skorlari güncelle
    while (fscanf(file, "%s %d", user.username, &user.score) != EOF) {
        for (i = 0; i < MAX_SCORES; i++) {
            if (user.score > topScores[i].score) {

                for (j = MAX_SCORES - 1; j > i; j--) {
                    strcpy(topScores[j].username, topScores[j - 1].username);
                    topScores[j].score = topScores[j - 1].score;
                }

                strcpy(topScores[i].username, user.username);
                topScores[i].score = user.score;
                break;
            }
        }
    }

    // en yuksek 5 skoru yazdir
    CLEAR_CONSOLE;
    printf("\nTop 5 Scores:\n");
    for (i = 0; i < MAX_SCORES; i++) {
        printf("%d. %s: %d\n", i + 1, topScores[i].username, topScores[i].score);
    }

    fclose(file);
}

int calculateScore(GameMap gameMap){

    int score = gameMap.rows * gameMap.cols * (gameMap.collectedp + gameMap.collectedE);
    return score;

}