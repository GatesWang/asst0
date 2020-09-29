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
	FLOAT = 7,
	LEFTPARENTHESIS,
	RIGHTPARENTHESIS,
	LEFTBRACKET,
	RIGHTBRACKET,
	SUBTRACTION,
	DECREMENT,
	ADDITION,
	INCREMENT,
	COMMA,
	MULTIPLICATION,
	DIVISION,
	ASSIGNMENT,
	PLUSEQUALS,
	MINUSEQUALS,
	DIVIDEEQUALS,
	TIMESEQUALS,
	EQUALITY,
	BITWISEOR,
	BITWISEOREQUALS,
	LOGICALOR,
	COMPLEMENT,
	XOR,
	XOREQUALS,
	LESSTHAN,
	GREATERTHAN,
	LESSTHANOREQUAL,
	GREATERTHANOREQUAL,
	SHIFTLEFT,
	SHIFTRIGHT,
	SHIFTLEFTEQUALS,
	SHIFTRIGHTEQUALS,
	INEQUALITY,
	NEGATE,
	AND,
	LOGICALAND,
	ANDEQUALS,
	CONDITIONALTRUE,
	CONDITIONALFALSE,
	MOD,
	MODEQUALS,
	STRUCTUREPOINTER,
	SIZEOF

} type;

type current = NONE;
type previous = NONE;
void set_previous_and_current(char *input, int *i);
void set_token_type();

int is_octal(char *string, int i);
int is_hex(char *string, int i);
int is_digit(char *string, int i);
void process_operator(char c);
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

	for (int i = 0; i < strlen(input); i++){
		process_index(input, &i);
	}
	set_token_type(current);
	printf("%s %s\n", token_type, token);
	return 0;
}

//print if previous and current toke aren't the same
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

void print_token(){
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


// i is the current index from the input, input is array holding input string
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
	else{
		process_operator(c);
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
		case MULTIPLICATION:
			sprintf(token_type, "multiply/dereference operator");
			break;
		case DIVISION:
			sprintf(token_type, "division");
			break;
		case ADDITION:
				sprintf(token_type, "addition");
				break;
		case SUBTRACTION:
			sprintf(token_type, "minus/subtract operator");
			break;
		case ASSIGNMENT:
			sprintf(token_type, "assignment");
			break;
		case PLUSEQUALS:
			sprintf(token_type, "plus equals");
			break;
		case MINUSEQUALS:
			sprintf(token_type, "minus equals");
			break;
		case TIMESEQUALS:
			sprintf(token_type, "times equals");
			break;
		case DIVIDEEQUALS:
			sprintf(token_type, "divide equals");
			break;
		case EQUALITY:
			sprintf(token_type, "equality test");
			break;
		case INCREMENT:
			sprintf(token_type, "increment");
			break;
		case DECREMENT:
			sprintf(token_type, "decrement");
			break;
		case NEGATE:
			sprintf(token_type, "negate");
			break;
		case INEQUALITY:
			sprintf(token_type, "inequality test");
			break;
		case LEFTPARENTHESIS:
			sprintf(token_type, "left parenthesis");
			break;
		case RIGHTPARENTHESIS:
			sprintf(token_type, "right parenthesis");
			break;
		case LEFTBRACKET:
			sprintf(token_type, "left bracket");
			break;
		case RIGHTBRACKET:
			sprintf(token_type, "right bracket");
			break;
		case COMMA:
			sprintf(token_type, "comma");
			break;
		case COMPLEMENT:
			sprintf(token_type, "1s complement");
			break;
		case BITWISEOR:
			sprintf(token_type, "bitwise OR");
			break;
		case LOGICALOR:
			sprintf(token_type, "logical OR");
			break;
		case XOR:
			sprintf(token_type, "bitwise XOR");
			break;
		case XOREQUALS:
			sprintf(token_type, "bitwise XOR equals");
			break;
		case LESSTHAN:
			sprintf(token_type, "less than test");
			break;
		case GREATERTHAN:
			sprintf(token_type, "greater than test");
			break;
		case LESSTHANOREQUAL:
			sprintf(token_type, "less than or equal test");
			break;
		case GREATERTHANOREQUAL:
			sprintf(token_type, "greater than or equal test");
			break;
		case SHIFTLEFT:
			sprintf(token_type, "shift left");
			break;
		case SHIFTRIGHT:
			sprintf(token_type, "shift right");
			break;
		case SHIFTLEFTEQUALS:
			sprintf(token_type, "shift left equals");
			break;
		case CONDITIONALTRUE:
			sprintf(token_type, "conditional true");
			break;
		case CONDITIONALFALSE:
			sprintf(token_type, "conditional false");
			break;
		case AND:
			sprintf(token_type, "AND/address operator");
			break;
		case ANDEQUALS:
			sprintf(token_type, "bitwise AND equals");
			break;
		case BITWISEOREQUALS:
			sprintf(token_type, "bitwise OR equals");
			break;
		case LOGICALAND:
			sprintf(token_type, "logical AND");
			break;
		case MOD:
			sprintf(token_type, "modulo");
			break;
		case MODEQUALS:
			sprintf(token_type, "mod equals");
			break;
		case STRUCTUREPOINTER:
			sprintf(token_type, "structure pointer");
			break;
		case SIZEOF:
			sprintf(token_type, "sizeof");
			break;
	}
}

/*
	compares prefix string with location at index pointer
	returns true if string begins with prefix
*/
int starts_with(char * prefix, char * string){
    return strncmp(prefix, string, strlen(prefix)) == 0;
}

/*
	parameter : (input array, current index)
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
	parameter : (input array, current index)
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



// idenitifies if current token is an operator, then assigns it the proper one.
void process_operator(char c){
	switch(c){
	// only need to check one chracter
	case '(':
		current = LEFTPARENTHESIS;
		printf("%d\n",current);
		break;
	case ')':
		current = RIGHTPARENTHESIS;
		break;
	case '[':
		current = LEFTBRACKET;
		break;
	case ']':
		current = RIGHTBRACKET;
		break;
	case ',':
			current = COMMA;
			break;
	case '~':
			current = COMPLEMENT;
			break;
	case '^':
			current = XOR;
			break;
	case '*':
			current = MULTIPLICATION;
			break;
	case '/':
			current = DIVISION;
			break;
	case '!':
			current = NEGATE;
			break;
	case '?':
			current = CONDITIONALTRUE;
			break;
	case ':':
			current = CONDITIONALFALSE;
			break;
	case '%':
			current = MOD;
			break;
	// need to check the previous type to confirm the type
	case '+':
		if (previous == ADDITION){
			current = INCREMENT;
			previous = INCREMENT;
			break;
		}
			current = ADDITION;
			break;
	case '-':
		if (previous == SUBTRACTION){
			current = DECREMENT;
			previous = DECREMENT;
			break;
		}
		else if(previous != FLOAT){
			current = SUBTRACTION;
		}
		break;
	case '<':
		if (previous == LESSTHAN){
			current = SHIFTLEFT;
			previous = SHIFTLEFT;
			break;
		}
			current = LESSTHAN;
			break;
	case '>':
		if (previous == GREATERTHAN){
			current = SHIFTRIGHT;
			previous = SHIFTRIGHT;
			break;
		}
		else if(previous == SUBTRACTION){
			current = STRUCTUREPOINTER;
			previous = STRUCTUREPOINTER;
			break;
		}
		else{
			current = GREATERTHAN;
			break;
		}
	// nested switch statment
	case '=':
		switch(previous){
			case ASSIGNMENT:
				current = EQUALITY;
				previous = EQUALITY;
				break;
			case ADDITION:
				current = PLUSEQUALS;
				previous = PLUSEQUALS;
				break;
			case SUBTRACTION:
				current = MINUSEQUALS;
				previous = MINUSEQUALS;
				break;
			case MULTIPLICATION:
				current = TIMESEQUALS;
				previous = TIMESEQUALS;
				break;
			case DIVISION:
				current = DIVIDEEQUALS;
				previous = DIVIDEEQUALS;
				break;
			case NEGATE:
				current = INEQUALITY;
				previous = INEQUALITY;
				break;
			case LESSTHAN:
				current = LESSTHANOREQUAL;
				previous = LESSTHANOREQUAL;
				break;
			case GREATERTHAN:
				current = GREATERTHANOREQUAL;
				previous = GREATERTHANOREQUAL;
				break;
			case SHIFTLEFT:
				current = SHIFTLEFTEQUALS;
				previous = SHIFTLEFTEQUALS;
				break;
			case SHIFTRIGHT:
				current = SHIFTRIGHTEQUALS;
				previous = SHIFTRIGHTEQUALS;
				break;
			case AND:
				current = ANDEQUALS;
				previous= ANDEQUALS;
				break;
			case XOR:
				current = XOREQUALS;
				previous = XOREQUALS;
				break;
			case BITWISEOR:
				current = BITWISEOREQUALS;
				previous = BITWISEOREQUALS;
				break;
			case MOD:
				current = MODEQUALS;
				previous = MODEQUALS;
				break;
			default:
				current = ASSIGNMENT;
			}
			break;

	 case '|':
		if (previous == BITWISEOR){
			current = LOGICALOR;
			previous = LOGICALOR;
			break;
		}
		current = BITWISEOR;
		break;
	case '&':
		if (previous == AND){
			current = LOGICALAND;
			previous = LOGICALAND;
			break;
		}
		current = AND;
		break;
	//string compare for sizeof operator
	default:
		if (current == WORD){
			 if(strcmp(token,"sizeo") == 0 && c =='f'){
					 previous = SIZEOF;
	  				 current = SIZEOF;
			 }
		}
		break;
	}
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
