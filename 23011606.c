#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// user
typedef struct{
    char name[30];
    char surname[30];
    char username[30];
    char password[30];
}USER;

// game
typedef struct{
    int score;
    int particlesCollected;
    int exitReached;

}GAME;

// Kullanıcı fonksiyonları
void createAccount(USER *user);
int login(USER *user);
void displayStartMenu();

/*void getUserInfo(USER *user) {
    printf("Isim: ");
    fgets(user->name, 30, stdin);
    user->name[strcspn(user->name, "\n")] = '\0';

    printf("Soyisim: ");
    fgets(user->surname, 30, stdin);
    user->surname[strcspn(user->surname, "\n")] = '\0';

    printf("Kullanici Adi: ");
    fgets(user->username, 30, stdin);
    user->username[strcspn(user->username, "\n")] = '\0';

    printf("Sifre: ");
    fgets(user->password, 30, stdin);
    user->password[strcspn(user->password, "\n")] = '\0';
}

void saveUserToFile(const USER *user) {
    FILE *file = fopen("user_data.bin", "ab");  // "ab" modu ile dosyayı aç, varsa devam et, yoksa oluştur
    if (file == NULL) {
        fprintf(stderr, "Dosya acilamadi.\n");
        return;
    }

    fwrite(user, sizeof(USER), 1, file);

    fclose(file);
}*/


void createAccount(USER *user) {
    printf("Name: ");
    scanf("%s", user-);
    fgets(user->name, 30, stdin);
    user->name[strcspn(user->name, "\n")] = '\0';

    printf("Soyisim: ");
    fgets(user->surname, 30, stdin);
    user->surname[strcspn(user->surname, "\n")] = '\0';

    printf("Kullanici Adi: ");
    fgets(user->username, 30, stdin);
    user->username[strcspn(user->username, "\n")] = '\0';

    printf("Sifre: ");
    fgets(user->password, 30, stdin);
    user->password[strcspn(user->password, "\n")] = '\0';

    FILE *file = fopen("user_data.bin", "ab");  // "ab" modu ile dosyayı aç, varsa devam et, yoksa oluştur
    if (file == NULL) {
        fprintf(stderr, "Dosya acilamadi.\n");
        return;
    }

    fwrite(user, sizeof(USER), 1, file);

    fclose(file);

}

int login(USER *user) {

    char inputUsername[30];
    char inputPassword[30];

    FILE *file = fopen("user_data.bin", "rb");
    if (file == NULL) {
        fprintf(stderr, "Dosya acilamadi.\n");
        return 0;  // Dosya açılamazsa oturum açma başarısız
    }

    int isLoggedIn = 0;  // Oturum açma durumu

    printf("Username: ");
    scanf("%s", inputUsername);
    printf("Password: ");
    scanf("%s", inputPassword);




    // Dosyadan kullanıcı bilgilerini okuma
    while (fread(&user, sizeof(USER), 1, file) == 1) {
        if (strcmp(user->username, inputUsername) == 0 && strcmp(user->password, inputPassword) == 0) {
            isLoggedIn = 1;  // Kullanıcı adı ve şifre eşleşirse oturum açma başarılı
            break;
        }

    }

    fclose(file);

    return isLoggedIn;
}




// Oyun fonksiyonları
void startGame(USER user);
void autoPlay();
void displayGameInfo(GAME game);


void displayStartMenu(USER *user) {
    int choice;
    printf("1. Create account\n");
    printf("2. Log in\n");
    printf("3. Exit\n");


    printf("Select your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            createAccount(user);
            break;
        case 2:
            login(user);
            break;
        case 3:

            break;
        default:
            printf("Invalid choice. Select again.\n");
            displayStartMenu(user);
    }
}



void startGame(USER user) {
    // Oyun başlatma fonksiyonu
}

void autoPlay() {
    // Otomatik oynatma fonksiyonu
}

void displayGameInfo(GAME game) {
    // Oyun bilgilerini ekrana yazdırma fonksiyonu
}

int main() {

    USER *user;

    displayStartMenu(user);



    return 0;
}
