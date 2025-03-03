#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";
int id[10];

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\n\t\t\t\t\t User Login: ");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\t\t\tEnter the password to login: ");
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
    char name[50], password[50]; // Declare variables
    int newId = 0;

    system("clear");
    printf("\n\n\n\t\t\t\t Bank Management System\n\n\n");

    printf("\n\n\t\t\t\tEnter name: ");
    scanf("%s", name);

    // Check if username already exists
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
                newId = userChecker.id + 1; // Find the next available ID
        }
    }
    fclose(fp);

    if (userExists) {
        printf("\n\t\t\t\t user already exists!\n");
        printf("\n\t\t\t\tPress any key to continue...");
        getchar(); // Consume newline
        getchar();
        return;
    }

    // Handle password input with hidden characters
    struct termios oflags, nflags;

    // Disable echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;


    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    printf("\n\n\t\t\t\tEnter Password: ");
    scanf("%s", password);

    // Restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    // Save new user to file
    fp = fopen("./data/users.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "%d %s %s \n",newId, name, password);
    fclose(fp);

    printf("\n\n\t\t\t\tRegistration Successful!\n");
    printf("\n\n\t\t\t\tPress any key to continue");
    getchar();
    getchar();

    u->id = newId;
    strcpy(u->name,name);
    strcpy(u->password,password);

}