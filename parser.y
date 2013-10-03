%{
#include <stdio.h>
#include "iks_ast.h"
#include "comp_tree.h"

#define YYERROR_VERBOSE

struct treeNode_t *ast = NULL;
%}

/* Declaração dos tokens da gramática da Linguagem K */

%union {
	struct treeNode_t *tree;
	struct comp_dict_item_t *symEntry;
}

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
%left TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE  '<' '>' '='
%left TK_OC_AND TK_OC_OR
%token <symEntry> TK_LIT_INT
%token <symEntry> TK_LIT_FLOAT
%token <symEntry> TK_LIT_FALSE
%token <symEntry> TK_LIT_TRUE
%token <symEntry> TK_LIT_CHAR
%token <symEntry> TK_LIT_STRING
%token <symEntry> TK_IDENTIFICADOR
%token TOKEN_ERRO
%left '-' '+' 
%left '*' '/'

/* Nao terminais */ 
%type <tree> programa
%type <tree> s
%type <tree> declaracao-funcao
%type <tree> lista-var-local
%type <tree> declaracao-varglobal
%type <tree> declaracao-var-simples
%type <tree> declaracao-vetor
%type <tree> tipo
%type <tree> comando
%type <tree> comando-composto
%type <tree> comando-funcao
%type <tree> comando-sequencia
%type <tree> comando-simples
%type <tree> condicional
%type <tree> laco
%type <tree> do-while
%type <tree> while
%type <tree> comando-retorno
%type <tree> comando-entrada
%type <tree> comando-saida
%type <tree> argumento-saida
%type <tree> atribuicao
%type <tree> atribuicao-simples
%type <tree> atribuicao-vetor
%type <tree> chamada-funcao
%type <tree> parametros-funcao-empty
%type <tree> parametros-declaracao-funcao
%type <tree> parametros-chamada-funcao
%type <tree> termo
%type <tree> expr



%%
/* Regras (e ações) da gramática da Linguagem K */
programa: s {printf ("%d \n", check_declar ($1) );}
;

s: s declaracao-varglobal ';'  { $$ = NULL; }
    | s declaracao-funcao {
					if (ast->left == NULL) {
						//printf("Inserindo %s na raiz!\n",
						$2->data.symEntry->key;
						treeInsert($2, ast);
					} else {
						//printf("Inserindo %s na arvore ja com raiz!\n",
						$2->data.symEntry->key;
						treeInsert($2, $1);
					}
                    $$ = $2;
                    }
    | { $$ = NULL; }
;

tipo: TK_PR_INT { }
    | TK_PR_FLOAT { }
    | TK_PR_BOOL { }
    | TK_PR_CHAR { }
    | TK_PR_STRING { }
;

declaracao-funcao: tipo ':' TK_IDENTIFICADOR '(' parametros-funcao-empty ')' lista-var-local comando-funcao {
						Data data;
						data.nodeType = IKS_AST_FUNCAO;
						data.symEntry = $3;
						
						comp_tree_t *father = treeCreate(data);
						treeInsert($8, father);
						
						$$ = father; 
}
;

lista-var-local: declaracao-var-simples ';' lista-var-local
    | { }
;

declaracao-varglobal: declaracao-var-simples
    | declaracao-vetor
;

declaracao-var-simples: tipo ':' TK_IDENTIFICADOR

;
declaracao-vetor: tipo ':' TK_IDENTIFICADOR '[' expr ']'
;

comando: comando-composto 
    | comando-simples
;
comando-composto: '{' comando-sequencia '}' {
	Data data;
	data.nodeType = IKS_AST_BLOCO;
	data.symEntry = NULL;
	comp_tree_t *father = treeCreate(data);
	treeInsert($2, father);
	
	//printf("Leu bloco de comando(composto)!\n");

	$$ = father;
}
;
comando-funcao: '{' comando-sequencia '}'{ $$ = $2;}
;

comando-sequencia: comando-simples { $$ = $1; }
    | comando-simples ';' comando-sequencia { 
                                treeInsert($3, $1);
                                $$ = $1; } //TODO verificar
    | comando-composto { $$ = $1; }
    | comando-composto ';' comando-sequencia {
                                treeInsert($3, $1);
                                $$ = $1; } //TODO verificar
    | { $$ = NULL; }
; 
comando-simples: condicional { $$ = $1; }
    | laco { $$ = $1; }
    | comando-entrada { $$ = $1; }
    | comando-saida { $$ = $1; }
    | comando-retorno { $$ = $1; }
    | atribuicao { $$ = $1; }
    | declaracao-var-simples
    | chamada-funcao { $$ = $1; }
                
    | ';' { $$ = NULL; }
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
comando-entrada: TK_PR_INPUT TK_IDENTIFICADOR {
                        Data data;
                        data.nodeType = IKS_AST_INPUT;
                        data.symEntry = NULL;
                        comp_tree_t *father = treeCreate(data);
                        
                        Data data2;
                        data2.nodeType = IKS_AST_IDENTIFICADOR;
                        data2.symEntry = $2;
                        comp_tree_t *son = treeCreate(data2);
                        
                        treeInsert(son, father);
                        
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
        comp_tree_t *attributionNode = treeCreate(data);
        
		Data data2;
		data2.nodeType = IKS_AST_IDENTIFICADOR;
		data2.symEntry = $1;
		comp_tree_t *id = treeCreate(data2);
        
        treeInsert(id, attributionNode);
        treeInsert($3, attributionNode);
        
/*        printf("Attribution node @ %p: \n", attributionNode);
        printf("---- leftside.name = %s, rightside.nodeType = %s \n",
				attributionNode->left->data.symEntry->key,
				attributionNode->left->right->data.symEntry->key);*/
        
        $$ = attributionNode; }
;

atribuicao-vetor: TK_IDENTIFICADOR '[' expr ']' '=' expr {
        Data data1;
        data1.nodeType = IKS_AST_ATRIBUICAO;
        data1.symEntry = NULL;
        comp_tree_t *attributionNode = treeCreate(data1);
        
		/* -- Creates vector node --*/
		Data data2;
		data2.nodeType = IKS_AST_VETOR_INDEXADO;
		data2.symEntry = NULL;
		comp_tree_t *vader = treeCreate(data2);
		
		Data data3;
		data3.nodeType = IKS_AST_IDENTIFICADOR;
		data3.symEntry = $1;
		comp_tree_t *luke = treeCreate(data3);
		
		treeInsert(luke, vader);
		treeInsert($3, vader);
		/* -- -- */
		
		/* -- Inserts vector and expr nodes into attribution node */ 
		treeInsert(vader, attributionNode);
		treeInsert($6, attributionNode);
		
        $$ = attributionNode; }
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
    
     | '!' termo {
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_NEGACAO;
        data.symEntry = NULL;
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($2, father);
        
        $$ = father; }

	| '-' expr { 
        Data data;
        data.nodeType = IKS_AST_ARIM_INVERSAO;
        data.symEntry = NULL;
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($2, father);
        
        
        $$ = father; }    

	| termo { $$ = $1; }
;

chamada-funcao: TK_IDENTIFICADOR '(' parametros-chamada-funcao ')' {
				Data data;
				data.nodeType = IKS_AST_CHAMADA_DE_FUNCAO;
				data.symEntry = NULL;
				comp_tree_t *father = treeCreate(data);
				
				Data data2;
				data2.nodeType = IKS_AST_IDENTIFICADOR;
				data2.symEntry = $1;
				comp_tree_t *son = treeCreate(data2); 
				
				treeInsert(son, father);
				treeInsert($3, father);
				$$ = father; }
;
parametros-funcao-empty : parametros-declaracao-funcao
    | { }
;

parametros-declaracao-funcao: tipo ':' TK_IDENTIFICADOR
    | tipo ':' TK_IDENTIFICADOR ',' parametros-declaracao-funcao
;
parametros-chamada-funcao: expr { $$ = $1; }
    | expr ',' parametros-chamada-funcao {
                                   treeInsert($3, $1); // TODO verificar
                                   $$ = $1; }
    | /* empty */ { $$ = NULL; }
;

termo: TK_IDENTIFICADOR { 
		Data data2;
		data2.nodeType = IKS_AST_IDENTIFICADOR;
		data2.symEntry = $1;
		comp_tree_t *daddy = treeCreate(data2); 
		
		$$ = daddy;
	}
    | TK_IDENTIFICADOR '[' expr ']' {
		/* -- Creates vector node --*/
		Data data;
		data.nodeType = IKS_AST_VETOR_INDEXADO;
		data.symEntry = NULL;
		comp_tree_t *vader = treeCreate(data);
		
		Data data2;
		data2.nodeType = IKS_AST_IDENTIFICADOR;
		data2.symEntry = $1;
		comp_tree_t *luke = treeCreate(data2);
		
		treeInsert(luke, vader);
		treeInsert($3, vader);
		
		$$ = vader;
    }
    | chamada-funcao 
    | TK_LIT_INT { 	Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_FLOAT { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_FALSE { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_TRUE { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_CHAR { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_STRING { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
%%
