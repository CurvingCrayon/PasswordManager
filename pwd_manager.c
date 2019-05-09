#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DB_NAME "./database.txt"
#define MAX_NAME_LENGTH 20
#define MAX_USER_LENGTH 20
#define MAX_PASS_LENGTH 20
#define MAX_SERVICES 100

struct service{
	char name[MAX_NAME_LENGTH+1];
	char user[MAX_USER_LENGTH+1];
	char pass[MAX_PASS_LENGTH+1];
};
typedef struct service service_t;

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

int get_instruction (void){
	int valid = 0;
	while(!valid){ /*Keep asking for input until a valid one is given")*/
		printf("\n"
		"1. add a service\n"
		"2. display all services\n"
		"3. save the services to the database file\n"
		"4. load the services from the database file\n"
		"5. exit the program\n"
		"Enter choice (number between 1-5)>\n");
		char command[2];
		get_input(command, 2); 

		int command_num = atoi(command);
		if(command_num > 0 && command_num < 6){ /*Check command_num is between 1 and 5*/
			valid = 1;
			return command_num;
		}
		else{
			printf("Invalid choice\n");
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
			printf("Enter password>\n");
			
			char pass_input[MAX_PASS_LENGTH+1];
			get_input(pass_input, MAX_PASS_LENGTH+1);
			strncpy(new_service.pass, pass_input, strlen(pass_input));
			new_service.pass[strlen(pass_input)]='\0';
			valid = 1;
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
	for(service_num = 0; service_num < num_services; service_num++){
		print_service(services[service_num]); /*Print it */
		printf("\n");
	}
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



int main(){
	int num_services = 0;
	service_t services[MAX_SERVICES];
	int exit = 0;
	while(!exit){
		int command = get_instruction();
		switch(command){
			case 1:
				num_services = input_service(services, num_services);
			break;
			
			case 2:
				display_services(services, num_services);
			break;
			
			case 3:
				save_services(services, num_services);
			break;
			
			case 4:
				num_services = load_services(services, num_services);
			break;
			
			case 5:
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