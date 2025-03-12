#include <termios.h>
#include "header.h"
#include <ctype.h>

char *USERS = "./data/users.txt";
int id[10];

void initMenu(struct User *u);

void loginMenu(char a[50], char pass[16])
{
    struct termios oflags, nflags;
    system("clear");
    printf("\n\t  Bank Management System\n");
    printf("\n\t Enter your username: ");
    scanf("%s", a);
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\tEnter your password: ");
    scanf("%s", pass);
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};
const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (fscanf(fp, " %d %s %s",&u.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }
    fclose(fp);
    return "no user found";
}

void registerMenu(struct User *u) {
    FILE *fp;
    struct User userChecker;
    int userExists = 0;
    char name[50], password[16];
    int newId = 0;
    system("clear");
    printf("\n\t Bank Management System\n");
    while (getchar() != '\n');
    while (1) {
        printf("\n\tEnter username: ");
        scanf(" %[^\n]", name);
        
        char *start = name;
        while (isspace((unsigned char)*start)) start++;
        char *end = name + strlen(name) - 1;
        while (end > start && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';
        if (strlen(start) == 0) {
            printf("\n\tError: Username cannot be empty. Please try again.\n");
            continue;
        }
         if (strlen(start) > 50) {
            printf("\n\tUsername should not exceed 50 characters. Please try again.\n");
            continue;
        }
        int valid = 1;
        for (int i = 0; start[i] != '\0'; i++) {
            if (!isalnum(start[i]) && start[i] != '_') {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("\n\tError: Username can only contain letters, numbers, and underscores. Please try again.\n");
            continue;
        }
        strcpy(name, start);
        break;
    }
    if ((fp = fopen("./data/users.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, name) == 0) {
            userExists = 1;
            break;
        }
        if (userChecker.id >= newId) {
            newId = userChecker.id + 1;
        }
    }
    fclose(fp);
    if (userExists) {
        printf("\n\tError: User already exists. Please try another username.\n");
        printf("\n\tPress Enter to continue");
        while (getchar() != '\n');
        initMenu(u);
    }
    struct termios oflags, nflags;
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }
    while (getchar() != '\n');
    while (1) {
        printf("\n\tEnter Password: ");
        int i = 0;
        char ch;
        while (i <= 15 && (ch = getchar()) != '\n' && ch != EOF) {
            password[i++] = ch;
        }
        password[i] = '\0';
        if (ch != '\n' && ch != EOF) {
        while (getchar() != '\n');
    } 
        char *start = password;
        while (isspace((unsigned char)*start)) start++;
        char *end = password + strlen(password) - 1;
        while (end > start && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';
        if (strlen(start) == 0) {
            printf("\n\tError: Password cannot be empty. Please try again.\n");
            continue;
        }
        if (strlen(start) < 10 || strlen(start) > 15) {
            printf("\n\tEnter a strong password 10-15 characters . Please try again.\n");
            continue;
        }
        strcpy(password, start); 
        break;
    }
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }
    fp = fopen("./data/users.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "%d %s %s \n", newId, name, password);
    fclose(fp);
    printf("\n\tRegistration Successful ✓✓✓\n");
    printf("\n\tPress Enter to login");
    while (getchar() != '\n');
    u->id = newId;
    strcpy(u->name, name);
    strcpy(u->password, password);
    initMenu(u);
}