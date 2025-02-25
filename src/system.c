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

// updateAccount function
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
        if (strcmp(userName, u.username) == 0 && r.accountNbr == accountId)
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

    // Ask for the account ID to perform the transaction
    printf("\nEnter the account ID to perform the transaction: ");
    scanf("%d", &accountId);

    // Read all records into memory
    struct Record records[100];  // assuming you have a maximum of 100 records
    int recordCount = 0;

    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.username) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            // Check if the account type allows transactions
            if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0)
            {
                printf("\n✖ Transactions are not allowed for this account type!\n");
                fclose(pf);
                stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
                return;
            }

            // Ask the user for the transaction type (deposit or withdraw)
            printf("\nChoose the transaction type:\n");
            printf("[1] - Deposit\n");
            printf("[2] - Withdraw\n");
            printf("Enter your choice: ");
            scanf("%d", &option);

            if (option == 1)
            {
                // Deposit
                printf("\nEnter amount to deposit: $");
                scanf("%lf", &transactionAmount);
                r.amount += transactionAmount;
                printf("\n✔ Successfully deposited $%.2f\n", transactionAmount);
            }
            else if (option == 2)
            {
                // Withdraw
                printf("\nEnter amount to withdraw: $");
                scanf("%lf", &transactionAmount);

                if (r.amount - transactionAmount < 0.0)
                {
                    printf("\n✖ Insufficient funds! You cannot withdraw more than the available balance.\n");
                    fclose(pf);
                    stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
                    return;
                }
                r.amount -= transactionAmount;
                printf("\n✔ Successfully withdrew $%.2f\n", transactionAmount);
            }
            else
            {
                printf("\n✖ Invalid transaction type!\n");
                fclose(pf);
                stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
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
        stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
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


// removeAccount function
void removeAccount(struct User u)
{
    FILE *pf = fopen(RECORDS, "r"); // Open file for reading
    FILE *tempFile = fopen("./data/records.txt", "w"); // Create a temporary file to store the remaining records
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
        if (strcmp(userName, u.username) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            printf("\n✔ Account found and will be removed!\n");
            continue;  // Skip writing this account to the new file
        }

        // Write all other records to the temporary file
        saveAccountToFile(tempFile, u, r);
    }

    if (!found)
    {
        printf("\n✖ Account ID not found for this user!\n");
        fclose(pf);
        fclose(tempFile);
        stayOrReturn(0, mainMenu, u); // Let the user choose to return or exit
        return;
    }

    fclose(pf);
    fclose(tempFile);

    // Delete the old records file and rename the temporary file
    if (remove(RECORDS) == 0)
    {
        if (rename("./data/records.txt", RECORDS) == 0)
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

// transfer account function

void transferAccount(struct User u)
{
    FILE *pf = fopen(RECORDS, "r+"); // Open file for reading and writing
    if (!pf)
    {
        printf("Unable to open file!\n");
        exit(1);
    }

    char userName[100];
    struct Record r;
    int accountId;
    char newOwner[50];
    int found = 0;

    system("clear");
    printf("\t\t===== Transfer Ownership =====\n");

    // Ask for the account ID to transfer ownership
    printf("\nEnter the account ID to transfer ownership: ");
    scanf("%d", &accountId);

    // Ask for the new owner's username
    printf("\nEnter the new owner's username: ");
    scanf("%s", newOwner);

    // Read all records into memory
    struct Record records[100];  // assuming a maximum of 100 records
    int recordCount = 0;

    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.username) == 0 && r.accountNbr == accountId)
        {
            found = 1;
            // Update the record with the new owner's username
            printf("\nAccount found!\n");
            printf("Transferring ownership to %s...\n", newOwner);

            // Change the ownership (username) of the account
            strcpy(userName, newOwner); // Transfer the ownership to the new user

            // Store the updated record
            records[recordCount++] = r;
        }
        else
        {
            // Store the record as is (for non-matching accounts)
            records[recordCount++] = r;
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

    // Success message
    success(u);
}
