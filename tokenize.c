#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int run_tests(FILE *f);
int tokenize(char *string);
void process_index(char * input, int *i);
void print_token();
char * token_type;
char * token;

typedef enum _type {
	NONE = 0,
	SPACE = 1,
	STRUCT_MEM = 2,
	WORD = 3,
	DECIMAL = 4,
	OCTAL = 5,
	HEX = 6,
	FLOAT = 7
} type;

type current = NONE;
type previous = NONE;
void set_previous_and_current(char *input, int *i);
void set_token_type();

int is_octal(char *string, int i);
int is_hex(char *string, int i);
int is_digit(char *string, int i);
int starts_with(char *prefix, char *string);

int main(int argc, char *argv[]){
	FILE *fptr = fopen(argv[1], "r");
	run_tests(fptr);
	fclose(fptr);
	return 0;
}

int tokenize(char *input){
	current = NONE;
	previous = NONE;
	token_type = malloc(1000 * sizeof(char));
	token = malloc(1000 * sizeof(char));

	for (int i = 0; i<strlen(input); i++){
		process_index(input, &i);
	}
	set_token_type(current);
	printf("%s %s\n", token_type, token);
	return 0;
}

void process_index(char * input, int *i){
	set_previous_and_current(input, i);
	if(previous!=current){
		print_token(token_type, token);
	}

	//print to token one character at a time
	char c = input[*i];
	if(!isspace(c)){
		sprintf(token, "%s%c", token, c);
	}
}

void set_previous_and_current(char *input, int *i){
	int j = *i;
	char c = input[j];

	//these are all about the previous token
	previous = current;
	int word = previous==WORD;
	int decimal = previous==DECIMAL;
	int hex = previous==HEX;
	int octal = previous==OCTAL;
	int floating = previous==FLOAT;

	if(isspace(c)){
		current = SPACE;
	}
	else if(isalpha(c)){
		int valid_exponent1 = starts_with("e", &input[j]) && is_digit(input, j+1);
		int valid_exponent2 = starts_with("e-", &input[j]) && is_digit(input, j+2);
		int start_of_exponent = floating && (valid_exponent1 || valid_exponent2);

		if(!hex && !start_of_exponent){
			current = WORD;
		}
		else if(hex && !is_hex(input, j)){
			current = WORD;
		}
	}
	else if(isdigit(c)){
		int is_valid_hex = starts_with("0x", &input[j]) || starts_with("0X", &input[j]);
		int is_valid_octal = starts_with("0", &input[j]);

		if(!word && !decimal && !octal && !floating && is_valid_hex){
			current = HEX;
			previous = hex ? NONE : previous;
			(*i)++; //skip the '0' and go to 'x' or 'X'
		}
		else if(!word && !hex && !decimal && !octal && !floating && is_valid_octal){
			current = OCTAL;
			previous = octal ? NONE : previous;
		}
		else if(!hex && !word && !octal && !floating){
			current = DECIMAL;
		}
		else if(octal && !is_octal(input, j)){
			current = DECIMAL;
		}
	}
	else if(c == '.'){
		if( (decimal || octal) && is_digit(input, j+1)){
			previous = FLOAT;
			current = FLOAT;
		}
		else{
			current = STRUCT_MEM;
		}
	}
}

void print_token(){
//	printf("prev %d current %d\n",previous,current);
	//print previous
	set_token_type(previous);
	if(strlen(token) > 0){
		printf("%s %s\n", token_type, token);
	}

	//set token_type for current
	set_token_type(current);
	token[0] = '\0';
	if(current==HEX){
		strcat(token,"0");
	}
}

void set_token_type(type type){
	switch(type){
		case WORD :
			sprintf(token_type, "word");
			break;
		case DECIMAL :
			sprintf(token_type, "decimal");
			break;
		case OCTAL :
			sprintf(token_type, "octal");
			break;
		case HEX :
			sprintf(token_type, "hex");
			break;
		case FLOAT :
			sprintf(token_type, "float");
			break;
		case STRUCT_MEM:
			sprintf(token_type, "struct member");
			break;
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

/*
	given an index return if the char at that position is a 
	valid decimal digit
*/
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
			printf("............................\n");
			printf("%s\n", input);
			printf("............................\n");
			tokenize(input);
		}
	}
}

