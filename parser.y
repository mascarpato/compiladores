%{
#include <stdio.h>
#include "comp_dict.h"

#define YYERROR_VERBOSE

%}

%union
{
	struct DictItem *symbol;
}

/* Declaração dos tokens da gramática da Linguagem K */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_DO
%token TK_PR_WHILE
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%left TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE '=' '<' '>'
%left TK_OC_AND TK_OC_OR
%left '-' '+'
%left '*' '/'

%nonassoc TK_PR_THEN 
%nonassoc TK_PR_ELSE

%%
 /* Regras (e ações) da gramática da Linguagem K */

s: declaracao-varglobal ';' s |
    declaracao-funcao s |
    /* empty */
;

tipo: TK_PR_INT | 
          TK_PR_FLOAT |
          TK_PR_BOOL |
          TK_PR_CHAR |
          TK_PR_STRING
;

declaracao-funcao: tipo ':' TK_IDENTIFICADOR '(' parametros-funcao-empty ')' lista-var-local comando-composto 
;

lista-var-local: declaracao-var-simples ';' |
				 declaracao-var-simples ';' lista-var-local |
				 /* empty */
;

declaracao-varglobal: declaracao-var-simples |
	declaracao-vetor
;

declaracao-var-simples: tipo ':' TK_IDENTIFICADOR
;
declaracao-vetor: tipo ':' vetor
;

vetor: TK_IDENTIFICADOR '[' expr ']'
;

comando: comando-composto | 
         comando-simples |
         /* empty */
;
comando-nao-vazio: comando-composto | 
         comando-simples
;
comando-composto: '{' comando-sequencia '}'
;
comando-sequencia: comando-simples |
		           comando-simples ';' comando-sequencia |
		           comando-composto |
		           comando-composto ';' comando-sequencia |
		           /* empty */
; 
comando-simples: condicional |
				 laco |
				 comando-entrada |
    			 comando-saida |
    			 comando-retorno | 
    			 atribuicao |
    			 declaracao-var-simples |
    			 chamada-funcao |
    			 ';'
;

condicional: TK_PR_IF '(' expr ')' TK_PR_THEN comando |
			 TK_PR_IF '(' expr ')' TK_PR_THEN comando TK_PR_ELSE comando-nao-vazio
;

laco: do-while |
	while
;

do-while: TK_PR_DO comando TK_PR_WHILE '(' expr ')' ';' // c-style
;
while: TK_PR_WHILE '(' expr ')' TK_PR_DO comando
;
comando-retorno: TK_PR_RETURN expr
;
comando-entrada: TK_PR_INPUT TK_IDENTIFICADOR
;
comando-saida: TK_PR_OUTPUT argumento-saida
;
argumento-saida: expr |
				 expr ',' argumento-saida 
;

atribuicao: atribuicao-simples |
    		atribuicao-vetor
;
atribuicao-simples: TK_IDENTIFICADOR '=' expr
;
atribuicao-vetor: vetor '=' expr
;

expr: '(' expr ')' |
    expr TK_OC_LE expr |
    expr TK_OC_GE expr |
    expr TK_OC_EQ expr |
    expr TK_OC_NE expr |
    expr '<' expr |
    expr '>' expr |
    expr '=' expr |
    expr TK_OC_AND expr |
    expr TK_OC_OR expr |
    expr '+' expr |
    expr '-' expr |
    expr '*' expr |
    expr '/' expr |
    termo
;

chamada-funcao: TK_IDENTIFICADOR '(' parametros-chamada-funcao ')'
;

parametros-funcao-empty : parametros-declaracao-funcao |
									 /* empty */
;

parametros-declaracao-funcao: tipo ':' TK_IDENTIFICADOR | 
    tipo ':' TK_IDENTIFICADOR ',' parametros-declaracao-funcao
;
parametros-chamada-funcao: termo |
    termo ',' parametros-chamada-funcao |
    /* empty */ 
;

termo: TK_IDENTIFICADOR |
		vetor |
		chamada-funcao |
           integer |
           float |
           TK_LIT_FALSE |
           TK_LIT_TRUE |
           TK_LIT_CHAR |
           TK_LIT_STRING
;

integer: TK_LIT_INT |
		 '-' TK_LIT_INT
;

float: TK_LIT_FLOAT |
		 '-' TK_LIT_FLOAT
;
%%

int yyerror (char *mensagem)
{
  fprintf (stderr, "%s at line %d\n", mensagem, getLineNumber());
}
