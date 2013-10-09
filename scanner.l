%{
#include <string.h>
#include "iks_ast.h"
#include "comp_tree.h"

#include "parser.h"

int num_lines=1;
int pos_in_line = 0;
int num_chars=0;

void inc_char_count();

SymValue_t encodeSymVal(char *lexeme, int symbolType);
Symbol_t encodeSymbol(char *lexeme, int symbolType);

%}

%x IN_COMMENT

%%
" " { inc_char_count(); }
"	" { inc_char_count(); }
"int" { inc_char_count(); return TK_PR_INT; }
"float" { inc_char_count(); return TK_PR_FLOAT; }
"bool" { inc_char_count(); return TK_PR_BOOL; }
"char" { inc_char_count(); return TK_PR_CHAR; }
"string" { inc_char_count(); return TK_PR_STRING; }
"if" { inc_char_count(); return TK_PR_IF; }
"then" { inc_char_count(); return TK_PR_THEN; }
"else" { inc_char_count(); return TK_PR_ELSE; }
"while" { inc_char_count(); return TK_PR_WHILE; }
"do" { inc_char_count(); return TK_PR_DO; }
"input" { inc_char_count(); return TK_PR_INPUT; }
"output" { inc_char_count(); return TK_PR_OUTPUT; }
"return" { inc_char_count(); return TK_PR_RETURN; }

"<=" { inc_char_count(); return TK_OC_LE; }
">=" { inc_char_count(); return TK_OC_GE; }
"==" { inc_char_count(); return TK_OC_EQ; }
"!=" { inc_char_count(); return TK_OC_NE; }
"&&" { inc_char_count(); return TK_OC_AND; }
"||" { inc_char_count(); return TK_OC_OR; }

"," { inc_char_count(); return (int) yytext[0]; }
";" { inc_char_count(); return (int) yytext[0]; }
":" { inc_char_count(); return (int) yytext[0]; }
"(" { inc_char_count(); return (int) yytext[0]; }
")" { inc_char_count(); return (int) yytext[0]; }
"[" { inc_char_count(); return (int) yytext[0]; }
"]" { inc_char_count(); return (int) yytext[0]; }
"{" { inc_char_count(); return (int) yytext[0]; }
"}" { inc_char_count(); return (int) yytext[0]; }
"+" { inc_char_count(); return (int) yytext[0]; }
"-" { inc_char_count(); return (int) yytext[0]; }
"*" { inc_char_count(); return (int) yytext[0]; }
"/" { inc_char_count(); return (int) yytext[0]; }
"<" { inc_char_count(); return (int) yytext[0]; }
">" { inc_char_count(); return (int) yytext[0]; }
"=" { inc_char_count(); return (int) yytext[0]; }
"!" { inc_char_count(); return (int) yytext[0]; }
"&" { inc_char_count(); return (int) yytext[0]; }
"$" { inc_char_count(); return (int) yytext[0]; }

[[:digit:]]+ 	{ 	// Scanned INTEGER 	
					inc_char_count();
					
					DictItem *symEntry = dict_insert(encodeSymbol(yytext, SYMTYPE_INT), yytext,  getLineNumber());
					
					yylval.symEntry = symEntry;
					
					return TK_LIT_INT; }
					
[[:digit:]]+"."[[:digit:]]+ { // Scanned FLOAT
					inc_char_count();
					
					DictItem *symEntry = dict_insert(encodeSymbol(yytext, SYMTYPE_FLOAT), yytext,  getLineNumber());
					
					yylval.symEntry = symEntry;
					
					return TK_LIT_FLOAT; }
					
false 			{	// Scanned FALSE
					inc_char_count();
					
					DictItem *symEntry = dict_insert(encodeSymbol(yytext, SYMTYPE_BOOL), yytext,  getLineNumber());
					
					yylval.symEntry = symEntry;
					
					return TK_LIT_FALSE; }
					
true 			{	// Scanned TRUE
					inc_char_count(); 
					
					DictItem *symEntry = dict_insert(encodeSymbol(yytext, SYMTYPE_FLOAT), yytext,  getLineNumber());
					
					yylval.symEntry = symEntry;
					
					return TK_LIT_TRUE; }
					
'[[:print:]]'   {	// Scanned CHAR
					inc_char_count(); 
					
					char *aux = strdup(yytext);
					aux[strlen(aux)-1] = '\0';
					
					DictItem *symEntry = dict_insert(encodeSymbol(yytext, SYMTYPE_CHAR), aux+1,  getLineNumber());

					yylval.symEntry = symEntry;
					free(aux);
					
					return TK_LIT_CHAR; }
					
\"[^"]*\" 		{	// Scanned STRING
					inc_char_count(); 
					
					char *aux = strdup(yytext);
					aux[strlen(aux)-1] = '\0';
					
					DictItem *symEntry = dict_insert(encodeSymbol(yytext, SYMTYPE_STRING), aux+1,  getLineNumber());
					
					yylval.symEntry = symEntry;
					
					free(aux);
					
					return TK_LIT_STRING; }
					
(_|[[:alpha:]])(_|[[:alnum:]])* { // Scanned identifier. Is the name of some variable or function.
					inc_char_count();
					
					DictItem *symEntry = dict_insert(encodeSymbol(yytext, SYMTYPE_IDENTIFIER), yytext,  getLineNumber());
					if (symEntry->valid != 1)
					  printf("Whoaaa something wrong here!!\n");
					
					yylval.symEntry = symEntry;					
					
					return TK_IDENTIFICADOR; }
"\n" { 
					inc_char_count(); pos_in_line=0; num_lines++; }

	/* Eats C-style multiline comments */
<INITIAL>{
"/*"   { BEGIN(IN_COMMENT); }
}
<IN_COMMENT>{
"*/"      { BEGIN(INITIAL); }
[^*\n]+   { inc_char_count(); } // eat comment in chunks
"*"       { inc_char_count(); } // eat the lone star
\n        { num_lines++; pos_in_line=0; }
}

	/* Eats C-style inline comments */
"//".*"\n" 		{ 
					inc_char_count(); pos_in_line=0; num_lines++; }
     
. 				{
					inc_char_count(); return TOKEN_ERRO; } //printf("Unexpected %s at line %d.\n", yytext, num_lines); 
%%

int getCharCount(void) {
	return num_chars;
}

int getLineNumber(void) {
  return num_lines;
}

void inc_char_count() {
	num_chars += strlen(yytext);
	pos_in_line += strlen(yytext);
}

SymValue_t encodeSymVal(char *lexeme, int symbolType) {
	SymValue_t retVal; 
	char *aux = strdup(lexeme);
	
	switch(symbolType) {
		case SYMTYPE_INT:
			retVal.value_int = atoi(lexeme); break;
		case SYMTYPE_FLOAT:
			retVal.value_float = atof(lexeme); break;
		case SYMTYPE_CHAR:
			retVal.value_char = lexeme[1]; break; // eats the ' ' 
		case SYMTYPE_STRING:
			aux[strlen(aux)-1] = '\0';
			retVal.value_string = strdup(aux+1); break; // eats the " "
		case SYMTYPE_BOOL:
			retVal.value_bool = (*lexeme == 't' ? TRUE : FALSE); break;
		default: 
			retVal.value_int = 0;
	}
	free(aux);
	
	return retVal;
}

Symbol_t encodeSymbol(char *lexeme, int symbolType) {
	Symbol_t retVal;
	retVal.symType = symbolType;
	//printf("Leu %s! SymbolType == %d!!\n", lexeme, symbolType);
	
	if (symbolType == SYMTYPE_IDENTIFIER)
		retVal.name = strdup(lexeme);
	else 
		retVal.name = NULL;
	retVal.value = encodeSymVal(lexeme, symbolType);
	
	return retVal;
}
