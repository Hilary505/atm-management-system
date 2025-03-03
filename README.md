# atm-management-system

## Objective

This project implements an ATM management system using C. The application  provides basic ATM features such as user registration, account management and transaction handling.

## Requirements

 - C Compiler (gcc or clang)
 - Make tool (for compiling)


## Features

The application provides the following features:

- **Login/Register:** Allow users to login or register with unique credentials.
- **Create a new account:** Users can create a new account with their personal details.
- **Check the details of existing accounts:** Users can view details of specific accounts.
- **Update information of existing accounts:** Users can update their phone number or country.
- **Remove existing accounts:** Users can delete an account they own.
- **Check list of owned accounts:** Users can view all accounts they own.
- **Make transactions:** Users can withdraw or deposit funds into their accounts (except for fixed-term accounts).
- **Transfer ownership:** Users can transfer an account to another user.

## File System

The project has the following file structure:The project has the following file structure:

├── data 
  ├── records.txt │
  └── users.txt  
└── src 
 ├── auth.c 
 ├── header.h 
 ├── main.c 
 └── system.c
├── Makefile

### Makefile

The **Makefile** is used to compile the project and handle the build process.


### Source Code

- **`auth.c`**: Contains functions related to user authentication and registration.
- **`system.c`**: Handles the business logic for account operations (view, update, delete, etc.).
- **`header.h`**: Contains the function prototypes and necessary includes for the project.
- **`main.c`**: The entry point of the application, controlling the flow of the program.


## How to Run

1. Clone the repository to your local machine.

```bash
$ git clone  https://learn.zone01kisumu.ke/git/hilaromondi/atm-management-system.git
```
2. Navigate to the project directory.
```bash
$ cd atm-management-system
```

3. Compile the program using `make`:
```bash
$ make
```

4. Run the application
```bash
$./atm
```

## License
This project is openSource. Feel free to fork and contribute.