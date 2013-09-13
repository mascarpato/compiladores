%{
#include <stdio.h>
//#include "comp_symbols.h"
#include "comp_dict.h"
#include "comp_tree.h"

#define YYERROR_VERBOSE

struct treeNode_t *ast = NULL;
%}

%define api.value.type {struct treeNode_t *}

/* Declaração dos tokens da gramática da Linguagem K */

%initial-action {
	Data data;
	data.nodeType = IKS_AST_PROGRAMA;
	data.symbol = NULL;
	ast = treeCreate(data);
}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%nonassoc TK_PR_THEN 
%nonassoc TK_PR_ELSE
%token TK_PR_DO
%token TK_PR_WHILE
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%left TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE '=' '<' '>'
%left TK_OC_AND TK_OC_OR

%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO
%left '-' '+'
%left '*' '/'

/* Nao terminais */ 
//%type <struct treeNode_t*> s

%%
/* Regras (e ações) da gramática da Linguagem K */
s: declaracao-varglobal ';' s { }
	| declaracao-funcao s {
/*			Data data;
			data.nodeType = IKS_AST_FUNCAO;
			data.symbol = NULL;
			comp_tree_t *newNode = treeCreate(data);
			
			treeInsert(newNode, ast);*/
			}
    | { }
;

tipo: TK_PR_INT
	| TK_PR_FLOAT
	| TK_PR_BOOL
	| TK_PR_CHAR
	| TK_PR_STRING
;

declaracao-funcao: tipo ':' TK_IDENTIFICADOR '(' parametros-funcao-empty ')' lista-var-local comando-composto
;

lista-var-local: declaracao-var-simples ';' lista-var-local
	| /* empty */
;

declaracao-varglobal: declaracao-var-simples
	| declaracao-vetor
;

declaracao-var-simples: tipo ':' TK_IDENTIFICADOR { }
;
declaracao-vetor: tipo ':' vetor
;

vetor: TK_IDENTIFICADOR '[' expr ']'
;

comando: comando-composto 
	| comando-simples
;
comando-composto: '{' comando-sequencia '}'
;
comando-sequencia: comando-simples
	| comando-simples ';' comando-sequencia 
	| comando-composto
	| comando-composto ';' comando-sequencia
	| /* empty */
; 
comando-simples: condicional
	| laco
	| comando-entrada
    | comando-saida
    | comando-retorno
    | atribuicao
    | declaracao-var-simples
    | chamada-funcao
    | ';'
;

condicional: TK_PR_IF '(' expr ')' TK_PR_THEN comando
	| TK_PR_IF '(' expr ')' TK_PR_THEN comando TK_PR_ELSE comando
;

laco: do-while
	| while
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
argumento-saida: expr
	| expr ',' argumento-saida 
;

atribuicao: atribuicao-simples
    | atribuicao-vetor
;
atribuicao-simples: TK_IDENTIFICADOR '=' expr
;
atribuicao-vetor: vetor '=' expr
;

expr: '(' expr ')'
	| expr TK_OC_LE expr 
	| expr TK_OC_GE expr
	| expr TK_OC_EQ expr
	| expr TK_OC_NE expr
	| expr '<' expr
	| expr '>' expr
	| expr '=' expr
	| expr TK_OC_AND expr
	| expr TK_OC_OR expr
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| termo
;

chamada-funcao: TK_IDENTIFICADOR '(' parametros-chamada-funcao ')'
;

parametros-funcao-empty : parametros-declaracao-funcao
	| /* empty */
;

parametros-declaracao-funcao: tipo ':' TK_IDENTIFICADOR
    | tipo ':' TK_IDENTIFICADOR ',' parametros-declaracao-funcao
;
parametros-chamada-funcao: termo
    | termo ',' parametros-chamada-funcao
    | /* empty */ 
;

termo: TK_IDENTIFICADOR
	| vetor
	| chamada-funcao	
	| integer
	| float
	| TK_LIT_FALSE {
			Data data;
			data.nodeType = IKS_AST_LITERAL;
			data.
			treeCreate()
			}
	| TK_LIT_TRUE
	| TK_LIT_CHAR 
	| TK_LIT_STRING
;

integer: TK_LIT_INT
	| '-' TK_LIT_INT
;

float: TK_LIT_FLOAT
	| '-' TK_LIT_FLOAT
;
%%
