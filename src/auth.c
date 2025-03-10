#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";
int id[10];

void initMenu(struct User *u);

void loginMenu(char a[50], char pass[50])
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
    // restore terminal
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
    char name[50], password[50];
    int newId = 0;

    system("clear");
    printf("\n\t Bank Management System\n");
    printf("\n\tEnter username: ");
    scanf("%s", name);
    if ((fp = fopen("./data/users.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    while (fscanf(fp, "%d %s %s",&userChecker.id, userChecker.name, userChecker.password) != EOF) {
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
        printf("\n\t user already exists try another username!\n");
        printf("\n\tPress Enter to continue");
        while (getchar() != '\n');
        getchar();
        initMenu(u);
        return;
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
    printf("\n\tEnter Password: ");
    scanf("%s", password);
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }
    fp = fopen("./data/users.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "%d %s %s \n",newId, name, password);
    fclose(fp);
    printf("\n\tRegistration Successful!\n");
    printf("\n\tPress Enter to login");
     while (getchar() != '\n');
    getchar();
    u->id = newId;
    strcpy(u->name,name);
    strcpy(u->password,password);

    initMenu(u);
}