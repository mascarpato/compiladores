#include "../include/comp_symbols.h"

void symSetInt(Symbol *s, int value) {
	s->symType = SYMTYPE_INT_LIT;
	s->symbol.data_int = value;
}

void symSetFloat(Symbol *s, float value) {
	s->symType = SYMTYPE_FLOAT_LIT;
	s->symbol.data_float = value;
}

void symSetChar(Symbol *s, char value) {
	s->symType = SYMTYPE_CHAR_LIT;
	s->symbol.data_char = value;
}

void symSetBool(Symbol *s, short value) {
	s->symType = SYMTYPE_BOOL_LIT;
	if (value > 0)
		s->symbol.data_bool = TRUE;
	else
		s->symbol.data_bool = FALSE;
}

void symSetString(Symbol *s, char *value) {
	s->symType = SYMTYPE_STRING_LIT;
	s->symbol.data_string = malloc(sizeof(char)*(strlen(value))+1);
	strcpy(s->symbol.data_string, value);
}

void symSetIdent(Symbol *s, char *value) {
	s->symType = SYMTYPE_IDENT;
	s->symbol.ident = malloc(sizeof(char)*(strlen(value))+1);
	strcpy(s->symbol.ident, value);
}