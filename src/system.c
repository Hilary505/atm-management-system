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
	    u.id,
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
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
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

    // Store both usernames and records
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

    // Read all records with their usernames
    char currentUser[100];
    struct Record r;
    while (getAccountFromFile(pf, currentUser, &r)) {
        strcpy(entries[entryCount].name, currentUser);
        entries[entryCount].record = r;
        entryCount++;
    }
    fclose(pf);

    // Find and update the correct record
    for (int i = 0; i < entryCount; i++) {
        if (entries[i].record.accountNbr == accountId && 
            strcmp(entries[i].name, u.name) == 0) {
            found = 1;
            printf("\nAccount found!\n");
            
            int option;
            printf("1. Update Country\n2. Update Phone Number\nSelect: ");
            scanf("%d", &option);

            switch (option) {
                case 1:
                    printf("Enter new country: ");
                    scanf("%s", entries[i].record.country); // Corrected typo: country -> country
                    break;
                case 2:
                    printf("Enter new phone number: ");
                    scanf("%d", &entries[i].record.phone);
                    break;
                default:
                    printf("Invalid option!\n");
                    return;
            }
            break;
        }
    }

    if (!found) {
        printf("\nAccount number not found for this user!\n");
        stayOrReturn(0, mainMenu, u);
        return;
    }

    // Rewrite all records with original usernames and user IDs
    pf = fopen(RECORDS, "w");
    if (!pf) {
        printf("Unable to open file!\n");
        exit(1);
    }

    for (int i = 0; i < entryCount; i++) {
        // Create a temporary User struct with stored username and user ID from the record
        struct User tmpUser;
        tmpUser.id = entries[i].record.userId; // Retrieve user ID from the record
        strcpy(tmpUser.name, entries[i].name); // Use the stored username

        // Save the record with the correct user info
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
        printf("Unable to open file!\n");
        exit(1);
    }

    char userName[100];
    struct Record r;
    int accountId;
    int found = 0;

    system("clear");
    printf("\t\t===== Check Account Details =====\n");

    // Ask for the account ID
    printf("\nEnter the account ID you want to check: ");
    scanf("%d", &accountId);

    // Read the records and search for the account
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            // Display account details
            printf("\nAccount found!\n");
            printf("Account number: %d\n", r.accountNbr);
            printf("Deposit Date: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone number: %d\n", r.phone);
            printf("Amount deposited: $%.2f\n", r.amount);
            printf("Type of Account: %s\n", r.accountType);

            // Calculate and display the interest based on the account type
            double monthlyInterest = 0.0;
            double annualInterestRate = 0.0;

            // Interest rates for different account types
            if (strcmp(r.accountType, "saving") == 0)
            {
                annualInterestRate = 7.0; // 7% annually for savings
            }

            if (strcmp(r.accountType, "fixed01") == 0)
            {
                annualInterestRate = 4.0; // 4% annually for 1-year fixed
            }

            if (strcmp(r.accountType, "fixed02") == 0)
            {
                annualInterestRate = 5.0; // 5% annually for 2-year fixed
            }

            if (strcmp(r.accountType, "fixed03") == 0)
            {
                annualInterestRate = 8.0; // 8% annually for 3-year fixed
            }

            if (strcmp(r.accountType, "current") == 0)
            {
                printf("You will not get interests because the account is of type current.\n");
                fclose(pf);
                success(u);
                return;
            }

            // If interest is applicable, calculate monthly interest
            if (annualInterestRate > 0)
            {
                monthlyInterest = (r.amount * annualInterestRate / 100) / 12; // Monthly interest
                printf("You will get $%.2f as interest on day %d of every month.\n", monthlyInterest, r.deposit.day);
            }
            break;
        }
    }

    if (!found)
    {
        printf("\nAccount ID not found for this user!\n");
        fclose(pf);
        stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
        return;
    }

    fclose(pf);

    // Option to go back to main menu or exit
    success(u);
}

// makeTransaction function 
void makeTransaction(struct User u)
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
    double transactionAmount;
    int found = 0;
    int option;

    system("clear");
    printf("\t\t===== Make Transaction =====\n");

    printf("\nEnter the account ID to perform the transaction: ");
    scanf("%d", &accountId);

    // Read all records into memory
    struct {
        char name[100];
        struct Record record;
    } entries[100];  // Store both usernames and records
    int entryCount = 0;

    while (getAccountFromFile(pf, userName, &r))
    {
        strcpy(entries[entryCount].name, userName); 
        entries[entryCount].record = r; 
        entryCount++;
    }
    fclose(pf);

    // Find and update the correct record
    for (int i = 0; i < entryCount; i++)
    {
        if (strcmp(entries[i].name, u.name) == 0 && entries[i].record.accountNbr == accountId)
        {
            found = 1;
            if (strcmp(entries[i].record.accountType, "fixed01") == 0 || strcmp(entries[i].record.accountType, "fixed02") == 0 || strcmp(entries[i].record.accountType, "fixed03") == 0)
            {
                printf("\n✖ Transactions are not allowed for this account type!\n");
                stayOrReturn(0, mainMenu, u); 
                return;
            }

            // Ask the user for the transaction type (deposit or withdraw)
            printf("\nChoose the transaction type:\n");
            printf("1 - Deposit\n");
            printf("2 - Withdraw\n");
            printf("\n\nEnter your choice: ");
            scanf("%d", &option);

            if (option == 1)
            {
                // Deposit
                printf("\nEnter amount to deposit: $");
                scanf("%lf", &transactionAmount);
                entries[i].record.amount += transactionAmount;
                printf("\n✔ Successfully deposited $%.2f\n", transactionAmount);
            }
            else if (option == 2)
            {
                // Withdraw
                printf("\nEnter amount to withdraw: $");
                scanf("%lf", &transactionAmount);

                if (entries[i].record.amount - transactionAmount < 0.0)
                {
                    printf("\n✖ Insufficient funds! You cannot withdraw more than the available balance.\n");
                    stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
                    return;
                }
                entries[i].record.amount -= transactionAmount;
                printf("\n✔ Successfully withdrew $%.2f\n", transactionAmount);
            }
            else
            {
                printf("\n✖ Invalid transaction type!\n");
                stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
                return;
            }
            break;
        }
    }
    if (!found)
    {
        printf("\nAccount ID not found for this user!\n");
        stayOrReturn(0, mainMenu, u); 
        return;
    }

    // Now, rewrite the entire file with updated records
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

        // Save the record with the correct user info
        saveAccountToFile(pf, tmpUser, entries[i].record);
    }
    fclose(pf);

    success(u);
}

// removeAccount function
void removeAccount(struct User u)
{
    FILE *pf = fopen(RECORDS, "r"); 
    FILE *tempFile = fopen("./data/temp_records.txt", "w"); // Use a temporary file
    if (!pf || !tempFile)
    {
        printf("Unable to open file!\n");
        exit(1);
    }

    char userName[100];
    struct Record r;
    int accountId;
    int found = 0;

    system("clear");
    printf("\t\t===== Remove Account =====\n");

    // Ask the user for the account number to remove
    printf("\nEnter the account number to remove: ");
    scanf("%d", &accountId);

    while (getAccountFromFile(pf, userName, &r))
    {
        // If account found for the user, skip it
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            printf("\n✔ Account found and will be removed!\n");
            continue;  
        }

        struct User tmpUser;
        tmpUser.id = r.userId; 
        strcpy(tmpUser.name, userName); 

        // Write all other records to the temporary file
        saveAccountToFile(tempFile, tmpUser, r);
    }

    fclose(pf);
    fclose(tempFile);

    if (!found)
    {
        printf("\n✖ Account ID not found for this user!\n");
        remove("./data/temp_records.txt"); 
        stayOrReturn(0, mainMenu, u);
        return;
    }

    // Delete the old records file and rename the temporary file
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

