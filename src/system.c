#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	    r.userId,
	    u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u, const char *errorMessage)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        if (errorMessage != NULL)
        {
            printf("\n%s\n", errorMessage); 
        }
        else
        {
            printf("\n✖ This account does not exist!!\n"); 
        }
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

int getUserId(const char *username) {
  FILE *fp = fopen("./data/users.txt", "r");
  if (!fp) {
    perror("\n\t\tError opening file");
    exit(1);
  }
  char id[10], name[50], pass[50];
  while (fscanf(fp, "%s %s %s", id, name, pass) != EOF) {
    if (strcmp(name, username) == 0) {
      fclose(fp);
      return atoi(id);
    }
  }
  fclose(fp);
  return -1;
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf)
    {
        printf("\nUnable to open file!\n");
        exit(1);
    }
noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    int maxId = 0;
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (cr.id > maxId)
        {
            maxId = cr.id;
        }
    }
    r.id = maxId + 1;
    while (1)
    {
        printf("\nEnter today's date (mm/dd/yyyy): ");
        scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
        if (r.deposit.month < 1 || r.deposit.month > 12)
        {
            printf("\nInvalid month! Please enter a month between 1 and 12.\n");
            continue;
        }
        if (r.deposit.day < 1 || r.deposit.day > 31)
        {
            printf("\nInvalid day! Please enter a day between 1 and 31.\n");
            continue;
        }
        if (r.deposit.month == 2)
        {
            int isLeapYear = (r.deposit.year % 4 == 0 && r.deposit.year % 100 != 0) || (r.deposit.year % 400 == 0);
            if (r.deposit.day > 29 || (r.deposit.day == 29 && !isLeapYear))
            {
                printf("\nInvalid day for February! Please enter a valid day.\n");
                continue;
            }
        }
        if (r.deposit.year < 1900 || r.deposit.year > 2025)
        {
            printf("\nInvalid year! Please enter a year between 1900 and 2025.\n");
            continue;
        }

        break; 
    }
    while (1)
    {
        printf("\nEnter the account number (up to 15 digits): ");
        scanf("%d", &r.accountNbr);
        if (r.accountNbr < 0 || r.accountNbr > 999999999999999)
        {
            printf("\nInvalid account number! Please enter up to 15 digits.\n");
            continue;
        }
        rewind(pf); 
        int accountExists = 0;
        while (getAccountFromFile(pf, userName, &cr))
        {
            if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
            {
                accountExists = 1;
                break;
            }
        }
        if (accountExists)
        {
            printf("\n✖ This account number already exists for this user!\n");
            goto noAccount;
        }
        break; 
    }
    printf("\nEnter the country: ");
    scanf("%s", r.country);
    while (1)
    {
        printf("\nEnter the phone number (up to 10 digits): ");
        scanf("%d", &r.phone);

        if (r.phone < 0 || r.phone > 9999999999) 
        {
            printf("\nInvalid phone number! Please enter up to 10 digits.\n");
            continue;
        }
        break; 
    }
    while (1)
    {
        printf("\nEnter amount to deposit: $");
        scanf("%lf", &r.amount);

        if (r.amount < 0)
        {
            printf("\nInvalid amount! Please enter a positive value.\n");
            continue;
        }
        break; 
    }
    int choice;
    while (1)
    {
        printf("\nChoose the type of account:\n");
        printf("1 saving\n");
        printf("2 current\n");
        printf("3 fixed01\n");
        printf("4 fixed02\n");
        printf("5 fixed03\n\n");
        scanf("%d", &choice);
        switch (choice) {
           case 1:
           strcpy(r.accountType, "saving");
           break;
           case 2:
           strcpy(r.accountType, "current");
           break;
           case 3:
           strcpy(r.accountType, "fixed01");
           break;
           case 4:
           strcpy(r.accountType, "fixed02");
           break;
           case 5:
           strcpy(r.accountType, "fixed03");
           break;
           default:
            printf("\nInvalid account type! Please choose from the options provided.\n");
            continue;
        }
        break;
    }
    u.id = getUserId(u.name);
    r.userId = u.id;
    saveAccountToFile(pf, u, r);
    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");
    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

//updateAccount function 
void updateAccount(struct User u) {
    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        printf("Unable to open file!\n");
        exit(1);
    }
    struct {
        char name[100];
        struct Record record;
    } entries[100];
    int entryCount = 0;
    int accountId;
    int found = 0;
    system("clear");
    printf("\n\t\t===== Update Account =====\n");
    printf("\nEnter the account number to update: ");
    scanf("%d", &accountId);
    char currentUser[100];
    struct Record r;
    while (getAccountFromFile(pf, currentUser, &r)) {
        strcpy(entries[entryCount].name, currentUser);
        entries[entryCount].record = r;
        entryCount++;
    }
    fclose(pf);
    for (int i = 0; i < entryCount; i++) {
        if (entries[i].record.accountNbr == accountId && 
            strcmp(entries[i].name, u.name) == 0) {
            found = 1;
            printf("\nAccount found!\n");
            
            int option;
            printf("\n1. Update Country\n\n2. Update Phone Number\n\nSelect: ");
            scanf("%d", &option);

            switch (option) {
                case 1:
                    printf("\nEnter new country: ");
                    scanf("%s", entries[i].record.country);
                    break;
                case 2:
                    printf("\nEnter new phone number: ");
                    scanf("%d", &entries[i].record.phone);
                    break;
                default:
                    printf("\nInvalid option!\n");
                    return;
            }
            break;
        }
    }
    if (!found) {
        stayOrReturn(0, mainMenu, u, "\nAccount number not found for this user!\n");
        return;
    }
    pf = fopen(RECORDS, "w");
    if (!pf) {
        printf("\nUnable to open file!\n");
        exit(1);
    }
    for (int i = 0; i < entryCount; i++) {
        struct User tmpUser;
        char userIdStr[20]; 
        sprintf(userIdStr, "%d", entries[i].record.userId);
        tmpUser.id = getUserId(userIdStr);
        strcpy(tmpUser.name, entries[i].name);
        saveAccountToFile(pf, tmpUser, entries[i].record);
    }
    fclose(pf);
    success(u);
}

// checkAccount function 
void checkAccount(struct User u)
{
    FILE *pf = fopen(RECORDS, "r");
    if (!pf)
    {
        printf("\nUnable to open file!\n");
        exit(1);
    }
    char userName[100];
    struct Record r;
    int accountId;
    int found = 0;
    system("clear");
    printf("\t\t===== Check Account Details =====\n");
    printf("\nEnter the accountId you want to check: ");
    scanf("%d", &accountId);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            printf("\nAccount found!\n");
            printf("Account number: %d\n", r.accountNbr);
            printf("Deposit Date: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone number: %d\n", r.phone);
            printf("Amount deposited: $%.2f\n", r.amount);
            printf("Type of Account: %s\n", r.accountType);
            double monthlyInterest = 0.0;
            double annualInterestRate = 0.0;
            double totalInterest = 0.0;
            if (strcmp(r.accountType, "saving") == 0)
            {
                monthlyInterest = (r.amount * 7.0 / 100)/12; 
                printf("\nYou will get $%.2f as interest on day 10 of every month.\n", monthlyInterest);
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                annualInterestRate = 4.0; 
                totalInterest = r.amount * annualInterestRate / 100 * 1;
                printf("\nYou will get a total of $%.2f as interest on 10/10/2013.\n", totalInterest);
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                annualInterestRate = 5.0;
                totalInterest = r.amount * annualInterestRate / 100 * 2;
                printf("\nYou will get a total of $%.2f as interest on 10/10/2014.\n", totalInterest);
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                annualInterestRate = 8.0; 
                totalInterest = r.amount * annualInterestRate / 100 * 3;
                printf("\nYou will get a total of $%.2f as interest on 10/10/2015.\n", totalInterest);
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                printf("\nYou will not get interests because the account is of type current.\n");
                fclose(pf);
                success(u);
                return;
            }
            break;
        }
    }
    if (!found)
    {
        fclose(pf);
        stayOrReturn(0, mainMenu, u,"\nAccountId not found for this user!\n"); 
        return;
    }
    fclose(pf);
    success(u);
}

// makeTransaction function 
void makeTransaction(struct User u)
{
    FILE *pf = fopen(RECORDS, "r+"); 
    if (!pf)
    {
        printf("\nUnable to open file!\n");
        exit(1);
    }
    char userName[100];
    struct Record r;
    int accountId;
    double transactionAmount;
    int found = 0;
    int option;
    system("clear");
    printf("\t\t===== Make Transaction =====\n");
    printf("\nEnter the accountId to perform the transaction: ");
    scanf("%d", &accountId);
    struct {
        char name[100];
        struct Record record;
    } entries[100]; 
    int entryCount = 0;
    while (getAccountFromFile(pf, userName, &r))
    {
        strcpy(entries[entryCount].name, userName); 
        entries[entryCount].record = r; 
        entryCount++;
    }
    fclose(pf);
    for (int i = 0; i < entryCount; i++)
    {
        if (strcmp(entries[i].name, u.name) == 0 && entries[i].record.accountNbr == accountId)
        {
            found = 1;
            if (strcmp(entries[i].record.accountType, "fixed01") == 0 || strcmp(entries[i].record.accountType, "fixed02") == 0 || strcmp(entries[i].record.accountType, "fixed03") == 0)
            {
                stayOrReturn(0, mainMenu, u,"\n✖ Transactions are not allowed for this account type!\n"); 
                return;
            }
            printf("\nChoose the transaction type:\n");
            printf("\n1 - Deposit\n");
            printf("\n2 - Withdraw\n");
            printf("\n\nEnter your choice: ");
            scanf("%d", &option);
            if (option == 1)
            {
                printf("\nEnter amount to deposit: $");
                scanf("%lf", &transactionAmount);
                entries[i].record.amount += transactionAmount;
                printf("\n✔ Successfully deposited $%.2f\n", transactionAmount);
            }
            else if (option == 2)
            {
                printf("\nEnter amount to withdraw: $");
                scanf("%lf", &transactionAmount);

                if (entries[i].record.amount - transactionAmount < 0.0)
                {
                    stayOrReturn(0, mainMenu, u,"✖ oops, Insufficient funds! You cannot withdraw more than the available balance.\n");
                    return;
                }
                entries[i].record.amount -= transactionAmount;
                printf("\n✔ Successfully withdrew $%.2f\n", transactionAmount);
            }
            else
            {
                stayOrReturn(0, mainMenu, u,"\n✖ Invalid transaction type!\n");
                return;
            }
            break;
        }
    }
    if (!found)
    {
        stayOrReturn(0, mainMenu, u,"\nAccountId not found for this user!\n"); 
        return;
    }
    pf = fopen(RECORDS, "w");
    if (!pf)
    {
        printf("Unable to open file!\n");
        exit(1);
    }
    for (int i = 0; i < entryCount; i++)
    {
        struct User tmpUser;
        tmpUser.id = entries[i].record.userId; 
        strcpy(tmpUser.name, entries[i].name);
        saveAccountToFile(pf, tmpUser, entries[i].record);
    }
    fclose(pf);
    success(u);
}

// removeAccount function
void removeAccount(struct User u)
{
    FILE *pf = fopen(RECORDS, "r"); 
    FILE *tempFile = fopen("./data/temp_records.txt", "w"); 
    if (!pf || !tempFile)
    {
        printf("\nUnable to open file!\n");
        exit(1);
    }
    char userName[100];
    struct Record r;
    int accountId;
    int found = 0;
    system("clear");
    printf("\t\t===== Remove Account =====\n");
    printf("\nEnter the account number to remove: ");
    scanf("%d", &accountId);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            printf("\n✔ Account found and will be removed!\n");
            continue;  
        }
        struct User tmpUser;
        tmpUser.id = r.userId; 
        strcpy(tmpUser.name, userName); 
        saveAccountToFile(tempFile, tmpUser, r);
    }
    fclose(pf);
    fclose(tempFile);
    if (!found)
    {
        remove("./data/temp_records.txt"); 
        stayOrReturn(0, mainMenu, u,"\n✖ Account ID not found for this user!\n");
        return;
    }
    if (remove(RECORDS) == 0)
    {
        if (rename("./data/temp_records.txt", RECORDS) == 0)
        {
            printf("\n✔ Account removed successfully!\n");
        }
        else
        {
            printf("\n✖ Error renaming temporary file.\n");
            exit(1);
        }
    }
    else
    {
        printf("\n✖ Error deleting old file.\n");
        exit(1);
    }
    success(u);
}

// transferAccount function
void transferAccount(struct User u)
{
    FILE *pf = fopen(RECORDS, "r+"); 
    if (!pf)
    {
        printf("\nUnable to open file!\n");
        exit(1);
    }
    char userName[100];
    struct Record r;
    int accountId;
    char newOwner[50];
    int found = 0;
    system("clear");
    printf("\t\t===== Transfer Ownership =====\n");
    printf("\nEnter the accountId to transfer ownership: ");
    scanf("%d", &accountId);

    printf("\nEnter the new owner's name: ");
    scanf("%s", newOwner);
    struct {
        char name[100];
        struct Record record;
    } entries[100]; 
    int entryCount = 0;
    while (getAccountFromFile(pf, userName, &r))
    {
        strcpy(entries[entryCount].name, userName);
        entries[entryCount].record = r; 
        entryCount++;
    }
    fclose(pf);
    for (int i = 0; i < entryCount; i++)
    {
        if (strcmp(entries[i].name, u.name) == 0 && entries[i].record.accountNbr == accountId)
        {
            found = 1;
            printf("\nAccount found!\n");
            printf("\nTransferring ownership to %s\n", newOwner);

            strcpy(entries[i].name, newOwner);
            entries[i].record.userId = getUserId(newOwner);
            break;
        }
    }
    if (!found)
    {
        stayOrReturn(0, mainMenu, u,"\nAccount ID not found for this user!\n"); 
        return;
    }
    pf = fopen(RECORDS, "w");
    if (!pf)
    {
        printf("\nUnable to open file!\n");
        exit(1);
    }
    for (int i = 0; i < entryCount; i++)
    {
        struct User tmpUser;
        tmpUser.id = entries[i].record.userId; 
        strcpy(tmpUser.name, entries[i].name); 
        saveAccountToFile(pf, tmpUser, entries[i].record);
    }
    fclose(pf);
    success(u);
}