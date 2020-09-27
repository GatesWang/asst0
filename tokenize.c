#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int run_tests(FILE *f);
int tokenize(char *string);

typedef enum _type {
NONE,
STRUCT_MEM,
SPACE,
WORD,
DECIMAL,
OCTAL,
HEX,
FLOAT} type;

type current = NONE;
type previous = NONE;
void set_type(char *input, int *i);
char* get_type(type type);

int is_octal(char *string, int i);
int is_hex(char *string, int i);
int is_digit(char *string, int i);
int starts_with(char *prefix, char *string);

int convert_token_type(char * token_type);
int is_float = 0;


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

int tokenize(char *input){
	is_float = 0;
	current = NONE;
	previous = NONE;

	int length = strlen(input);
	char * token_type = malloc(1000 * sizeof(char));
	char * token = malloc(1000 * sizeof(char));

	for (int i = 0; i<length ; i++){
		if(isspace(input[i])){
			current = SPACE;
		}
		else{
			set_type(input, &i);
			if(previous!=current){
				if(strlen(token_type) > 0){//print
					convert_token_type(token_type);
					printf("%s %s\n", token_type, token);
				}

				//set token_type and token
				token_type = malloc(1000 * sizeof(char));
				token = malloc(1000 * sizeof(char));
				strcpy(token_type, get_type(current));
				if(current==HEX){
					strcat(token,"0");
				}
			}
			sprintf(token, "%s%c", token, input[i]);//append to token
		}
		previous = current;
	}
	convert_token_type(token_type);
	printf("%s %s\n", token_type, token);
	return 0;
}

/*
	input: the input string, an index, and the previous type
	output: the type of the current index
*/
void set_type(char *input, int *i){
	int j = *i;
	char c = input[j];

	//these are all about the previous token
	int word = previous==WORD;
	int decimal = previous==DECIMAL;
	int hex = previous==HEX;
	int octal = previous==OCTAL;

	int valid_hex_start = starts_with("0x", &input[j]) || starts_with("0X", &input[j]);
	int is_valid_hex = valid_hex_start && is_hex(input, j+2); 
	int is_valid_octal = starts_with("0", &input[j]) && is_octal(input, j+1); 

	if(is_valid_hex){
		current = HEX;
		previous = NONE;
		(*i)++; //skip the '0' and go to 'x' or 'X'
	}
	else if(!word && !hex && !decimal && !octal && is_valid_octal){
		current = OCTAL;
		previous = NONE;
	}
	else if(isalpha(c)){
		if(!hex){// if we are not continuing for hex
			current = WORD;
		}
		else if(!is_hex(input, j)){// we are continuing for hex, but it ends now
			current = WORD;
		}
	}
	else if(isdigit(c)){
		if(!hex && !word && !octal){// if we are not continuing for a different type
			current = DECIMAL;
		}
		else if(octal && !is_octal(input, j)){//we are continuing for octal, but it ends now
			current = DECIMAL;
		}
	}
	else if(c == '.'){
		if(current == DECIMAL && is_digit(input, j+1)){
			is_float = 1;
		}
		else{
			is_float = 0;
			current = STRUCT_MEM; 
		}
	}
}

/*
	given a type print the corresponding string
*/
char* get_type(type type){
	char * answer = malloc(1000 * sizeof(char));

	switch(type){
		case WORD :
			strcat(answer, "word");
			break;
		case DECIMAL :
			strcat(answer, "decimal");
			break;
		case OCTAL :
			strcat(answer, "octal");
			break;
		case HEX :
			strcat(answer, "hex");
			break;
		case STRUCT_MEM:
			strcat(answer, "struct mem");
			break;
	}

	return answer;
}

int convert_token_type(char * token_type){
	if(previous == DECIMAL && is_float){
		strcpy(token_type, "float");
		is_float = 0;
	}
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


int is_digit(char *string, int i){
	return i<strlen(string) && isdigit(string[i]);
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
