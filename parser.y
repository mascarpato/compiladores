%{
#include <stdio.h>
#include "iks_ast.h"
#include "comp_tree.h"
#include "semantic.h"
#include "comp_dict.h"
#include "comp_list.h"
#include "tac.h"

#define YYERROR_VERBOSE

#define SYM_TABLE_INITSIZE 500

struct treeNode_t *ast = NULL;
extern int sserror(int errCode, DictItem *symEntry);

/** Marks the function being currently parsed. Points to NULL if parser is
*   not currently working inside a function. */
struct comp_dict_item_t *currentFunction;

	/* Acao semantica na declaracao de funcao */
void declara_funcao(int tipo, DictItem *identifer);
	/* Acao semantica na declaracao de vetor */
void declara_vetor(int tipo, DictItem *identifier);

void declara_varglobal(int tipo, DictItem *identifier);

	/** Usado para alocar espaco para as variaveis declaradas globalmente */
int globalVarAddrCnt = 0;
	/** Usado para alocar espaco para as variaveis declaradas localmente */
int localVarAddrCnt = 0;
	/* Marca uso do escopo local ou global durante execucao do parser. */
short usingLocalScope = 0;
	
%}

%union {
	struct treeNode_t *tree;
	struct comp_dict_item_t *symEntry;
	int tipo;
	struct comp_list_t *param;
}

%initial-action {
    dict_create(SYM_TABLE_INITSIZE);
    Data data;
    data.nodeType = IKS_AST_PROGRAMA;
    data.symEntry = NULL;
    ast = treeCreate(data);
    currentFunction = NULL;
}

/* Declaracao dos tokens da gramatica da Linguagem IKS */

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
%type <tree> lista-dimensoes /*instavel*/
%type <tree> lista-dimensoes-atr /*instavel*/
%type <tipo> tipo
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
%type <param> parametros-funcao-empty
%type <param> parametros-declaracao-funcao
%type <tree> parametros-chamada-funcao
%type <tree> termo
%type <tree> expr

%%
/* Regras (e acoes) da gramatica da Linguagem K */
programa: s // { generateCode(ast); }
;

s: s declaracao-varglobal ';'  { $$ = NULL; }
    | s declaracao-funcao {
                                        if (ast->left == NULL) {
                                                //printf("Inserindo %s na raiz!\n",
                                                $2->data.symEntry->key;
                                                treeInsert($2, ast);
                                        } else {
                                                //printf("Inserindo %s na arvore ja com raiz!\
                                                $2->data.symEntry->key;
                                                treeInsert($2, $1);
                                        }
                    $$ = $2;
                    }
    | { $$ = NULL; }
;

tipo: TK_PR_INT { $$ = SYMTYPE_INT;}
    | TK_PR_FLOAT { $$ = SYMTYPE_FLOAT;}
    | TK_PR_BOOL { $$ = SYMTYPE_BOOL;}
    | TK_PR_CHAR { $$ = SYMTYPE_CHAR;}
    | TK_PR_STRING { $$ = SYMTYPE_STRING;}
;

declaracao-funcao: tipo ':' TK_IDENTIFICADOR { usingLocalScope=1; ($1, $3); dict_create(SYM_TABLE_INITSIZE); }   
                        '(' parametros-funcao-empty ')' { $3->symbol.params = $6; }
                        lista-var-local comando-funcao { currentFunction = NULL; dict_pop(); $$ = ast_criano_funcao($3, $10); usingLocalScope=0; }
;

lista-var-local: declaracao-var-simples ';' lista-var-local
    | { }
;

declaracao-varglobal: declaracao-var-simples
    | declaracao-vetor
;

declaracao-var-simples: tipo ':' TK_IDENTIFICADOR { declara_varglobal($1, $3); }
;

//modificacoes instaveis

declaracao-vetor: tipo ':' TK_IDENTIFICADOR '[' expr ']' lista-dimensoes { declara_vetor($1, $3); }
;

lista-dimensoes: '[' expr ']' lista-dimensoes {}
		 | {}
;

//fim modificacoes instaveis

comando: comando-composto 
    | comando-simples
;
comando-composto: '{' comando-sequencia '}' {
	
	//printf("Leu bloco de comando(composto)!\n");

	$$ = ast_criano_bloco($2);
}
;
comando-funcao: '{' comando-sequencia '}'{ $$ = $2;}
;

comando-sequencia: comando-simples { $$ = $1; }
    | comando-simples ';' comando-sequencia { treeInsert($3, $1); $$ = $1; } //TODO verificar
    | comando-composto { $$ = $1; }
    | comando-composto ';' comando-sequencia { treeInsert($3, $1); $$ = $1; } //TODO verificar    
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

condicional: TK_PR_IF '(' expr ')' TK_PR_THEN comando { $$ = ast_criano_ifthenelse($3, $6, ast_criano_bloco(NULL)); }
            | TK_PR_IF '(' expr ')' TK_PR_THEN comando TK_PR_ELSE comando { $$ = ast_criano_ifthenelse($3, $6, $8); }
;
laco: do-while
    | while
;

    // c-style do while
do-while: TK_PR_DO comando TK_PR_WHILE '(' expr ')' { $$ = ast_criano_dowhile($2, $5); } 
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
						// Checks if expr type matches the type of the current function
						if ($2->data.semanticType == 0) {
							printf("Compiler internal error. should not ocurr.\n");
							exit(-1);
						}
						
						// Embelezar o trecho a seguir antes de lancar o produto, pfv
						if (eval_atrib(currentFunction->symbol.symType & MASK_SYMTYPE_TYPE, $2->data.semanticType, NULL) == IKS_ERROR_CHAR_TO_X ||
							eval_atrib(currentFunction->symbol.symType & MASK_SYMTYPE_TYPE, $2->data.semanticType, NULL) == IKS_ERROR_STRING_TO_X ||
							eval_atrib(currentFunction->symbol.symType & MASK_SYMTYPE_TYPE, $2->data.semanticType, NULL) == SYMTYPE_UNDEF) {
							sserror(IKS_ERROR_WRONG_PAR_RETURN, NULL);
							return(IKS_ERROR_WRONG_PAR_RETURN);
						}
						
                        $$ = ast_criano_return($2); }
;

comando-entrada: TK_PR_INPUT expr { check_is_valid_input($2); $$ = ast_criano_entrada($2); }
;

comando-saida: TK_PR_OUTPUT argumento-saida { $$ = ast_criano_saida($2); }
;

argumento-saida: expr { check_is_valid_output($1); $$ = $1; }
				| expr ',' argumento-saida { check_is_valid_output($1); treeInsert($3, $1); $$ = $1; }
;

atribuicao: atribuicao-simples { $$ = $1; }
    | atribuicao-vetor { $$ = $1; }
;
atribuicao-simples: TK_IDENTIFICADOR '=' expr {
		// Tests if variable has been defined.
		if (!check_id_declr($1)) {
			sserror(IKS_ERROR_UNDECLARED, $1);
			return(IKS_ERROR_UNDECLARED);
		}
		// Checks if identifier is a vector
		if (check_id_isvector($1)) {
			sserror(IKS_ERROR_VECTOR, $1);
			return(IKS_ERROR_VECTOR);
		}
		
		// Checks if identifier is a function
		if (check_id_isfunction($1)) {
			sserror(IKS_ERROR_FUNCTION, $1);
			return(IKS_ERROR_FUNCTION);
		}

		//operações de coersão
		int aux_coersion;
		//printf("tipo 1: %d tipo2: %d", $1->symbol.symType & MASK_SYMTYPE_TYPE, $3->data.semanticType);
		
					// Aqui eh um dict_item
		aux_coersion = eval_atrib($1->symbol.symType & MASK_SYMTYPE_TYPE, $3->data.semanticType, &($3->data.semanticType));
		//printf("tipo esquerda: %d, tipo direita: %d", $1->symbol.symType & MASK_SYMTYPE_TYPE, $3->data.semanticType);
		//Checks if atrib is valid and sets right coersion
		if (aux_coersion == SYMTYPE_UNDEF){
			sserror(IKS_ERROR_WRONG_TYPE, NULL);
			return (IKS_ERROR_WRONG_TYPE);
		}else if (aux_coersion == IKS_ERROR_STRING_TO_X){
			sserror(IKS_ERROR_STRING_TO_X, NULL);
			return (IKS_ERROR_STRING_TO_X);
		}else if (aux_coersion == IKS_ERROR_CHAR_TO_X){
			sserror(IKS_ERROR_CHAR_TO_X, NULL);
			return (IKS_ERROR_CHAR_TO_X);
		}

		$3->data.coersionType = aux_coersion; //define tipo da coersão
		
		$$ = ast_criano_atrib($1, $3); }
;

atribuicao-vetor: TK_IDENTIFICADOR '[' expr ']' lista-dimensoes-atr '=' expr { //TODO: adicionar suporte para atrib em vetor multidimens
		// Tests if variable has been defined.
		if (!check_id_declr($1)) {
			sserror(IKS_ERROR_UNDECLARED, $1);
			return(IKS_ERROR_UNDECLARED);
		}
		// Checks if identifier is a vector
		if (!check_id_isvector($1)) {
			sserror(IKS_ERROR_VARIABLE, $1);
			return(IKS_ERROR_VARIABLE);
		}
// TESTE MULTIDIM
		
		// Checks if expression is an integer (char)
		if ($3->data.semanticType == SYMTYPE_CHAR) {
			sserror(IKS_ERROR_CHAR_TO_X, NULL);
			return(IKS_ERROR_CHAR_TO_X);
		}
		// Checks if expression is an integer (string)
		if ($3->data.semanticType == SYMTYPE_STRING) {
			sserror(IKS_ERROR_STRING_TO_X, NULL);
			return(IKS_ERROR_STRING_TO_X);
		}
		// Checks if expression is an integer (string)
		if ($3->data.semanticType != SYMTYPE_INT) {
			sserror(IKS_ERROR_WRONG_TYPE, NULL); //TODO futuro: incluir cod de erro para indice nao inteiro.
			return(IKS_ERROR_WRONG_TYPE);
		}
//FIM_TESTE_MULTIDIM
	//operações de coersão	
	int aux_coersion;
					// aqui eh dict
					// $1->symbol.symType & MASK_SYMTYPE_TYPE
	aux_coersion = eval_atrib($1->symbol.symType & MASK_SYMTYPE_TYPE, $7->data.semanticType, &($7->data.semanticType)); // ultimo expr
	//Checks if atrib is valid and sets right coersion
	if (aux_coersion == SYMTYPE_UNDEF){
		sserror(IKS_ERROR_WRONG_TYPE, NULL);
		return (IKS_ERROR_WRONG_TYPE);
	}else if (aux_coersion == IKS_ERROR_STRING_TO_X){
		sserror(IKS_ERROR_STRING_TO_X, NULL);
		return (IKS_ERROR_STRING_TO_X);
	}else if (aux_coersion == IKS_ERROR_CHAR_TO_X){
		sserror(IKS_ERROR_CHAR_TO_X, NULL);
		return (IKS_ERROR_CHAR_TO_X);
	}

	$7->data.coersionType = aux_coersion; //define tipo da coersão 

        Data data1;
        data1.nodeType = IKS_AST_ATRIBUICAO;
        data1.symEntry = NULL;
		data1.semanticType = $1->symbol.symType & MASK_SYMTYPE_TYPE;
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
		treeInsert($3, vader);// TESTE MULTIDIM
		/* -- -- */
		
		/* -- Inserts vector and expr nodes into attribution node */ 
		treeInsert(vader, attributionNode);
		treeInsert($7, attributionNode);  //ultimo expr
		
        $$ = attributionNode; }
;

////////teste
lista-dimensoes-atr: '[' expr ']' lista-dimensoes-atr {
		if ($2->data.semanticType == SYMTYPE_CHAR) {
			sserror(IKS_ERROR_CHAR_TO_X, NULL);
			return(IKS_ERROR_CHAR_TO_X);
		}
		// Checks if expression is an integer (string)
		if ($2->data.semanticType == SYMTYPE_STRING) {
			sserror(IKS_ERROR_STRING_TO_X, NULL);
			return(IKS_ERROR_STRING_TO_X);
		}
		// Checks if expression is an integer (string)
		if ($2->data.semanticType != SYMTYPE_INT) {
			sserror(IKS_ERROR_WRONG_TYPE, NULL); //TODO futuro: incluir cod de erro para indice nao inteiro.
			return(IKS_ERROR_WRONG_TYPE);
		}

		Data data;
		comp_tree_t *dimNode = treeCreate(data);
		
		treeInsert($2, dimNode);
		$$=dimNode;
}	
	| /*empty*/ {}
;

////////fim_de_teste

expr: '(' expr ')' { $$ = $2; }
	| termo { $$ = $1; } 
    | expr TK_OC_LE expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_cmpLE($1, $3); }
    | expr TK_OC_GE expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_cmpGE($1, $3); }
    | expr TK_OC_EQ expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_cmpEQ($1, $3); }
    | expr TK_OC_NE expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_cmpNE($1, $3); }
    | expr '<' expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_cmpL($1, $3); }
    | expr '>' expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_cmpG($1, $3); }
	| expr TK_OC_AND expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_and($1, $3); }
	| expr TK_OC_OR expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_or($1,$3); }
	| expr '+' expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_soma($1, $3); }
	| expr '-' expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_sub($1, $3); }
	| expr '*' expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_mult($1, $3); }
	| expr '/' expr { check_coercaoimpossivel_char_string($1, $3); $$ = ast_criano_div($1, $3); }
	| '!' termo { $$ = ast_criano_neg($2); }
	| '-' expr { $$ = ast_criano_inv($2);}    
;

chamada-funcao: TK_IDENTIFICADOR '(' parametros-chamada-funcao ')' {
				check_is_id_fun($1);
				
				if($3 != NULL){
					int err;
					if(err = check_paramlist($1->symbol.params, $3->data.symEntry->symbol.params)){
						sserror(err, $1);
						return(err);
					}
					// Libera DictItem criado temporariamente
					list_terminate($3->data.symEntry->symbol.params);
					free($3->data.symEntry); 
				}
				else if($1->symbol.params != NULL){
					printf("\noi\n");
					sserror(IKS_ERROR_MISSING_ARGS, $1);
					return(IKS_ERROR_MISSING_ARGS);
				}
				
				$$ = ast_criano_chamadafuncao($1, $3); }
;
parametros-funcao-empty : parametros-declaracao-funcao { $$ = $1; }
    | { $$ = NULL; }
;

parametros-declaracao-funcao: tipo ':' TK_IDENTIFICADOR { 
							ListNode* param = (ListNode*)malloc(sizeof(ListNode));
							param->data = $1;
							param->next = NULL;
							$$ = param; }
    | tipo ':' TK_IDENTIFICADOR ',' parametros-declaracao-funcao { 
							ListNode* param = (ListNode*)malloc(sizeof(ListNode));
							param->data = $1;
							param->next = $5;
							$$ = param; }
;

parametros-chamada-funcao: expr { DictItem *tempDictItem = (DictItem*) malloc(sizeof(DictItem)); //DictItem criado temporariamente para passar os parametros, será liberado na sequencia
					$1->data.symEntry = tempDictItem;
					ListNode* param = (ListNode*)malloc(sizeof(ListNode));
					param->data = $1->data.semanticType & MASK_SYMTYPE_TYPE;
					param->next = NULL;
					$1->data.symEntry->symbol.params = param;
				  
					$$ = $1; }
    | expr ',' parametros-chamada-funcao { 
					DictItem *tempDictItem = (DictItem*)malloc(sizeof(DictItem)); //DictItem criado temporariamente para passar os parametros, será liberado na sequencia
					$1->data.symEntry = tempDictItem;
					ListNode* param = (ListNode*)malloc(sizeof(ListNode));
					param->data = $1->data.semanticType & MASK_SYMTYPE_TYPE;
					param->next = $3->data.symEntry->symbol.params;
					$1->data.symEntry->symbol.params = param;
					   
					free($3->data.symEntry); // Libera tempDictItem criado pelo seu filho
					   
					treeInsert($3, $1); // TODO verificar
					$$ = $1; }
    | { $$ = NULL; }
;

termo: TK_IDENTIFICADOR { check_is_id_var($1); $$ = ast_criano_identificador($1); }
    | TK_IDENTIFICADOR '[' expr ']' { check_is_valid_indexed_vector($1, $3); $$ = ast_criano_vetor($1, $3); }
    | chamada-funcao 
    | TK_LIT_INT { $$ = ast_criano_literal($1, SYMTYPE_INT); }
    | TK_LIT_FLOAT { $$ = ast_criano_literal($1, SYMTYPE_FLOAT); }
    | TK_LIT_FALSE { $$ = ast_criano_literal($1, SYMTYPE_BOOL); }
    | TK_LIT_TRUE { $$ = ast_criano_literal($1, SYMTYPE_BOOL); }
    | TK_LIT_CHAR { $$ = ast_criano_literal($1, SYMTYPE_CHAR); }
    | TK_LIT_STRING { $$ = ast_criano_literal($1, SYMTYPE_STRING); }
%%

void declara_funcao(int tipo, DictItem *identifier)
{
	// Tests if function is being redefined .
	if (check_id_declr(identifier)) {
		sserror(IKS_ERROR_DECLARED, identifier);
		exit(IKS_ERROR_DECLARED);
	}
	currentFunction = identifier;
	identifier->symbol.symType = tipo | SYMTYPE_FUN;
	
	dict_create(SYM_TABLE_INITSIZE);
}

void declara_vetor(int tipo, DictItem *identifier)
{
	// Tests if function is being redefined .
	if (check_id_declr(identifier)) {
		sserror(IKS_ERROR_DECLARED, identifier);
		exit(IKS_ERROR_DECLARED);
	}
	
	identifier->symbol.symType = tipo | SYMTYPE_VEC;
}

void declara_varglobal(int tipo, DictItem *identifier)
{
	// Tests if function is being redefined .
	if (check_id_declr(identifier)) {
		sserror(IKS_ERROR_DECLARED, identifier);
		exit(IKS_ERROR_DECLARED);
	}

	identifier->symbol.symType = tipo | SYMTYPE_VAR;
	
	int *varAddr;
	if (usingLocalScope) {
		varAddr = &localVarAddrCnt;
		identifier->symbol.symType = identifier->symbol.symType | SYM_IS_LOCALSCOPE;
	}
	else {
		varAddr = &globalVarAddrCnt;
	}
	
	// Evaluates var address and increments, to hold current variable being defined.
	switch(tipo) {
		case SYMTYPE_INT:
			identifier->symbol.symAddr = *varAddr; *varAddr += sizeof(iks_int); break;
		case SYMTYPE_FLOAT:
			identifier->symbol.symAddr = *varAddr; *varAddr += sizeof(iks_float); break;
		case SYMTYPE_CHAR:
			identifier->symbol.symAddr = *varAddr; *varAddr += sizeof(iks_char); break;
		case SYMTYPE_STRING:
			identifier->symbol.symAddr = *varAddr; *varAddr += sizeof(iks_string); break;
		case SYMTYPE_BOOL:
			identifier->symbol.symAddr = *varAddr; *varAddr += sizeof(iks_boolean); break;
	}
}
