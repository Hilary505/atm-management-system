#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char username[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		          &r->userId,username,
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
	        u.id,
	        u.username,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
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

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.username) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

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
    printf("\t\t====== All accounts from user, %s =====\n\n", u.username);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.username) == 0)
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

void updateAccount(struct User u)
{
    FILE *pf = fopen(RECORDS, "r+"); // Open the file for reading and writing
    if (!pf)
    {
        printf("Unable to open file!\n");
        exit(1);
    }

    char userName[100];
    struct Record r;
    int accountId;
    int option;
    int found = 0;

    system("clear");
    printf("\n\t\t===== Update Account =====\n");

    // Ask for the account ID to update
    printf("\nEnter the account ID to update: ");
    scanf("%d", &accountId);

    // Read all records into memory
    struct Record records[100];  // assuming you have a maximum of 100 records
    int recordCount = 0;

    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.username) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            // Update the record if found
            printf("\nAccount found!\n");
            printf("1. Update Country\n");
            printf("2. Update Phone Number\n");
            printf("Select an option (1 or 2): ");
            scanf("%d", &option);

            switch (option)
            {
            case 1:
                printf("\nEnter new country: ");
                scanf("%s", r.country);
                break;

            case 2:
                printf("\nEnter new phone number: ");
                scanf("%d", &r.phone);
                break;

            default:
                printf("Invalid option!\n");
                fclose(pf);
                return;
            }
        }

        // Store the record (updated or original)
        records[recordCount++] = r;
    }

    if (!found)
    {
        printf("\nAccount ID not found for this user!\n");
        fclose(pf);
        stayOrReturn(0, mainMenu, u);
        return;
    }

    fclose(pf);

    // Now, rewrite the entire file with updated records
    pf = fopen(RECORDS, "w"); // Open the file again for writing
    if (!pf)
    {
        printf("Unable to open file!\n");
        exit(1);
    }

    for (int i = 0; i < recordCount; i++)
    {
        // Write all records (updated and original) back to the file
        saveAccountToFile(pf, u, records[i]);
    }

    fclose(pf);

    // Success
    success(u);
}
