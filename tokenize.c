#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int run_tests(FILE *f);

int tokenize(char *string);
typedef enum _type {NONE, SPACE, WORD, DECIMAL, OCTAL, HEX, FLOAT} type;
void set_type(char *input, type *previous, type *current, int *i);
void print_type(type type);

int is_octal(char *string, int i);
int is_hex(char *string, int i);
int starts_with(char *prefix, char *string);

int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Please enter string to tokenize.\n");
		return -1;
	}
	else if(argc > 2){
		printf("You entered too many arguments.\n");
		return -1;
	}

/*
	int length = strlen(argv[1]);
	char* input = malloc(length+1 * sizeof(char));
	strcpy(input,argv[1]);
	return tokenize(input);
*/

	FILE *fptr = fopen(argv[1], "r");
	run_tests(fptr);
	fclose(fptr);
	return 0;
}


/*
	returns 0 if tokenizes string succesfully.
	returns -1 otherwise.
*/
int tokenize(char *input){
	int length = strlen(input);

	type previous = NONE;
	type current = NONE;
	for (int i = 0; i<length ; i++){
		if(isspace(input[i])){
			current = SPACE;
		}
		else{
			set_type(input, &previous, &current, &i);
			if(previous!=current){
				print_type(current);
				if(current==HEX)
					printf("0");
			}
			printf("%c", input[i]);
		}
		previous = current;
	}
	return 0;
}

/*
	input: the input string, an index, and the previous type
	output: the type of the current index
*/
void set_type(char *input, type *previous, type *current, int *i){
	int j = *i;
	char c = input[j];

	//these are all about the previous token
	int not_word = *previous!=WORD;
	int not_decimal = *previous!=DECIMAL;
	int not_hex = *previous!=HEX;
	int not_octal = *previous!=OCTAL;

	/*
		"0XG" : in this case there is no valid hex digit
		----------
		decimal 0
		word XG

		"A0XRR" : in this case there is also no valid hex digit, but the 0 is part of a word
		----------
		word A0XRR

		"0720XTT" : in this case there is no valid hex digit, but the 0 is an octal
		----------
		octal 0720
		word XTT
	*/
	int valid_hex_start = starts_with("0x", &input[j]) || starts_with("0X", &input[j]);
	int is_valid_hex = valid_hex_start && is_hex(input, j+2);
	/*
		"07020" :
		----------
		octal 07020

		"079021" : in this case the octal stops because the digit is not a valid octal digit
		----------
		octal 07
		decimal 9021
	*/
	int is_valid_octal = starts_with("0", &input[j]) && is_octal(input, j+1); 

	if(is_valid_hex){
		*current = HEX;
		*previous = NONE;
		(*i)++; //skip the x or X
	}
	else if(not_word && not_hex && not_decimal && not_octal && is_valid_octal){
		*current = OCTAL;
		*previous = NONE;
	}
	else if(isalpha(c)){
		if(not_hex){// if we are not continuing for a different type
			*current = WORD;
		}
		if(!is_hex(input, j)){
			*current = WORD;
		}
	}
	else if(isdigit(c)){
		if(not_hex && not_word && not_octal){// if we are not continuing for a different type
			*current = DECIMAL;
		}
		if(!is_octal(input, j)){
			*current = DECIMAL;
		}
	}
}

/*
	given a type print the corresponding string
*/
void print_type(type type){
	printf("\n");
	switch(type){
		case WORD :
			printf("word");
			break;
		case DECIMAL :
			printf("decimal");
			break;
		case OCTAL :
			printf("octal");
			break;
		case HEX :
			printf("hex");
			break;
		case FLOAT :
			printf("float");
			break;
	}
	printf(" ");
}

/*
	returns true if string begins with prefix
*/
int starts_with(char * prefix, char * string){
    return strncmp(prefix, string, strlen(prefix)) == 0;
}

/*
	given an integer return if it is a valid octal digit
*/
int is_octal(char *string, int i){
	if(i<strlen(string)){
		int d = (string[i] - '0');
		return (d>=0 && d<=7);
	}
	return 0;
}

/*
	given an index return if it is a valid hex digit
*/
int is_hex(char *string, int i){
	return i<strlen(string) && isxdigit(string[i]);
}

/*
	given a file pointer read the text that is in double quotes
*/
int run_tests(FILE *f){
	int c;
	while(c != EOF){
		//discard text before opening quotation
		while ((c = fgetc(f)) != '\"' && c != EOF){
		}
		//read until closing quotation
		char *input = malloc(1000);
		size_t n = 0;
		while ((c = fgetc(f)) != '\"' && c != EOF){
			input[n++] = (char) c;
		}
		input[n] = '\0';
		if(n > 0){
			tokenize(input);
			printf("\n............................\n");
		}
	}
}
