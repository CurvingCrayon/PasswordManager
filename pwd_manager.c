#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encrypt.h"

#define DB_NAME "./database.txt"
#define MAX_NAME_LENGTH 20
#define MAX_USER_LENGTH 20
#define MAX_PASS_LENGTH 20
#define MAX_SERVICES 100
#define MAX_ENCRYPT_LENGTH  20

#define DEFAULT "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"

struct service{
	char name[MAX_NAME_LENGTH+1];
	char user[MAX_USER_LENGTH+1];
	char pass[MAX_PASS_LENGTH+1];
};  
typedef struct service service_t;

void conceal_input(){
	system("stty -echo");
}

void deconceal_input(){
	system("stty echo");
}

int check_empty(char str[], int len){
	int char_num;
	int empty = 1;
	for(char_num = 0; char_num < len; char_num++){
		if(str[char_num] != ' '){
			empty = 0;
		}
	}
	return empty;
}
int fix_multi_line(char str[], int len, int max_len){ /* Addresses problems of fgets having multi-line inputs*/
	int too_large = 1;
	int char_num;
	for(char_num = 0; char_num < len; char_num++){
		if(str[char_num] == '\n' && too_large){
			str[char_num] = '\0';
			if(char_num < max_len){
				too_large = 0;
			}
		}
	}
	
	return too_large;
}
int get_input(char str[], int max_len){ /*Implements fgets to get user input */

	char input[max_len+1]; /* Uses +1 to allow for a newline character */
	fgets(input, max_len+1, stdin);
	int too_large = fix_multi_line(input, strlen(input), max_len+1);
	if(too_large){
		/* Clear input buffer */
		int buffer_char;
		int end_of_buffer = 0;
		while (!end_of_buffer){ 
			buffer_char = getchar();
			if(buffer_char == '\n' || buffer_char == EOF){
				end_of_buffer = 1;
			}
		}
	}
	if(strlen(input) == 0 || check_empty(input,strlen(input))){
		get_input(input, max_len); /*Call this function again*/
	}
	input[max_len-1] = '\0';
	strncpy(str,input,max_len);
	return too_large;
}
void get_enter(){
	char empty[3];
	fgets(empty, 3, stdin);
	int too_large = fix_multi_line(empty, 3, 3);
	if(too_large){
		/* Clear input buffer */
		int buffer_char;
		int end_of_buffer = 0;
		while (!end_of_buffer){ 
			buffer_char = getchar();
			if(buffer_char == '\n' || buffer_char == EOF){
				end_of_buffer = 1;
			}
		}
	}
}
int get_instruction (void){
	int valid = 0;
	
	while(!valid){ /*Keep asking for input until a valid one is given")*/
		printf(DEFAULT
		"1. add a service\n"
		"2. display all services\n"
		"3. get service password\n"
		"4. save the services to the database file\n"
		"5. load the services from the database file\n"
		"6. exit the program\n"
		"Enter choice (number between 1-6)>\n");
		char command[2];
		get_input(command, 2); 

		int command_num = atoi(command);
		if(command_num > 0 && command_num < 7){ /*Check command_num is between 1 and 5*/
			valid = 1;
			return command_num;
		}
		else{
			printf(RED"Invalid choice\n");
		}
	}
	return 0;
}


int input_service(service_t services[], int num_services){
	if(num_services < MAX_SERVICES){
		service_t new_service;
		/*Get service name */
		int valid = 0;
		while(!valid){
			printf("Enter service name>\n");
			
			char pass_input[MAX_NAME_LENGTH+1];
			get_input(pass_input, MAX_NAME_LENGTH+1);
			strncpy(new_service.name, pass_input, strlen(pass_input));
			new_service.name[strlen(pass_input)]='\0';
			valid = 1;
		}
		
		/*Get service username */
		valid = 0;
		while(!valid){
			printf("Enter username>\n");
			
			char user_input[MAX_USER_LENGTH+1];
			get_input(user_input, MAX_USER_LENGTH+1);
			strncpy(new_service.user, user_input, strlen(user_input));
			new_service.user[strlen(user_input)]='\0';
			valid = 1;
		}
		
		/*Get service password */
		valid = 0;
		while(!valid){
			printf(DEFAULT "Enter password>\n");
			
			char pass_input[MAX_PASS_LENGTH+1];
			conceal_input();
			get_input(pass_input, MAX_PASS_LENGTH+1);
			deconceal_input();
			
				
			char pass[MAX_ENCRYPT_LENGTH+1];
			char key[10];
			encrypt(pass_input, strlen(pass_input), key, pass);
			pass[strlen(pass_input)] = '\0';

			printf("Confirm password>\n");
			
			char pass_input2[MAX_PASS_LENGTH+1];
			conceal_input();
			get_input(pass_input2, MAX_PASS_LENGTH+1);
			deconceal_input();


			char pass2[MAX_ENCRYPT_LENGTH+1];
			char key2[10];
			encrypt(pass_input2, strlen(pass_input2), key2, pass2);
			pass[strlen(pass_input2)] = '\0';
			

			/*TODO: compare pass and pass2 and set valid*/
			if(!strcmp(pass,pass2)){
				strncpy(new_service.pass, pass2, strlen(pass2));
				new_service.pass[strlen(pass2)]='\0';
				
				valid = 1;
				
				printf(GREEN "SUCCESS. Password encrypted as: %s\n", pass);
				printf(DEFAULT "Press <ENTER> to continue\n");
				get_enter();
				system("clear");
			}
			else{
				printf(RED "Passwords do not match.\n");
			}
		}
		
		/*Append local new_service to services array */
		
		services[num_services] = new_service;
		return num_services + 1;
	}
	else{
		printf("Cannot add more services - memory full\n");	
		return num_services;
	}
	return num_services;
}

void print_service(service_t service){
	printf("%s   %s    %s",service.name, service.user, service.pass);
}

void display_services(service_t services[], int num_services){
	int service_num;
	int service_exists = 0;
	for(service_num = 0; service_num < num_services; service_num++){
		service_exists = 1;
		print_service(services[service_num]); /*Print it */
		printf("\n");
	}
	if(!service_exists){
		printf(RED "No services exist.\n");		
	}
	printf(DEFAULT "Press <ENTER> to continue\n");
	get_enter();
}

int get_password(service_t services[], int num_services){
	printf("Enter name of desired service>");
	char name[MAX_NAME_LENGTH+1];
	get_input(name, MAX_NAME_LENGTH+1);
	
	int service_num;
	int found = 0;
	for(service_num = 0; service_num  < num_services; service_num++){
		if(!strcmp(services[service_num].name, name)){
			found = 1;
			char decrypted[MAX_PASS_LENGTH+1];
			char key[20];
			decrypt(services[service_num].pass, strlen(services[service_num].pass), key, decrypted);
			printf(GREEN "Successfully decrypted.\n");
			printf(DEFAULT "%s\n", decrypted);
			printf("Press <ENTER> to continue\n");
			get_enter();
		}
	}
	if(!found){
		printf(RED "Service not found.\n");
		printf(DEFAULT "Press <ENTER> to continue\n");
		get_enter();
	}
	return 1;
}

void save_services(service_t services[], int num_services){
	FILE* db_file = fopen(DB_NAME, "w"); /*Open file as read text */
	if(db_file == NULL){ /*If file doesnt exist*/
		printf("Write error\n");
		return;
	}
	int service_num;
	for(service_num = 0; service_num < num_services; service_num++){
		service_t sv = services[service_num];
		fprintf(db_file, "%s %s %s\n", sv.name, sv.user, sv.pass);
	}
	fclose(db_file);
}

int load_services(service_t services[], int num_services){
	FILE* db_file = fopen(DB_NAME, "r"); /*Open file as read text */
	
	if(db_file == NULL){ /*If file doesn't exist */
		printf("Read error\n");
		return num_services;
	}
	
	/*Get number of lines in file*/
	int num_lines = 0;
	while( !feof(db_file) ){
		char next_char = fgetc(db_file);
		if(next_char == '\n'){
			num_lines++;
		}
	}
	
	rewind(db_file); /*Point back to start of file */
	
	/* Go through each line of the file */
	int new_num_services = 0;
	int line_num;
	for(line_num = 0; line_num < MAX_SERVICES; line_num++){
		if(line_num < num_lines){ /*If there is still data left in file*/
			service_t sv;
			fscanf(db_file, "%s %s %s", sv.name, sv.user, sv.pass);
			services[line_num] = sv;
			new_num_services++;
		}
		else{
			break;
		}
	}
	fclose(db_file);
	return new_num_services;
}


int login(){
	printf("Enter username:>\n");
	return 1;
}
int main(){
	system("clear");
	int num_services = 0;
	service_t services[MAX_SERVICES];
	int exit = 0;
	printf("================================\n");
	printf("Welcome to password manager 1.0\n");
	printf("================================\n");
	int first_run = 1;
	while(!exit){
		if(!first_run){
			system("clear");
		}
		first_run = 0;
		int command = get_instruction();
		
		switch(command){
			case 1:
				num_services = input_service(services, num_services);
			break;
			
			case 2:
				display_services(services, num_services);
			break;
			
			case 3:
				get_password(services, num_services);
			break;
			
			case 4:
				save_services(services, num_services);
			break;
			
			case 5:
				num_services = load_services(services, num_services);
			break;
			
			case 6:
				exit = 1;
			break;
			
			default:
				printf("Error: invalid command entered, exiting program.");
				return 1;
			break;
		}
	}
	return 0;
}
