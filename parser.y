%{
#include <stdio.h>
%}

/* Declaração dos tokens da gramática da Linguagem K */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
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

%%
 /* Regras (e ações) da gramática da Linguagem K */

s: declaracao-varglobal ';' s |
    declaracao-funcao s |
    
;

tipo: TK_PR_INT | 
          TK_PR_FLOAT |
          TK_PR_BOOL |
          TK_PR_CHAR |
          TK_PR_STRING
;

declaracao-funcao: tipo TK_IDENTIFICADOR '(' parametros-declaracao-funcao ')' '{' comando '}' // definir lista-argumentos
;

declaracao-varglobal: declaracao-var-simples |
	declaracao-vetor
;
	//declaracao-varlocal: declaracao-var-simples
	;
declaracao-var-simples: tipo ':' TK_IDENTIFICADOR
;
declaracao-vetor: tipo ':' TK_IDENTIFICADOR '[' expr ']'
;

condicional: if-simples | if-then-else
;
if-simples: TK_PR_IF '(' expr ')' TK_PR_THEN comando 
;
if-then-else: TK_PR_IF '(' TK_PR_THEN comando TK_PR_ELSE comando
;

laco: while |
	do-while
;
while: TK_PR_WHILE '(' expr ')' TK_PR_DO comando
;
do-while: TK_PR_DO comando TK_PR_WHILE '(' expr ')'
;

comando: '{' comando '}' | 
    comando-simples ';' comando |
    comando-simples
;
comando-simples: comando-entrada |
    comando-saida |
    comando-retorno | 
    atribuicao |
    condicional |
    laco
;
comando-entrada: TK_PR_INPUT TK_IDENTIFICADOR
;
comando-saida: TK_PR_OUTPUT TK_IDENTIFICADOR
;
comando-retorno: TK_PR_RETURN
;
atribuicao: atribuicao-simples |
    atribuicao-vetor
;
atribuicao-simples: TK_IDENTIFICADOR '=' expr
;
atribuicao-vetor: TK_IDENTIFICADOR '[' expr ']' '=' expr
;

expr: '(' expr ')' |
    expr TK_OC_LE expr |
    expr TK_OC_GE expr |
    expr TK_OC_EQ expr |
    expr TK_OC_NE expr |
    expr TK_OC_AND expr |
    expr TK_OC_OR expr |
    expr '=' expr |
    termo | 
    TK_IDENTIFICADOR '(' parametros-chamada-funcao ')'
;

parametros-declaracao-funcao: tipo TK_IDENTIFICADOR | 
    tipo TK_IDENTIFICADOR ',' parametros-declaracao-funcao |
    
;
parametros-chamada-funcao: TK_IDENTIFICADOR |
    TK_IDENTIFICADOR ',' parametros-chamada-funcao |
    
;

termo: TK_IDENTIFICADOR |
           TK_LIT_INT |
           TK_LIT_FLOAT |
           TK_LIT_FALSE |
           TK_LIT_TRUE |
           TK_LIT_CHAR |
           TK_LIT_STRING
;
%%