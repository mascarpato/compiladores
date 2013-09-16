%{
#include <stdio.h>
#include "iks_ast.h"
#include "comp_tree.h"

#define YYERROR_VERBOSE

struct treeNode_t *ast = NULL;
%}

%define api.value.type {struct treeNode_t*}

/* Declaração dos tokens da gramática da Linguagem K */

%initial-action {
	Data data;
	data.nodeType = IKS_AST_PROGRAMA;
	data.symEntry = NULL;
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

condicional: TK_PR_IF '(' expr ')' TK_PR_THEN comando {
			Data data;
			data.nodeType = IKS_AST_IF_ELSE;
			data.symEntry = NULL;
			comp_tree_t *father = treeCreate(data);
			
			treeInsert($3, father);
			treeInsert($6, father);
			treeInsert(NULL, father); // No else command
			
			$$ = father; }
	| TK_PR_IF '(' expr ')' TK_PR_THEN comando TK_PR_ELSE comando {
			Data data;
			data.nodeType = IKS_AST_IF_ELSE;
			data.symEntry = NULL;
			comp_tree_t *father = treeCreate(data);
			
			treeInsert($3, father);
			treeInsert($6, father);
			treeInsert($8, father); // No else command
			
			$$ = father; }
;

laco: do-while
	| while
;

	// c-style do while
do-while: TK_PR_DO comando TK_PR_WHILE '(' expr ')' ';' {
		Data data;
		data.nodeType = IKS_AST_DO_WHILE;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($2, father);
		treeInsert($5, father);
		
		$$ = father; } 
;
while: TK_PR_WHILE '(' expr ')' TK_PR_DO comando {
		Data data;
		data.nodeType = IKS_AST_WHILE_DO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($3, father);
		treeInsert($6, father);
		
		$$ = father; } 
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
atribuicao-simples: TK_IDENTIFICADOR '=' expr {
		Data data;
		data.nodeType = IKS_AST_ATRIBUICAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
;
atribuicao-vetor: vetor '=' expr {
		Data data;
		data.nodeType = IKS_AST_ATRIBUICAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
;

expr: '(' expr ')' { $$ = $2; }
	| expr TK_OC_LE expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_COMP_LE;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr TK_OC_GE expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_COMP_GE;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr TK_OC_EQ expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_COMP_IGUAL;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr TK_OC_NE expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_COMP_DIF;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr '<' expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_COMP_L;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr '>' expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_COMP_G;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
		// | expr '=' expr { Eliminado a partir da etapa 3
		//
		//11}
	| expr TK_OC_AND expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_E;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr TK_OC_OR expr {
		Data data;
		data.nodeType = IKS_AST_LOGICO_OU;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr '+' expr { 
		Data data;
		data.nodeType = IKS_AST_ARIM_SOMA;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father);
		
		$$ = father; }
	| expr '-' expr {
		Data data;
		data.nodeType = IKS_AST_ARIM_SUBTRACAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father); 
		
		$$ = father; }
	| expr '*' expr {
		Data data;
		data.nodeType = IKS_AST_ARIM_MULTIPLICACAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father); 
		
		$$ = father; }
	| expr '/' expr {
		Data data;
		data.nodeType = IKS_AST_ARIM_DIVISAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($1, father);
		treeInsert($3, father); 
		
		$$ = father; }
	/*| '!' expr { // Adicionado na etapa 3 - Gera conflito shift-reduce
		Data data;
		data.nodeType = IKS_AST_LOGICO_COMP_NEGACAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($2, father);
		
		$$ = father; }*/
	| termo { $$ = $1; }
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

termo: TK_IDENTIFICADOR { $$ = $1; }
	| vetor
	| chamada-funcao 
	| integer { $$ = $1; }
	| float { $$ = $1; }
	| TK_LIT_FALSE { $$ = $1; }
	| TK_LIT_TRUE { $$ = $1; }
	| TK_LIT_CHAR { $$ = $1; }
	| TK_LIT_STRING { $$ = $1; }
;

integer: TK_LIT_INT { $$ = $1; }
	| '-' TK_LIT_INT { 
		Data data;
		data.nodeType = IKS_AST_ARIM_INVERSAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($2, father);
		$$ = $2;
		//$2->data.symEntry->symbol.value.value_int = -$2->data.symEntry->symbol.value.value_int;
		}
;

float: TK_LIT_FLOAT { $$ = $1; }
	| '-' TK_LIT_FLOAT { 
		Data data;
		data.nodeType = IKS_AST_ARIM_INVERSAO;
		data.symEntry = NULL;
		comp_tree_t *father = treeCreate(data);
		
		treeInsert($2, father);
		$$ = $2;
		
		//$2->data.symEntry->symbol.value.value_float = -$2->data.symEntry->symbol.value.value_float; }
		}
;
%%
