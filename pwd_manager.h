//pwd_manager.h file
#ifndef _pwd_manager_h
#define _pwd_manager_h

//prompts user to login or create new user
int id();

//creates a new username and password
int createUser();

//encrypts the given sequence
int encrypt();

//decrypts the given sequence
int decrypt();

//prompts username and password for previously created user
int login();

//allows user to find managed passwords or create new passwords to manage
int menu();

//prompts for new service and password, and encrypts it
int newPassword();

//prompts for service name, decrypts password and prints it
int showPassword();

#endif
