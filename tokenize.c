#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum _type {NONE, SPACE, WORD, DECIMAL, OCTAL, HEX, FLOAT} type;

int run_tests(FILE *f);
int tokenize(char *string);
int starts_with(char *prefix, char *string);
void set_type(char *input, type *previous, type *current, int *i);
void print_type(type type);

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

	int not_word = *previous!=WORD;
	int not_decimal = *previous!=DECIMAL;
	int not_hex = *previous!=HEX;
	int not_octal = *previous!=OCTAL;

	if(starts_with("0x", &input[j]) || starts_with("0X", &input[j])){
		*current = HEX;
		*previous = NONE;
		(*i)++; //skip the X or x
	}
	else if(not_word && not_hex && not_decimal && starts_with("0", &input[j])){
		*current = OCTAL;
		*previous = NONE;
	}
	else if(isalpha(c)){
		if(not_hex || !isxdigit(c)){
			*current = WORD;
		}
	}
	else if(isdigit(c)){
		if(not_hex && not_word && not_octal){
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
