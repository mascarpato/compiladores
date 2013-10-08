%{
#include <stdio.h>
#include "iks_ast.h"
#include "comp_tree.h"
#include "semantic.h"

#define YYERROR_VERBOSE

struct treeNode_t *ast = NULL;
extern int sserror(int errCode, DictItem *symEntry);

/** Marks the function being currently parsed. Points to NULL if parser is
*   not currently working inside a function. */
struct comp_dict_item_t *currentFunction;

	/* Acao semantica na declaracao de funcao */
void declara_funcao(int tipo, DictItem *identifer);

%}

/* Declaracao dos tokens da gramática da Linguagem K */

%union {
	struct treeNode_t *tree;
	struct comp_dict_item_t *symEntry;
	int tipo;
	struct param_list_t *param;
}

%initial-action {
    Data data;
    data.nodeType = IKS_AST_PROGRAMA;
    data.symEntry = NULL;
    ast = treeCreate(data);
    currentFunction = NULL;
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
/* Regras (e acoes) da gramática da Linguagem K */
programa: s { }
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

tipo: TK_PR_INT { $$ = SYMTYPE_INT;}
    | TK_PR_FLOAT { $$ = SYMTYPE_FLOAT;}
    | TK_PR_BOOL { $$ = SYMTYPE_BOOL;}
    | TK_PR_CHAR { $$ = SYMTYPE_CHAR;}
    | TK_PR_STRING { $$ = SYMTYPE_STRING;}
;

declaracao-funcao: tipo ':' TK_IDENTIFICADOR { declara_funcao($1, $3); } '(' parametros-funcao-empty ')' lista-var-local comando-funcao {
						currentFunction = NULL;
						
						Data data;
						data.nodeType = IKS_AST_FUNCAO;
						// data.semanticType = $1; // Nao é utilizado ?
						data.symEntry = $3;
						data.symEntry->symbol.params = $6;
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

declaracao-var-simples: tipo ':' TK_IDENTIFICADOR { 
						//printf("Symbol type is %d\n", $3->symbol.symType);
			// Tests if function is being redefined .
			if (check_id_declr($3)) {
				sserror(IKS_ERROR_DECLARED, $3);
				return(IKS_ERROR_DECLARED);
			}

			$3->symbol.symType = $1 | SYMTYPE_VAR;
}
;

declaracao-vetor: tipo ':' TK_IDENTIFICADOR '[' expr ']' { 
			// Tests if function is being redefined .
			if (check_id_declr($3)) {
				sserror(IKS_ERROR_DECLARED, $3);
				return(IKS_ERROR_DECLARED);
			}
			
			$3->symbol.symType = $1 | SYMTYPE_VEC;
}
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
						// Checks if expr type matches the type of the current function
						if ($2->data.semanticType == 0) {
							printf("Compiler internal error. should not ocurr.\n");
							exit(-1);
						}
						printf("chegay.");
						if ($2->data.semanticType != (currentFunction->symbol.symType & MASK_SYMTYPE_TYPE)) {
							printf("Tipos sao %d e %d.\n", $2->data.semanticType, currentFunction->symbol.symType & MASK_SYMTYPE_TYPE);
							sserror(IKS_ERROR_WRONG_PAR_RETURN, NULL);
							return(IKS_ERROR_WRONG_PAR_RETURN);
							printf("cheguei aqui.");
						} else
							printf("Tipos sao %d e %d.\n", $2->data.semanticType, currentFunction->symbol.symType & MASK_SYMTYPE_TYPE);
						
                        Data data;
                        data.nodeType = IKS_AST_RETURN;
                        data.symEntry = NULL;
                        
                        comp_tree_t *father = treeCreate(data);
                        treeInsert($2, father);
                        
                        $$ = father; }
;
	// at� etapa3
	/*comando-entrada: TK_PR_INPUT TK_IDENTIFICADOR {
						// Tests if variable has been defined.
						if (!check_id_declr($2)) {
							sserror(IKS_ERROR_UNDECLARED, $2);
							return(IKS_ERROR_UNDECLARED);
						}
						// Tests if identifier is a variable
						// TODO Check: Using identifier = vector or function allowed?
// 						if (!check_id_isvariable($2))
// 							return(IKS_ERROR_WRONG_PAR_INPUT);

                        Data data;
                        data.nodeType = IKS_AST_INPUT;
                        data.symEntry = NULL;
                        // Nau utilizado data.semanticType
                        comp_tree_t *father = treeCreate(data);
                        
                        Data data2;
                        data2.nodeType = IKS_AST_IDENTIFICADOR;
                        data2.symEntry = $2;
                        comp_tree_t *son = treeCreate(data2);
                        
                        treeInsert(son, father);
                        
                        $$ = father; }
	;*/
comando-entrada: TK_PR_INPUT expr {
			// Tests if expr is something valie (i.e. is declared in symbol table)
			if ($2->data.symEntry == NULL) {
				sserror(IKS_ERROR_WRONG_PAR_INPUT, NULL);
				return(IKS_ERROR_WRONG_PAR_INPUT);
			}
			// Tests if identifier is a variable or vector
			if (!check_id_isvariable($2->data.symEntry)) {
				sserror(IKS_ERROR_WRONG_PAR_INPUT, $2->data.symEntry);
				return(IKS_ERROR_WRONG_PAR_INPUT);
			}
			if (!check_id_isvector($2->data.symEntry)) {
				sserror(IKS_ERROR_WRONG_PAR_INPUT, $2->data.symEntry);
				return(IKS_ERROR_WRONG_PAR_INPUT);
			}			
			// Tests if variable has been defined (typed)
			if (!check_id_declr($2->data.symEntry)) {
				sserror(IKS_ERROR_UNDECLARED, $2->data.symEntry);
				return(IKS_ERROR_UNDECLARED);
			}
			
			Data data;
			data.nodeType = IKS_AST_INPUT;
			data.symEntry = NULL;
			// Nau utilizado data.semanticType
			comp_tree_t *father = treeCreate(data);
			
			Data data2;
			data2.nodeType = IKS_AST_IDENTIFICADOR;
			data2.symEntry = $2->data.symEntry;
			comp_tree_t *son = treeCreate(data2);
			
			treeInsert(son, father);
			
			$$ = father;
}
;

comando-saida: TK_PR_OUTPUT argumento-saida {
                        Data data;
                        data.nodeType = IKS_AST_OUTPUT;
                        data.symEntry = NULL;
                        
                        comp_tree_t *father = treeCreate(data);
                        treeInsert($2, father);
                        
                        $$ = father; }
;
argumento-saida: expr { $$ = $1;
// 						// Tests if variable has been defined.
// 						if (!check_id_declr($1->data.symEntry))
// 							return(IKS_ERROR_UNDECLARED);
// 						// Tests if identifier is a variable
// 						if (!check_id_isvariable($1->data.symEntry))
// 							return(IKS_ERROR_WRONG_PAR_INPUT);
					}
				| expr ',' argumento-saida {
					/*	// Tests if variable has been defined.
						if (!check_id_declr($1->data.symEntry))
							return(IKS_ERROR_UNDECLARED);
						// Tests if identifier is a variable
						if (!check_id_isvariable($1->data.symEntry))
							return(IKS_ERROR_WRONG_PAR_INPUT);*/
							
							treeInsert($3, $1);
							$$ = $1; 
				}
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
        		
						
        Data data;
        data.nodeType = IKS_AST_ATRIBUICAO;
        data.symEntry = NULL;
        data.semanticType = $1->symbol.symType & MASK_SYMTYPE_TYPE;
        comp_tree_t *attributionNode = treeCreate(data);
        
		Data data2;
		data2.nodeType = IKS_AST_IDENTIFICADOR;
		data2.symEntry = $1;
        // Redundante ? data2.semanticType
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
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr TK_OC_GE expr {
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_GE;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr TK_OC_EQ expr {
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_IGUAL;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr TK_OC_NE expr {
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_DIF;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr '<' expr {
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_L;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr '>' expr {
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_G;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
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
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr TK_OC_OR expr {
        Data data;
        data.nodeType = IKS_AST_LOGICO_OU;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr '+' expr { 
        Data data;
        data.nodeType = IKS_AST_ARIM_SOMA;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr '-' expr {
        Data data;
        data.nodeType = IKS_AST_ARIM_SUBTRACAO;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father);
        
        $$ = father; }
    | expr '*' expr {
        Data data;
        data.nodeType = IKS_AST_ARIM_MULTIPLICACAO;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father); 
        
        $$ = father; }
    | expr '/' expr {
        Data data;
        data.nodeType = IKS_AST_ARIM_DIVISAO;
        data.symEntry = NULL;
        data.semanticType = eval_infer(
				$1->data.semanticType, $3->data.semanticType,
				&($1->data.semanticType), &($3->data.semanticType));
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($1, father);
        treeInsert($3, father); 
        
        $$ = father; }
    
     | '!' termo {
        Data data;
        data.nodeType = IKS_AST_LOGICO_COMP_NEGACAO;
        data.symEntry = NULL;
        data.semanticType = $2->data.semanticType;
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($2, father);
        
        $$ = father; }

	| '-' expr { 
        Data data;
        data.nodeType = IKS_AST_ARIM_INVERSAO;
        data.symEntry = NULL;
        data.semanticType = $2->data.semanticType;
        comp_tree_t *father = treeCreate(data);
        
        treeInsert($2, father);
        
        
        $$ = father; }    

	| termo { $$ = $1; }
;

chamada-funcao: TK_IDENTIFICADOR '(' parametros-chamada-funcao ')' {
				// Tests if variable has been defined.
				if (!check_id_declr($1)) {
					sserror(IKS_ERROR_UNDECLARED, $1);
					return(IKS_ERROR_UNDECLARED);
				}
				if (!check_id_isfunction($1)) {
					sserror(IKS_ERROR_FUNCTION, $1);
					return(IKS_ERROR_FUNCTION);
				}
				
				
				if($3 != NULL){
				  int err;
				  if(err = check_paramlist($1->symbol.params, $3->data.symEntry->symbol.params)){
				    sserror(err, $1);
				    return(err);
				  }
				}
				else if($1->symbol.params != NULL){
				  sserror(IKS_ERROR_MISSING_ARGS, $1);
				  return(IKS_ERROR_MISSING_ARGS);
				}
				  
							
				Data data;
				data.nodeType = IKS_AST_CHAMADA_DE_FUNCAO;
				data.symEntry = NULL;
				data.semanticType = $1->symbol.symType & MASK_SYMTYPE_TYPE;
				comp_tree_t *father = treeCreate(data);
				
				Data data2;
				data2.nodeType = IKS_AST_IDENTIFICADOR;
				data2.symEntry = $1;
				comp_tree_t *son = treeCreate(data2); 
				
				treeInsert(son, father);
				treeInsert($3, father);
				$$ = father; }
;
parametros-funcao-empty : parametros-declaracao-funcao { $$ = $1; }
    | { }
;

parametros-declaracao-funcao: tipo ':' TK_IDENTIFICADOR { ParamList* param = (ParamList*)malloc(sizeof(ParamList));
							  param->paramType = $1;
							  param->next = NULL;
							  $$ = param; }
    | tipo ':' TK_IDENTIFICADOR ',' parametros-declaracao-funcao { ParamList* param = (ParamList*)malloc(sizeof(ParamList));
								   param->paramType = $1;
								   param->next = $5;
								   $$ = param; }
;
parametros-chamada-funcao: expr { ParamList* param = (ParamList*)malloc(sizeof(ParamList));
				  param->paramType = $1->data.semanticType & MASK_SYMTYPE_TYPE;
				  param->next = NULL;
				  $1->data.symEntry->symbol.params = param;
				  
				  $$ = $1; }
    | expr ',' parametros-chamada-funcao { 
						ParamList* param = (ParamList*) malloc(sizeof(ParamList));
						param->paramType = $1->data.semanticType & MASK_SYMTYPE_TYPE;
						param->next = $3->data.symEntry->symbol.params;
						$1->data.symEntry->symbol.params = param;
					   
					   treeInsert($3, $1); // TODO verificar
					   $$ = $1; }
    | /* empty */ { $$ = NULL; }
;

termo: TK_IDENTIFICADOR {
		// Tests if variable has been defined.
		if (!check_id_declr($1)) {
			sserror(IKS_ERROR_UNDECLARED, $1);
			return(IKS_ERROR_UNDECLARED);
		}
		if (!check_id_isvariable($1)) {
			sserror(IKS_ERROR_VARIABLE, $1);
			return(IKS_ERROR_VARIABLE);
		}
			
		Data data2;
		data2.nodeType = IKS_AST_IDENTIFICADOR;
		data2.symEntry = $1;
		data2.semanticType = $1->symbol.symType;
		comp_tree_t *daddy = treeCreate(data2); 
		
		$$ = daddy;
	}
    | TK_IDENTIFICADOR '[' expr ']' {
		// Tests if variable has been defined.
		if (!check_id_declr($1)) {
			sserror(IKS_ERROR_UNDECLARED, $1);
			return(IKS_ERROR_UNDECLARED);
		}
		if (!check_id_isvector($1)) {
			sserror(IKS_ERROR_FUNCTION, $1);
			return(IKS_ERROR_FUNCTION);
		}
			
		/* -- Creates vector node --*/
		Data data;
		data.nodeType = IKS_AST_VETOR_INDEXADO;
		data.symEntry = $1;
		data.semanticType = $1->symbol.symType;
		comp_tree_t *vader = treeCreate(data);
		
		Data data2;
		data2.nodeType = IKS_AST_IDENTIFICADOR;
		data2.semanticType = $1->symbol.symType;
		data2.symEntry = $1;
		comp_tree_t *luke = treeCreate(data2);
		
		treeInsert(luke, vader);
		treeInsert($3, vader);
		
		$$ = vader;
    }
    | chamada-funcao 
    | TK_LIT_INT { 	Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.semanticType = SYMTYPE_INT;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_FLOAT { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.semanticType = SYMTYPE_FLOAT;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_FALSE { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.semanticType = SYMTYPE_BOOL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_TRUE { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.semanticType = SYMTYPE_BOOL;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_CHAR { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.semanticType = SYMTYPE_CHAR;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
    | TK_LIT_STRING { Data data;
					data.nodeType = IKS_AST_LITERAL;
					data.semanticType = SYMTYPE_STRING;
					data.symEntry = $1;
					
					$$ = treeCreate(data); }
%%

void declara_funcao(int tipo, DictItem *identifier) {
	// Tests if function is being redefined .
	if (check_id_declr(identifier)) {
		sserror(IKS_ERROR_DECLARED, identifier);
		exit(IKS_ERROR_DECLARED);
	}
	currentFunction = identifier;
	identifier->symbol.symType = tipo | SYMTYPE_FUN;
}