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
	NONE,
	STRUCT_MEM,
	SPACE,
	WORD,
	DECIMAL,
	OCTAL,
	HEX,
	FLOAT
} type;

type current = NONE;
type previous = NONE;
void set_type(char *input, int *i);
char* get_type(type type);

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
		if(isspace(input[i])){
			current = SPACE;
		}
		else{
			process_index(input, &i);
		}
		previous = current;
	}
	print_token(token_type, token);
	return 0;
}


void process_index(char * input, int *i){
	set_type(input, i);

	if(previous!=current){
		print_token(token_type, token);
	}
	sprintf(token, "%s%c", token, input[(*i)]);
}

/*w
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
	int floating = previous==FLOAT;

	int is_valid_hex = starts_with("0x", &input[j]) || starts_with("0X", &input[j]);
	int is_valid_octal = starts_with("0", &input[j]);

	if(is_valid_hex){
		current = HEX;
		previous = NONE;
		(*i)++; //skip the '0' and go to 'x' or 'X'
	}
	else if(!word && !hex && !decimal && !octal && !floating && is_valid_octal){
		current = OCTAL;
		previous = NONE;
	}
	else if(isalpha(c)){
		if(!hex)
			current = WORD;
		else if(!is_hex(input, j))
			current = WORD;
	}
	else if(isdigit(c)){
//		printf("h %d, w %d, o %d, f %d \n",hex,word,octal,floating);
		if(!hex && !word && !octal && !floating)
			current = DECIMAL;
		else if(octal && !is_octal(input, j))
			current = DECIMAL;
	}
	else if(c == '.'){
		if(previous == DECIMAL && is_digit(input, j+1)){
			previous = FLOAT;
			current = FLOAT;
		}
		else{
			current = STRUCT_MEM;
		}
	}
}

void print_token(){
	//print previous
	token_type = malloc(1000 * sizeof(char));
	strcpy(token_type, get_type(previous));
	if(strlen(token_type) > 0){
		printf("%s %s\n", token_type, token);
	}

	//reset token_type and token for current
	token_type = malloc(1000 * sizeof(char));
	token = malloc(1000 * sizeof(char));
	strcpy(token_type, get_type(current));
	if(current==HEX){
		strcat(token,"0");
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
		case FLOAT :
			strcat(answer, "float");
			break;
		case STRUCT_MEM:
			strcat(answer, "struct mem");
			break;
	}

	return answer;
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
			tokenize(input);
			printf("\n............................\n");
		}
	}
}

