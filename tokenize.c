#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

	typedef enum _type {NONE, WORD, DECIMAL, OCTAL, HEX, FLOAT} type;
	
	type current = NONE;

	for (int i = 0; i < length ; i++){
  		printf("%c", input[i]);
  		//check type then go to appropriate classifier method
	}
	return 0;
}
