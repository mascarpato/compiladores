%{
#include "parser.h"
#include <string.h>
#include "comp_dict.h"
#define SYM_TABLE_INITSIZE 100
#define YY_USER_ACTION dict_create(SYM_TABLE_INITSIZE);

#define IKS_SIMBOLO_INDEFINIDO 0
#define IKS_SIMBOLO_LITERAL_INT 1
#define IKS_SIMBOLO_LITERAL_FLOAT 2
#define IKS_SIMBOLO_LITERAL_CHAR 3
#define IKS_SIMBOLO_LITERAL_STRING 4
#define IKS_SIMBOLO_LITERAL_BOOL 5
#define IKS_SIMBOLO_IDENTIFICADOR 6

int num_lines=1;
int pos_in_line = 0;
int num_chars=0;

void inc_charc() {
	num_chars += strlen(yytext);
	pos_in_line += strlen(yytext);
}

%}

%x IN_COMMENT

%%
" " { inc_charc(); }
"	" { inc_charc(); }
"int" { inc_charc(); return TK_PR_INT; }
"float" { inc_charc(); return TK_PR_FLOAT; }
"bool" { inc_charc(); return TK_PR_BOOL; }
"char" { inc_charc(); return TK_PR_CHAR; }
"string" { inc_charc(); return TK_PR_STRING; }
"if" { inc_charc(); return TK_PR_IF; }
"then" { inc_charc(); return TK_PR_THEN; }
"else" { inc_charc(); return TK_PR_ELSE; }
"while" { inc_charc(); return TK_PR_WHILE; }
"do" { inc_charc(); return TK_PR_DO; }
"input" { inc_charc(); return TK_PR_INPUT; }
"output" { inc_charc(); return TK_PR_OUTPUT; }
"return" { inc_charc(); return TK_PR_RETURN; }

"<=" { inc_charc(); return TK_OC_LE; }
">=" { inc_charc(); return TK_OC_GE; }
"==" { inc_charc(); return TK_OC_EQ; }
"!=" { inc_charc(); return TK_OC_NE; }
"&&" { inc_charc(); return TK_OC_AND; }
"||" { inc_charc(); return TK_OC_OR; }

"," { inc_charc(); return (int) yytext[0]; }
";" { inc_charc(); return (int) yytext[0]; }
":" { inc_charc(); return (int) yytext[0]; }
"(" { inc_charc(); return (int) yytext[0]; }
")" { inc_charc(); return (int) yytext[0]; }
"[" { inc_charc(); return (int) yytext[0]; }
"]" { inc_charc(); return (int) yytext[0]; }
"{" { inc_charc(); return (int) yytext[0]; }
"}" { inc_charc(); return (int) yytext[0]; }
"+" { inc_charc(); return (int) yytext[0]; }
"-" { inc_charc(); return (int) yytext[0]; }
"*" { inc_charc(); return (int) yytext[0]; }
"/" { inc_charc(); return (int) yytext[0]; }
"<" { inc_charc(); return (int) yytext[0]; }
">" { inc_charc(); return (int) yytext[0]; }
"=" { inc_charc(); return (int) yytext[0]; }
"!" { inc_charc(); return (int) yytext[0]; }
"&" { inc_charc(); return (int) yytext[0]; }
"$" { inc_charc(); return (int) yytext[0]; }

[[:digit:]]+ { inc_charc(); //-?
				yylval.symbol=dict_insert(yytext,IKS_SIMBOLO_LITERAL_INT,getLineNumber()); 
				return TK_LIT_INT; }
[[:digit:]]+"."[[:digit:]]+ { //-?
				inc_charc(); 
				yylval.symbol=dict_insert(yytext,IKS_SIMBOLO_LITERAL_FLOAT,getLineNumber()); 
				return TK_LIT_FLOAT; }
false { 		inc_charc(); 
				yylval.symbol=dict_insert(yytext,IKS_SIMBOLO_LITERAL_BOOL,getLineNumber()); 
				return TK_LIT_FALSE; }
true { 			inc_charc(); 
				yylval.symbol=dict_insert(yytext,IKS_SIMBOLO_LITERAL_BOOL,getLineNumber()); 
				return TK_LIT_TRUE; }
'[[:print:]]' { inc_charc(); 
				yylval.symbol=dict_insert(yytext,IKS_SIMBOLO_LITERAL_CHAR,getLineNumber()); 
				return TK_LIT_CHAR; }
\"[^"]*\" { 	inc_charc(); 
				yylval.symbol=dict_insert(yytext,IKS_SIMBOLO_LITERAL_STRING,getLineNumber()); 
				return TK_LIT_STRING; }
(_|[[:alpha:]])(_|[[:alnum:]])* { 
				inc_charc(); 
				yylval.symbol=dict_insert(yytext,IKS_SIMBOLO_IDENTIFICADOR,getLineNumber()); 
				return TK_IDENTIFICADOR; }
"\n" { inc_charc(); pos_in_line=0; num_lines++; }

<INITIAL>{
"/*"   { BEGIN(IN_COMMENT); }
}
<IN_COMMENT>{
"*/"      { BEGIN(INITIAL); }
[^*\n]+   { inc_charc(); } // eat comment in chunks
"*"       { inc_charc(); } // eat the lone star
\n        { num_lines++; pos_in_line=0; }
}

"//".*"\n" { inc_charc(); pos_in_line=0; num_lines++; }
     
. { inc_charc(); return TOKEN_ERRO; } //printf("Unexpected %s at line %d.\n", yytext, num_lines); 
%%

int getCharCount(void) {
	return num_chars;
}

int getLineNumber(void) {
  return num_lines;
}
