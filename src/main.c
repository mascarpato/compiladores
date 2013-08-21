/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 */

#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

#define SYM_TABLE_INITSIZE 100
Dict *sym_table;

extern char *yytext;

int main (int argc, char **argv)
{
  int token = TOKEN_ERRO;
  sym_table = dict_create(SYM_TABLE_INITSIZE);
  
  while (token = yylex()){
    printf ("token <%d> at %d\n", token, getLineNumber());
	if (token == TK_PR_INT) {
		dict_insert(sym_table, yytext, yytext, getLineNumber());
	}
	else if (token == TK_PR_FLOAT) {
		dict_insert(sym_table, yytext, yytext, getLineNumber());
	}
	else if (token == TK_PR_BOOL) {
		dict_insert(sym_table, yytext, yytext, getLineNumber());
	}
	else if (token == TK_PR_CHAR) {
		dict_insert(sym_table, yytext, yytext, getLineNumber());
	}
	else if (token == TK_PR_STRING) {
		dict_insert(sym_table, yytext, yytext, getLineNumber());
	}
	if (token == TK_IDENTIFICADOR) {
		dict_insert(sym_table, yytext, yytext, getLineNumber());
	}
  }
  
  //dict_print(sym_table);
  dict_terminate(sym_table);
  
  return 0;
}
