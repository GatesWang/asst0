#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//list of token types
typedef enum _type {
  NONE,
  SPACE,
  STRUCT_MEM,
  WORD,
  DECIMAL,
  FLOAT,
  FLOAT_WITH_EXPO,
  OCTAL,
  HEX,
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
  SIZEOF,
  INT,
  SHORT,
  DOUBLE,
  AUTO,
  BREAK,
  CASE,
  CHAR,
  CONST,
  CONTINUE,
  DEFAULT,
  DO,
  ELSE,
  ENUM,
  EXTERN,
  STRUCT,
  LONG,
  REGISTER,
  SWITCH,
  TYPEDEF,
  RETURN,
  UNION,
  FOR,
  SIGNED,
  VOID,
  IF,
  STATIC,
  WHILE,
  GOTO,
  VOLATILE,
  UNSIGNED,
  SINGLEQUOTE,
  DOUBLEQUOTE,
  QUOTEEND,
  FRONTSLASH,
  SINGLELINECOMMENT
}
tokenType;
tokenType current = NONE;
tokenType previous = NONE;

//function declerations
int run_tests(FILE * f);

int is_octal(char * string, int i);
int is_hex(char * string, int i);
int is_digit(char * string, int i);
int starts_with(char * prefix, char * string);

void process_operator(char c);
void process_index(char * input, int * i);
void process_alpha(char * input, int * i);
void process_digit(char * input, int * i);

void set_previous_and_current(char * input, int * i);
void setTokenTypeString(tokenType type);
void print_token();

void identifyOperator(char * input, int j);
void identifyKeyword(char c);
void isKeyword(char * keyword, tokenType typeOfKeyword, char currentChar);

char tokenTypeString[30];
char * token;

/*processes the input string then tokenizes it and prints out the result
parameter : (input string) */
int tokenize(char * input) {
  current = NONE;
  previous = NONE;
  token = malloc(1000 * sizeof(char));
  for (int i = 0; i < strlen(input); i++) {
    process_index(input, & i);
  }
  setTokenTypeString(current);
  // prints last token
  printf("%s: \"%s\"\n", tokenTypeString, token);
  return 0;
}

/*processes the current char of the input string and assigns it a token type
parameter : (input string, current index) */
void process_index(char * input, int * i) {
  set_previous_and_current(input, i);

  if (previous != current) {
	print_token();
  }
  //copies current char of the input string to the end of the token string if it not a white space character
  char c = input[ * i];
  if (current != SPACE) {
    sprintf(token, "%s%c", token, c);
  }
}

//prints the token and token type
void print_token() {
  //prints previous
  setTokenTypeString(previous);
  if (strlen(token) > 0) {
    printf("%s: \"%s\"\n", tokenTypeString, token);
  }
  //set token type for current
  setTokenTypeString(current);
  token[0] = '\0';
  if(current == HEX){
      strcat(token, "0");
  }
}


int previousWasFloating(){ return previous == FLOAT;}
int previousWasFloatingExpo(){ return previous == FLOAT_WITH_EXPO;}
int previousWasWord(){return previous == WORD;}
int previousWasDecimal(){return previous == DECIMAL;}
int previousWasHex(){return previous == HEX;}
int previousWasOctal(){return previous == OCTAL;}

/* sets or modifies the type for the previous and current token
	parameter : (input string, current index) */
void set_previous_and_current(char * input, int * i) {
  int j = *i;
  char c = input[j];

  previous = current;

  if (isspace(c)) {
    current = SPACE;
  } else if (isalpha(c)) {
    process_alpha(input, i);
	return;
  } else if (isdigit(c)) {
    process_digit(input, i);
	return;
  }
  identifyOperator(input, j);
  identifyKeyword(c);
}

/*processes the index at i, which is assumed to be a digit
parameter : (input string, index i) */
void process_digit(char * input, int * i){
	int j = *i;
	int is_valid_hex = starts_with("0x", & input[j]) || starts_with("0X", & input[j]);
    int is_valid_octal = starts_with("0", & input[j]);

    if (!previousWasWord() && !previousWasDecimal() && !previousWasOctal() && !previousWasFloating() && is_valid_hex) {
      current = HEX;
      previous = previousWasHex() ? NONE : previous;
      (*i)++; //skip the '0' and go to 'x' or 'X'
    } else if (!previousWasWord() && !previousWasHex && !previousWasDecimal() && !previousWasOctal() && !previousWasFloating() && is_valid_octal) {
      current = OCTAL;
      previous = previousWasOctal() ? NONE : previous;
    } else if (previousWasOctal() && !is_octal(input, j)) {
      current = DECIMAL;
    } else if (!previousWasHex() && !previousWasWord() && !previousWasOctal() && !previousWasFloating() && !previousWasFloatingExpo()) {
      current = DECIMAL;
	}
}

/*processes the index at i which is assumed to be an alpha
parameter : (input string) */
void process_alpha(char * input, int * i){
	int j = *i;
	int valid_exponent1 = starts_with("e", & input[j]) && is_digit(input, j + 1);
    int valid_exponent2 = starts_with("E", & input[j]) && is_digit(input, j + 1);
    int valid_exponent3 = starts_with("e-", & input[j]) && is_digit(input, j + 2);
    int valid_exponent4 = starts_with("e+", & input[j]) && is_digit(input, j + 2);
    int valid_exponent5 = starts_with("E+", & input[j]) && is_digit(input, j + 2);
    int valid_exponent6 = starts_with("E-", & input[j]) && is_digit(input, j + 2);
    int no_exponent = is_digit(input, j);

    if(previousWasFloating()){
		if(valid_exponent1 || valid_exponent2){
			previous = FLOAT_WITH_EXPO;
			current = FLOAT_WITH_EXPO;
		}
		else if(valid_exponent3 || valid_exponent4 || valid_exponent5 || valid_exponent6){
			(*i)++; //skip the '+ or '-'
		    sprintf(token, "%s%c", token, input[j]);
			previous = FLOAT_WITH_EXPO;
			current = FLOAT_WITH_EXPO;
		}
		else if(!no_exponent){
			current = WORD;
		}
	}
    else if (!previousWasHex()) {
		current = WORD;
    }
	else if (previousWasHex() && !is_hex(input, j)) {
		current = WORD;
    }
}

// decodes the token type into its proper string representation
void setTokenTypeString(tokenType type) {
  switch (type) {
  case WORD:
    sprintf(tokenTypeString, "word");
    break;
  case DECIMAL:
    sprintf(tokenTypeString, "decimal integer");
    break;
  case OCTAL:
    sprintf(tokenTypeString, "octal integer");
    break;
  case HEX:
    sprintf(tokenTypeString, "hexadecimal integer");
    break;
  case FLOAT_WITH_EXPO:
    sprintf(tokenTypeString, "float");
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
/*
	compares a prefix string with the input string beginning at the current index
	returns integer > 0 if string begins with the prefix
	parameter : (prefix string, input string)
*/
int starts_with(char * prefix, char * string) {
  return strncmp(prefix, string, strlen(prefix)) == 0;
}

/*
	returns an integer > 0 if the current char is a valid octal digit
	parameter : (input string, current index)
*/
int is_octal(char * string, int i) {
  if (i < strlen(string)) {
    int d = (string[i] - '0');
    return (d >= 0 && d <= 7);
  }
  return 0;
}

/*
 returns a non zero value if the current char is a valid hex digit
	parameter : (input string, current index)
*/
int is_hex(char * string, int i) {
  return i < strlen(string) && isxdigit(string[i]);
}

/*
	returns an integer > 0 if the current char is a
	valid decimal digit.
	parameter : (input string, current index)
*/
int is_digit(char * string, int i) {
	return i < strlen(string) && isdigit(string[i]);
}

/*
idenitifies if the current token is an operator, and assigns it the proper token.
parameter : (current char from input string)
*/
void identifyOperator(char * input, int j) {
  char c = input[j];
  switch (c) {
    // only needs to check one chracter
  case '.' :
    if ((previousWasDecimal() || previousWasOctal()) && is_digit(input, j + 1)) {
      previous = FLOAT;
      current = FLOAT;
    } else {
      current = STRUCT_MEM;
	  print_token();
    }
	break;
  case '(':
    current = LEFTPARENTHESIS;
    print_token();
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
  case '+':
    if (previous == ADDITION) {
      current = INCREMENT;
      previous = INCREMENT;
      break;
    } else if (previous != FLOAT) {
      current = ADDITION;
    }
    break;
  case '-':
    if (previous == SUBTRACTION) {
      current = DECREMENT;
      previous = DECREMENT;
      break;
    } else if (previous != FLOAT) {
      current = SUBTRACTION;
    }
    break;
  case '<':
    if (previous == LESSTHAN) {
      current = SHIFTLEFT;
      previous = SHIFTLEFT;
      break;
    }
    current = LESSTHAN;
    break;
  case '>':
    if (previous == GREATERTHAN) {
      current = SHIFTRIGHT;
      previous = SHIFTRIGHT;
      break;
    } else if (previous == SUBTRACTION) {
      current = STRUCTUREPOINTER;
      previous = STRUCTUREPOINTER;
      break;
    } else {
      current = GREATERTHAN;
      break;
    }
    // nested switch statment for operators containing =
    case '=':
      switch (previous) {
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
        previous = ANDEQUALS;
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
      if (previous == BITWISEOR) {
        current = LOGICALOR;
        previous = LOGICALOR;
        break;
      }
      current = BITWISEOR;
      break;
    case '&':
      if (previous == AND) {
        current = LOGICALAND;
        previous = LOGICALAND;
        break;
      }
      current = AND;
      break;
  }
}

/*
tests if the current token is a specific keyword
parameter : (String representation of keyword, tokenType of keyword, current char from input string)
*/
void isKeyword(char * keyword, tokenType typeOfKeyword, char currentChar) {
  if (current == WORD) {
    int keywordLength = strlen(keyword);
    int tokenLength = strlen(token);
    if (strncmp(token, keyword, keywordLength - 1) == 0 && currentChar == keyword[keywordLength - 1] &&
      tokenLength == keywordLength - 1) {
      previous = typeOfKeyword;
      current = typeOfKeyword;
    }
    else if (previous == typeOfKeyword) {
      previous = WORD;
      current = WORD;
    }
  }
}

/*
checks if the token is a any of the 32 keywords in c and sets the token appropriatly
parameter : (current char from input string)
*/
void identifyKeyword(char c) {
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

int main(int argc, char * argv[]) {
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
  FILE * fptr = fopen(argv[1], "r");
  run_tests(fptr);
  fclose(fptr);
  return 0;

}

int run_tests(FILE * f) {
  int c;
  while (c != EOF) {
    //discard text before opening quotation
    while ((c = fgetc(f)) != '\"' && c != EOF) {}
    //read until closing quotation
    char * input = malloc(1000);
    size_t n = 0;
    while ((c = fgetc(f)) != '\"' && c != EOF) {
      input[n++] = (char) c;
    }
    input[n] = '\0';
    if (n > 0) {
      printf("............................\n");
      printf("%s\n", input);
      printf("............................\n");
      tokenize(input);
    }
  }
}
