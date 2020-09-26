#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int starts_with(char * prefix, char * string);
int tokenize(char * string);
int run_tests(FILE * f);

int main(int argc, char * argv[]){
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
	given a file pointer read the text that is in double quotes
*/
int run_tests(FILE * f){
	int c;
	while(c != EOF){
		//discard text before opening quotation
		while ((c = fgetc(f)) != '\"' && c != EOF){
		}
		char * input = malloc(1000);
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

/*
	returns 0 if tokenizes string succesfully. 
	returns -1 otherwise. 
*/
int tokenize(char * input){
	int length = strlen(input);
	typedef enum _type {NONE, SPACE, WORD, DECIMAL, OCTAL, HEX, FLOAT} type;

	type previous = NONE;
	type current = NONE;
	for (int i = 0; i < length ; i++){
  		char c = input[i];
		if(isspace(c)){
			current = SPACE;
		}
		else{
			if(starts_with("0x", &input[i]) || starts_with("0X", &input[i])){
				current = HEX;
				previous = NONE;
				i++; //skip the X or x
			}
			else if(previous!=WORD &&
					previous!=HEX &&
					previous!=DECIMAL &&
					starts_with("0", &input[i])){
				current = OCTAL;
				previous = NONE;
			}
			else if(isalpha(c)){
				if(previous==HEX && isxdigit(c)){
				}
				else{
					current = WORD;
				}
			}
			else if(isdigit(c)){
				if( (previous==HEX && isxdigit(c)) || previous==WORD || previous==OCTAL){
				}
				else{
					current = DECIMAL;
				}
			}
			if(previous!=current){
				if(current==WORD) printf("word\n");
				if(current==DECIMAL) printf("decimal\n");
				if(current==HEX) printf("hex\n");
				if(current==OCTAL) printf("octal\n");
			}
		}
		previous = current;
	}
	return 0;
}


int starts_with(char * prefix, char * string)
{
    return strncmp(prefix, string, strlen(prefix)) == 0;
}



