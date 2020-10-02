#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//list of token types
typedef enum _type {
	NONE, SPACE, STRUCT_MEM, WORD, DECIMAL, OCTAL, HEX, FLOATING, LEFTPARENTHESIS,
	RIGHTPARENTHESIS, LEFTBRACKET, RIGHTBRACKET, SUBTRACTION, DECREMENT,ADDITION,
	INCREMENT,COMMA, MULTIPLICATION, DIVISION, ASSIGNMENT, PLUSEQUALS,MINUSEQUALS,
	DIVIDEEQUALS,	TIMESEQUALS, EQUALITY, BITWISEOR, BITWISEOREQUALS,
	LOGICALOR, COMPLEMENT, XOR, XOREQUALS, LESSTHAN, GREATERTHAN, LESSTHANOREQUAL,
	GREATERTHANOREQUAL, SHIFTLEFT,SHIFTRIGHT, SHIFTLEFTEQUALS,SHIFTRIGHTEQUALS,
	INEQUALITY, NEGATE, AND, LOGICALAND, ANDEQUALS, CONDITIONALTRUE, FLOAT,
	CONDITIONALFALSE, MOD, MODEQUALS, STRUCTUREPOINTER, SIZEOF, INT, SHORT, DOUBLE,
  AUTO, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DO, ELSE, ENUM,	EXTERN,
	STRUCT, LONG, REGISTER, SWITCH, TYPEDEF, RETURN, UNION, FOR, SIGNED, VOID, IF,
  STATIC, WHILE, GOTO, VOLATILE, UNSIGNED, SINGLEQUOTE, DOUBLEQUOTE, QUOTEEND,
	FRONTSLASH, SINGLELINECOMMENT
} tokenType;

//function declerations
int run_tests(FILE *f);
int tokenize(char *string);
void process_index(char * input, int *i);
void print_token();
<<<<<<< HEAD
=======
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

>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
int is_octal(char *string, int i);
int is_hex(char *string, int i);
int is_digit(char *string, int i);
void process_operator(char c);
int starts_with(char *prefix, char *string);
void identifyOperator(char c);
void set_previous_and_current(char *input, int *i);
void setTokenTypeString(tokenType type);
void isKeyword(char* keyword, tokenType typeOfKeyword, char currentChar);
void identitfyKeyword(char c);


char tokenTypeString[30];
char * token;


tokenType current = NONE;
tokenType previous = NONE;







/*processes the input string then tokenizes it and prints out the result
parameter : (input string) */
int tokenize(char *input){
	current = NONE;
	previous = NONE;
	token = malloc(1000 * sizeof(char));
<<<<<<< HEAD
=======

>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
	for (int i = 0; i < strlen(input); i++){
		process_index(input, &i);
	}
	setTokenTypeString(current);
	// prints last token
	printf("%s: \"%s\"\n", tokenTypeString, token);
	return 0;
}

/*processes the current char of the input string and assigns it a token type
parameter : (input string, current index) */
void process_index(char * input, int *i){
	set_previous_and_current(input, i);
	if(previous!=current){
		print_token(tokenTypeString, token);
	}
	//copies current char of the input string to the end of the token string if it not a white space character
	char c = input[*i];
	if(current != SPACE){
		sprintf(token, "%s%c", token, c);
	}
}

<<<<<<< HEAD
//prints the token and token type
void print_token(){
	//prints previous
	setTokenTypeString(previous);
	if(strlen(token) > 0){
		printf("%s: \"%s\"\n", tokenTypeString, token);
	}
	//set token type for current
	setTokenTypeString(current);
=======
void print_token(){
	//print previous
	set_token_type(previous);
	if(strlen(token) > 0){
		printf("%s %s\n", token_type, token);
	}

	//set token_type for current
	set_token_type(current);
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
	token[0] = '\0';
	if(current==HEX){
		strcat(token,"0");
	}
}

<<<<<<< HEAD
// decodes the token type into its proper string representation
void setTokenTypeString(tokenType type){
	switch(type){
		case WORD :
			sprintf(tokenTypeString, "word");
			break;
		case DECIMAL :
			sprintf(tokenTypeString, "decimal integer");
			break;
		case OCTAL :
			sprintf(tokenTypeString, "octal integer");
			break;
		case HEX :
			sprintf(tokenTypeString, "hexadecimal integer");
			break;
		case FLOATING :
			sprintf(tokenTypeString, "floating point");
			break;
		case STRUCT_MEM:
			sprintf(tokenTypeString, "struct member");
			break;
		case MULTIPLICATION:
			sprintf(tokenTypeString, "multiply/dereference operator");
			break;
		case DIVISION:
			sprintf(tokenTypeString, "division");
			break;
		case ADDITION:
				sprintf(tokenTypeString, "addition");
				break;
		case SUBTRACTION:
			sprintf(tokenTypeString, "minus/subtract operator");
			break;
		case ASSIGNMENT:
			sprintf(tokenTypeString, "assignment");
			break;
		case PLUSEQUALS:
			sprintf(tokenTypeString, "plus equals");
			break;
		case MINUSEQUALS:
			sprintf(tokenTypeString, "minus equals");
			break;
		case TIMESEQUALS:
			sprintf(tokenTypeString, "times equals");
			break;
		case DIVIDEEQUALS:
			sprintf(tokenTypeString, "divide equals");
			break;
		case EQUALITY:
			sprintf(tokenTypeString, "equality test");
			break;
		case INCREMENT:
			sprintf(tokenTypeString, "increment");
			break;
		case DECREMENT:
			sprintf(tokenTypeString, "decrement");
			break;
		case NEGATE:
			sprintf(tokenTypeString, "negate");
			break;
		case INEQUALITY:
			sprintf(tokenTypeString, "inequality test");
			break;
		case LEFTPARENTHESIS:
			sprintf(tokenTypeString, "left parenthesis");
			break;
		case RIGHTPARENTHESIS:
			sprintf(tokenTypeString, "right parenthesis");
			break;
		case LEFTBRACKET:
			sprintf(tokenTypeString, "left bracket");
			break;
		case RIGHTBRACKET:
			sprintf(tokenTypeString, "right bracket");
			break;
		case COMMA:
			sprintf(tokenTypeString, "comma");
			break;
		case COMPLEMENT:
			sprintf(tokenTypeString, "1s complement");
			break;
		case BITWISEOR:
			sprintf(tokenTypeString, "bitwise OR");
			break;
		case LOGICALOR:
			sprintf(tokenTypeString, "logical OR");
			break;
		case XOR:
			sprintf(tokenTypeString, "bitwise XOR");
			break;
		case XOREQUALS:
			sprintf(tokenTypeString, "bitwise XOR equals");
			break;
		case LESSTHAN:
			sprintf(tokenTypeString, "less than test");
			break;
		case GREATERTHAN:
			sprintf(tokenTypeString, "greater than test");
			break;
		case LESSTHANOREQUAL:
			sprintf(tokenTypeString, "less than or equal test");
			break;
		case GREATERTHANOREQUAL:
			sprintf(tokenTypeString, "greater than or equal test");
			break;
		case SHIFTLEFT:
			sprintf(tokenTypeString, "shift left");
			break;
		case SHIFTRIGHT:
			sprintf(tokenTypeString, "shift right");
			break;
		case SHIFTRIGHTEQUALS:
			sprintf(tokenTypeString, "shift right equals");
			break;
		case SHIFTLEFTEQUALS:
			sprintf(tokenTypeString, "shift left equals");
			break;
		case CONDITIONALTRUE:
			sprintf(tokenTypeString, "conditional true");
			break;
		case CONDITIONALFALSE:
			sprintf(tokenTypeString, "conditional false");
			break;
		case AND:
			sprintf(tokenTypeString, "AND/address operator");
			break;
		case ANDEQUALS:
			sprintf(tokenTypeString, "bitwise AND equals");
			break;
		case BITWISEOREQUALS:
			sprintf(tokenTypeString, "bitwise OR equals");
			break;
		case LOGICALAND:
			sprintf(tokenTypeString, "logical AND");
			break;
		case MOD:
			sprintf(tokenTypeString, "modulo");
			break;
		case MODEQUALS:
			sprintf(tokenTypeString, "mod equals");
			break;
		case STRUCTUREPOINTER:
			sprintf(tokenTypeString, "structure pointer");
			break;
		case SIZEOF:
			sprintf(tokenTypeString, "sizeof");
			break;
		case INT:
			sprintf(tokenTypeString, "int");
			break;
		case SHORT:
			sprintf(tokenTypeString, "short");
			break;
		case DOUBLE:
			sprintf(tokenTypeString, "double");
			break;
	 case AUTO:
				sprintf(tokenTypeString, "auto");
				break;
		case BREAK:
			sprintf(tokenTypeString, "break");
			break;
		case CASE:
			sprintf(tokenTypeString, "case");
			break;
		case CHAR:
			sprintf(tokenTypeString, "char");
			break;
		case CONST:
			sprintf(tokenTypeString, "const");
			break;
		case CONTINUE:
			sprintf(tokenTypeString, "continue");
			break;
		case DEFAULT:
			sprintf(tokenTypeString, "default");
			break;
		case DO:
			sprintf(tokenTypeString, "do");
			break;
		case ELSE:
			sprintf(tokenTypeString, "else");
			break;
		case ENUM:
			sprintf(tokenTypeString, "enum");
			break;
		case EXTERN:
			sprintf(tokenTypeString, "extern");
			break;
		case LONG:
				sprintf(tokenTypeString, "long");
				break;
		case FLOAT:
				sprintf(tokenTypeString, "float");
				break;
		case STRUCT:
				sprintf(tokenTypeString, "struct");
				break;
		case REGISTER:
				sprintf(tokenTypeString, "register");
				break;
		case SWITCH:
				sprintf(tokenTypeString, "switch");
			break;
		case TYPEDEF:
			sprintf(tokenTypeString, "typedef");
			break;
		case RETURN:
			sprintf(tokenTypeString, "return");
			break;
		case UNION:
				sprintf(tokenTypeString, "union");
				break;
		case FOR:
				sprintf(tokenTypeString, "for");
				break;
		case SIGNED:
				sprintf(tokenTypeString, "signed");
				break;
		case VOID:
			sprintf(tokenTypeString, "void");
			break;
		case IF:
			sprintf(tokenTypeString, "if");
			break;
		case STATIC:
			sprintf(tokenTypeString, "static");
			break;
		case WHILE:
			sprintf(tokenTypeString, "while");
			break;
		case GOTO:
			sprintf(tokenTypeString, "goto");
			break;
		case VOLATILE:
			sprintf(tokenTypeString, "volatile");
			break;
		case UNSIGNED:
		  sprintf(tokenTypeString, "unsigned");
		  break;
		case QUOTEEND:
			  sprintf(tokenTypeString, "string");
			  break;
		case SINGLELINECOMMENT:
					sprintf(tokenTypeString, " ");
					break;

	}
}

/* sets or modifies the type for the previous and current token
	parameter : (input string, current index) */
=======

// i is the current index from the input, input is array holding input string
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
void set_previous_and_current(char *input, int *i){
	int j = *i;
	char c = input[j];

	previous = current;
	int previousWasWord = previous==WORD;
	int previousWasDecimal = previous==DECIMAL;
	int previousWasHex = previous==HEX;
	int previousWasOctal = previous==OCTAL;
	int previousWasFloating = previous==FLOAT;

	if(isspace(c)){
		current = SPACE;
	}

	else if(isalpha(c)){
		int valid_exponent1 = starts_with("e", &input[j]) && is_digit(input, j+1);
		int valid_exponent2 = starts_with("e-", &input[j]) && is_digit(input, j+2);
		int valid_exponent3 = starts_with("e+", &input[j]) && is_digit(input, j+2);
		int start_of_exponent = previousWasFloating && (valid_exponent1 || valid_exponent2 || valid_exponent3);

		if(!previousWasHex && !start_of_exponent){
			current = WORD;
		}
		else if(previousWasHex && !is_hex(input, j)){
			current = WORD;
		}
	}
	else if(isdigit(c)){
		int is_valid_hex = starts_with("0x", &input[j]) || starts_with("0X", &input[j]);
		int is_valid_octal = starts_with("0", &input[j]);

		if(!previousWasWord && !previousWasDecimal && !previousWasOctal && !previousWasFloating && is_valid_hex){
			current = HEX;
			previous = previousWasHex ? NONE : previous;
			(*i)++; //skip the '0' and go to 'x' or 'X'
		}
		else if(!previousWasWord && !previousWasHex && !previousWasDecimal && !previousWasOctal && !previousWasFloating && is_valid_octal){
			current = OCTAL;
			previous = previousWasOctal ? NONE : previous;
		}
		else if(!previousWasHex && !previousWasWord && !previousWasOctal && !previousWasFloating){
			current = DECIMAL;
		}
		else if(previousWasOctal && !is_octal(input, j)){
			current = DECIMAL;
		}
	}
	else if(c == '.'){
		if( (previousWasDecimal || previousWasOctal) && is_digit(input, j+1)){
			previous = FLOAT;
			current = FLOAT;
		}
		else{
			current = STRUCT_MEM;
			print_token();
		}
	}
<<<<<<< HEAD
	identifyOperator(c);
	identitfyKeyword(c);
=======
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
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
}

/*
	compares a prefix string with the input string beginning at the current index
	returns integer > 0 if string begins with the prefix
	parameter : (prefix string, input string)
*/
int starts_with(char * prefix, char * string){
    return strncmp(prefix, string, strlen(prefix)) == 0;
}

/*
	returns an integer > 0 if the current char is a valid octal digit
	parameter : (input string, current index)
*/
int is_octal(char *string, int i){
	if(i<strlen(string)){
		int d = (string[i] - '0');

		return (d>=0 && d<=7);
	}
	return 0;
}

/*
 returns a non zero value if the current char is a valid hex digit
	parameter : (input string, current index)
*/
int is_hex(char *string, int i){
	return i<strlen(string) && isxdigit(string[i]);
}

/*
	returns an integer > 0 if the current char is a
	valid decimal digit.
	parameter : (input string, current index)
*/
int is_digit(char *string, int i){
	return i<strlen(string) && isdigit(string[i]);
}

<<<<<<< HEAD
/*
idenitifies if the current token is an operator, and assigns it the proper token.
parameter : (current char from input string)
*/
void identifyOperator(char c){
	switch(c){
	// only needs to check one chracter
	case '(':
		current = LEFTPARENTHESIS;
		print_token();
=======


// idenitifies if current token is an operator, then assigns it the proper one.
void process_operator(char c){
	switch(c){
	// only need to check one chracter
	case '(':
		current = LEFTPARENTHESIS;
		printf("%d\n",current);
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
		break;
	case ')':
		current = RIGHTPARENTHESIS;
		print_token();
		break;
	case '[':
		current = LEFTBRACKET;
		print_token();
		break;
	case ']':
		current = RIGHTBRACKET;
			print_token();
		break;
	case ',':
			current = COMMA;
<<<<<<< HEAD
				print_token();
			break;
	case '~':
			current = COMPLEMENT;
				print_token();
			break;
	case '^':
			current = XOR;
				print_token();
			break;
	case '*':
			current = MULTIPLICATION;
				print_token();
			break;
	case '/':
	/*if (previous == DIVISION){
		  current = SINGLELINECOMMENT;
			previous = SINGLELINECOMMENT;
			break;
		}*/
			current = DIVISION;
				print_token();
			break;
	case '!':
			current = NEGATE;
				print_token();
			break;
	case '?':
			current = CONDITIONALTRUE;
				print_token();
			break;
	case ':':
			current = CONDITIONALFALSE;
				print_token();
			break;
	case '%':
			current = MOD;
				print_token();
			break;

	// need to check the previous type to confirm the type of token
=======
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
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
	case '+':
		if (previous == ADDITION){
			current = INCREMENT;
			previous = INCREMENT;
			break;
		}
		else if (previous != FLOAT){
			current = ADDITION;
		}
			break;
	case '-':
		if (previous == SUBTRACTION){
			current = DECREMENT;
			previous = DECREMENT;
			break;
		}
		else if (previous != FLOAT){
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
<<<<<<< HEAD
=======
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
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
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
<<<<<<< HEAD
	// nested switch statment for operators containing =
=======
	// nested switch statment
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
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
<<<<<<< HEAD

	default:
	//digit value for carriage return, newLine, and current characters
  /*int carriage = (int)'\r';
	int newLine = (int)'\n';
	int cNumber = (int)c;
	if( previous == SINGLELINECOMMENT){
		if( cNumber != carriage && cNumber != newLine ){
			current = SINGLELINECOMMENT;
		}
	}
	*/

	break;
=======
	//string compare for sizeof operator
	default:
		if (current == WORD){
			 if(strcmp(token,"sizeo") == 0 && c =='f'){
					 previous = SIZEOF;
	  				 current = SIZEOF;
			 }
		}
		break;
>>>>>>> 8c0909869c195c9e2b271cb936d700ba7a3f64ee
	}
}

/*
tests if the current token is a specific keyword
parameter : (String representation of keyword, tokenType of keyword, current char from input string)
*/
void isKeyword( char* keyword, tokenType typeOfKeyword, char currentChar){
	if (current == WORD){
	int keywordLength = strlen(keyword);
	int tokenLength = strlen(token);
	if(strncmp(token, keyword, keywordLength-1) == 0 && currentChar == keyword[keywordLength - 1]
		&& tokenLength == keywordLength-1 ){
	previous = typeOfKeyword;
	current = typeOfKeyword;
	}
	else if (previous == typeOfKeyword){

	previous = WORD;
	current = WORD;
	}
	}
	}

/*
checks if the token is a any of the 32 keywords in c and sets the token appropriatly
parameter : (current char from input string)
*/
void identitfyKeyword(char c){
	isKeyword("sizeof", SIZEOF, c);
	isKeyword("int", INT, c);
  isKeyword("float", FLOAT, c);
	isKeyword("short", SHORT, c);
	isKeyword("double", DOUBLE, c);
  isKeyword("auto", AUTO, c);
	isKeyword("break", BREAK, c);
	isKeyword("case", CASE, c);
	isKeyword("char", CHAR, c);
	isKeyword("const", CONST, c);
	isKeyword("continue", CONTINUE, c);
	isKeyword("default", DEFAULT, c);
	isKeyword("do", DO, c);
	isKeyword("else", ELSE, c);
	isKeyword("enum", ENUM, c);
	isKeyword("extern", EXTERN, c);
	isKeyword("struct", STRUCT, c);
	isKeyword("long", LONG, c);
	isKeyword("switch", SWITCH, c);
	isKeyword("register", REGISTER, c);
	isKeyword("typedef", TYPEDEF, c);
	isKeyword("return", RETURN, c);
	isKeyword("union", UNION, c);
	isKeyword("for", FOR, c);
	isKeyword("void", SIGNED, c);
	isKeyword("if", IF, c);
	isKeyword("static", STATIC, c);
	isKeyword("while", WHILE, c);
	isKeyword("goto", GOTO, c);
  isKeyword("volatile", VOLATILE, c);
	isKeyword("unsigned", UNSIGNED, c);
	isKeyword("signed", SIGNED, c);
}

int main(int argc, char *argv[]){
/*
	if (argc < 2){
	printf("Please enter string to tokenize.");
	return 0;
	}
	// copies argv[1] into variable input
	int length = strlen(argv[1]);
	printf("%d",length);
	char* input = malloc(length+1 * sizeof(char));
	strcpy(input,argv[1]);
	tokenize(input);


	}
*/
	FILE *fptr = fopen(argv[1], "r");
	run_tests(fptr);
	fclose(fptr);
	return 0;

}





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
