#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOWER_OFFSET 97
#define UPPER_OFFSET 65
#define MAX_ENCRYPT_LENGTH 20

/* (int) 'a' = 97, (int) 'A' = 65*/
char cipher[] = "zyxwvutsrqponmlkjihgfedbca\0";
char cipher2[] = "ZYXWVUTSRQPONMLKJIHGFEDBCA\0";

char encode(char in){
	int index = (int) in;
	if(index > 96 && index < 124){ /*Lowercase input char*/
		return cipher[index - LOWER_OFFSET];
	}
	else if(index > 64 && index < 92){ /*Upper case input char*/
		return cipher2[index - UPPER_OFFSET];
	}
	else{
		return in;
	}
}

int encrypt(char input[], int len, char key[], char output[]){
	int char_num = 0;
	for(char_num = 0; char_num < len; char_num++){
		output[char_num] = encode(input[char_num]);
	}
	output[len] = '\0';
	/*strncpy(output, input, strlen(input));*/
	return 1;
}

