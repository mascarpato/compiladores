#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMTYPE_UNDEF 0
#define SYMTYPE_INT_LIT 1
#define SYMTYPE_FLOAT_LIT 2
#define SYMTYPE_CHAR_LIT 3
#define SYMTYPE_STRING_LIT 4
#define SYMTYPE_BOOL_LIT 5
#define SYMTYPE_IDENT 6

#define TRUE 1
#define FALSE 0

typedef struct symbol_t {
	int symType;
	union symbol_u {
		int data_int;
		double data_float;
		char data_char;
		char *data_string;
		short data_bool;
		char *ident;
	} symbol;
} Symbol;

void symSetInt(Symbol *s, int value);
void symSetFloat(Symbol *s, float value);
void symSetChar(Symbol *s, char value);
void symSetBool(Symbol *s, short value);
void symSetString(Symbol *s, char *value);
void symSetIdent(Symbol *s, char *value);