%{
#include <stdio.h>
#include "iks_ast.h"
#include "comp_tree.h"

#define YYERROR_VERBOSE
#define YYSTYPE struct treeNode_t*

struct treeNode_t *ast = NULL;
int stubFunctionParser(comp_tree_t *node) {
	int i, j;
	j = 10;
	return 0;
}

%}

//%define api.value.type {struct treeNode_t*}

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
s: declaracao-varglobal ';' s { $$ = NULL; } //TODO ??? Verificar 
    | declaracao-funcao s { 
                    $$ = $1;
                    treeInsert($1, ast); } //TODO ??? Verificar 
    |
;

tipo: TK_PR_INT { $$ = NULL; } // Nao vai pra arvore
    | TK_PR_FLOAT { $$ = NULL; } // Nao vai pra arvore
    | TK_PR_BOOL { $$ = NULL; } // Nao vai pra arvore
    | TK_PR_CHAR { $$ = NULL; } // Nao vai pra arvore
    | TK_PR_STRING { $$ = NULL; } // Nao vai pra arvore
;

declaracao-funcao: tipo ':' TK_IDENTIFICADOR '(' parametros-funcao-empty ')' lista-var-local comando-funcao {
/*                                                        Data data;
                                                        data.nodeType = IKS_AST_FUNCAO;
                                                        data.symEntry = $3->data.symEntry; //TODO Check??
                                                        
                                                        printf("Criando nó funcao...");
                                                        stubFunctionParser($3);*/
                                                        // assert($3->data.symEntry->key != NULL);
														// assert(data.symEntry->key != NULL); 

                                                        comp_tree_t *father = $3;
                                                        father->data.nodeType = IKS_AST_FUNCAO;
                                                        treeInsert($8, father);
                                                        
                                                        $$ = father; }
;

lista-var-local: declaracao-var-simples ';' lista-var-local { $$ = NULL; } // Nao entra na arvore
    | /* empty */
;

declaracao-varglobal: declaracao-var-simples { $$ = NULL; } // Nao entra na arvore
    | declaracao-vetor { $$ = NULL; } // Nao entra na arvore
;

declaracao-var-simples: tipo ':' TK_IDENTIFICADOR { $$ = NULL; } // Nao entra na arvore
;
declaracao-vetor: tipo ':' vetor { $$ = NULL; } // Nao entra na arvore
;

variavel: TK_IDENTIFICADOR
		| vetor
;

vetor: TK_IDENTIFICADOR '[' expr ']' {
                            Data data;
                            data.nodeType = IKS_AST_VETOR_INDEXADO;
                            data.symEntry = NULL;
                            
                            comp_tree_t *father = treeCreate(data);
                            treeInsert($1, father);
                            treeInsert($3, father);
                            
                            $$ = father; }
;
comando: comando-composto 
    | comando-simples
;
comando-composto: '{' comando-sequencia '}' 
{
	Data data;
        data.nodeType = IKS_AST_BLOCO;
        data.symEntry = NULL;
        comp_tree_t *father = treeCreate(data);
	treeInsert($2, father);

	$$ = father;
}
;
comando-funcao: '{' comando-sequencia '}'{$$ = $2;}
;

comando-sequencia: comando-simples { $$ = $1; }
    | comando-simples ';' comando-sequencia { 
                                treeInsert($3, $1);
                                $$ = $1; } //TODO verificar
    | comando-composto { $$ = $1; }
    | comando-composto ';' comando-sequencia {
                                treeInsert($3, $1);
                                $$ = $1; } //TODO verificar
    |  /* empty */ { $$ = NULL; } //comando-vazio { $$ = $1;} 
; 
comando-simples: condicional { $$ = $1; }
    | laco { $$ = $1; }
    | comando-entrada { $$ = $1; }
    | comando-saida { $$ = $1; }
    | comando-retorno { $$ = $1; }
    | atribuicao { $$ = $1; }
    | declaracao-var-simples { $$ = NULL; } // Nao entra na arvore
    | chamada-funcao { $$ = $1; }
                
    | ';' { $$ = NULL; } // Nao entra na arvore 
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
                    treeInsert($8, father);
                    
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
comando-retorno: TK_PR_RETURN expr {
                        Data data;
                        data.nodeType = IKS_AST_RETURN;
                        data.symEntry = NULL;
                        
                        comp_tree_t *father = treeCreate(data);
                        treeInsert($2, father);
                        
                        $$ = father; }
;
comando-entrada: TK_PR_INPUT variavel {
                        Data data;
                        data.nodeType = IKS_AST_INPUT;
                        data.symEntry = NULL;
                        
                        comp_tree_t *father = treeCreate(data);
                        treeInsert($2, father);
                        
                        $$ = father; }
;
comando-saida: TK_PR_OUTPUT argumento-saida {
                        Data data;
                        data.nodeType = IKS_AST_OUTPUT;
                        data.symEntry = NULL;
                        
                        comp_tree_t *father = treeCreate(data);
                        treeInsert($2, father);
                        
                        $$ = father; }
;
argumento-saida: expr { $$ = $1; }
    | expr ',' argumento-saida {
                    treeInsert($3, $1);
                    $$ = $1; }
;

atribuicao: atribuicao-simples { $$ = $1; }
    | atribuicao-vetor { $$ = $1; }
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
    
     | '!' termo { // Adicionado na etapa 3 - Gera conflito shift-reduce
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_NEGACAO;
        data.symEntry = NULL;
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($2, father);
        
        $$ = father; }

	/*| teste '-' termo { 
        Data data;
        data.nodeType = IKS_AST_ARIM_INVERSAO;
        data.symEntry = NULL;
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($2, father);
        
        $$ = father; }    */

	| termo { $$ = $1; }

	
;

chamada-funcao: TK_IDENTIFICADOR '(' parametros-chamada-funcao ')' {
				Data data;
				data.nodeType = IKS_AST_CHAMADA_DE_FUNCAO;
				data.symEntry = NULL;
				comp_tree_t *father = treeCreate(data);
				
				treeInsert($1, father);
				treeInsert($3, father);
				$$ = father; }
;

parametros-funcao-empty : parametros-declaracao-funcao
    | /* empty */
;

parametros-declaracao-funcao: tipo ':' TK_IDENTIFICADOR
    | tipo ':' TK_IDENTIFICADOR ',' parametros-declaracao-funcao
;
parametros-chamada-funcao: termo { $$ = $1; }
    | termo ',' parametros-chamada-funcao {
                                   treeInsert($3, $1); // TODO verificar
                                   $$ = $1; }                                            
    | /* empty */ { $$ = NULL; }
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
