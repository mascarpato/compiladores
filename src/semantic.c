#include "semantic.h"

/*int check_declar (comp_tree_t *root) 
{
    int i = 0;
    
	if (root == NULL) return IKS_SUCCESS ;
	else
	{
		check_declar(root->left);
		check_declar(root->right);		
	}
   
    if (root->data.nodeType == IKS_AST_ATRIBUICAO || 
	//root->data.nodeType == IKS_AST_IDENTIFICADOR || 
	root->data.nodeType == IKS_AST_VETOR_INDEXADO ||  
	root->data.nodeType == IKS_AST_CHAMADA_DE_FUNCAO ||
	root->data.nodeType == IKS_AST_FUNCAO) 
	{
		if(root->data.symEntry->key == 0) 
		{
			printf("Line %d: Declaration is missing the identifier name.\n", root->data.symEntry->occLine);
			return IKS_ERROR_UNDECLARED;
		} 
		else 
		{
			//teste se o símbolo já não foi definido
			if (root->data.symEntry->symbol.symType != SYMTYPE_IDENTIFIER) // != NULL
			{
				printf("Line %d: Symbol %s already defined.\n", root->data.symEntry->occLine, root->data.symEntry->symbol.name);
				return IKS_ERROR_DECLARED;
			}
			if (root->data.nodeType == IKS_AST_ATRIBUICAO) root->data.symEntry->symbol.symType = SYMTYPE_IDENTIFIER_DECLARED;				
			if (root->data.nodeType == IKS_AST_VETOR_INDEXADO) root->data.symEntry->symbol.symType = SYMTYPE_IDENTIFIER_DECLARED;
			//if (root->data.nodeType == IKS_AST_FUNCAO) root->data.symEntry->symbol.symType = SYMTYPE_IDENTIFIER_DECLARED;
			//if (root->data.nodeType == IKS_AST_CHAMADA_DE_FUNCAO) root->data.symEntry->symbol.symType = SYMTYPE_IDENTIFIER_DECLARED;	
			else
				return 999;//return IKS_SUCCESS ;
		}
		
	}
	
} */  

int check_id_declr (DictItem *sym) {
	if ((sym->symbol.symType & MASK_SYMTYPE_TYPE) == SYMTYPE_INT ||
		(sym->symbol.symType & MASK_SYMTYPE_TYPE) == SYMTYPE_FLOAT ||
		(sym->symbol.symType & MASK_SYMTYPE_TYPE) == SYMTYPE_CHAR ||
		(sym->symbol.symType & MASK_SYMTYPE_TYPE) == SYMTYPE_STRING ||
		(sym->symbol.symType & MASK_SYMTYPE_TYPE) == SYMTYPE_BOOL)
		return 1; // Declared = true.
	else
		return 0; // Declared = false
}

int check_id_isfunction (DictItem *sym) {
	if ((sym->symbol.symType & SYMTYPE_FUN) == SYMTYPE_FUN)
		return 1;
	else
		return 0;
}

int check_id_isvariable (DictItem *sym) {
	if ((sym->symbol.symType & SYMTYPE_VAR) == SYMTYPE_VAR)
		return 1;
	else
		return 0;
}

int check_id_isvector (DictItem *sym) {
	if ((sym->symbol.symType & SYMTYPE_VEC) == SYMTYPE_VEC)
		return 1;
	else
		return 0;
}


int check_id_isstring (DictItem *sym) {
	if ((sym->symbol.symType & MASK_SYMTYPE_TYPE) == SYMTYPE_STRING)
		return 1;
	else
		return 0;
}

int check_paramlist (ListNode *paramsDecl, ListNode *paramsCall) {
	int err = 0;

	while(paramsDecl != NULL && paramsCall != NULL && !err) {
	  if(paramsDecl->data != paramsCall->data){
	    err = IKS_ERROR_WRONG_TYPE_ARGS;
	  } else {
	    paramsDecl = paramsDecl->next;
	    paramsCall = paramsCall->next;
	  }
	}
	
	if(!err)
	  if(paramsDecl != NULL && paramsCall == NULL)
	    err = IKS_ERROR_MISSING_ARGS;
	  else if(paramsDecl == NULL && paramsCall != NULL)
	    err = IKS_ERROR_EXCESS_ARGS;

	return err;
}

void check_is_id_var(DictItem *var)
{
	// Tests if variable has been defined.
	if (!check_id_declr(var)) {
		sserror(IKS_ERROR_UNDECLARED, var);
		exit(IKS_ERROR_UNDECLARED);
	}
	
	if (!check_id_isvariable(var)) {
		sserror(IKS_ERROR_VARIABLE, var);
		exit(IKS_ERROR_VARIABLE);
	}
}

void check_is_id_fun(DictItem *fun)
{
	// Tests if variable has been defined.
	if (!check_id_declr(fun)) {
		sserror(IKS_ERROR_UNDECLARED, fun);
		exit(IKS_ERROR_UNDECLARED);
	}
	// Tests if was defined as function
	if (!check_id_isfunction(fun)) {
		sserror(IKS_ERROR_FUNCTION, fun);
		exit(IKS_ERROR_FUNCTION);
	}
}

void check_is_valid_indexed_vector(DictItem *vector, comp_tree_t *int_expr)
{
	// Tests if variable has been defined.
	if (!check_id_declr(vector)) {
		sserror(IKS_ERROR_UNDECLARED, vector);
		exit(IKS_ERROR_UNDECLARED);
	}
	if (!check_id_isvector(vector)) {
		sserror(IKS_ERROR_FUNCTION, vector);
		exit(IKS_ERROR_FUNCTION);
	}
	// Checks if expression is an integer (char)
	if (int_expr->data.semanticType == SYMTYPE_CHAR) {
		sserror(IKS_ERROR_CHAR_TO_X, NULL);
		exit(IKS_ERROR_CHAR_TO_X);
	}
	// Checks if expression is an integer (string)
	if (int_expr->data.semanticType == SYMTYPE_STRING) {
		sserror(IKS_ERROR_STRING_TO_X, NULL);
		exit(IKS_ERROR_STRING_TO_X);
	}
	// Checks if expression is an integer (string)
	if (int_expr->data.semanticType != SYMTYPE_INT) {
		sserror(IKS_ERROR_WRONG_TYPE, NULL); //TODO futuro: incluir cod de erro para indice nao inteiro.
		exit(IKS_ERROR_WRONG_TYPE);
	}	
}

/** Detecção de erros com string ou char */
void check_coercaoimpossivel_char_string(comp_tree_t *expr1, comp_tree_t *expr2)
{
	if (eval_infer(expr1->data.semanticType, expr2->data.semanticType,&(expr1->data.semanticType), &(expr2->data.semanticType)) == IKS_ERROR_STRING_TO_X) {
		sserror(IKS_ERROR_STRING_TO_X, NULL);
		exit(IKS_ERROR_STRING_TO_X);
	}
	else if (eval_infer(expr1->data.semanticType, expr2->data.semanticType,&(expr1->data.semanticType), &(expr2->data.semanticType)) == IKS_ERROR_CHAR_TO_X) {
		sserror(IKS_ERROR_CHAR_TO_X, NULL);
		exit(IKS_ERROR_CHAR_TO_X);
	}
}
void check_is_valid_input(comp_tree_t *expr)
{
	// Tests if expr is something valid (i.e. is declared in symbol table)
	if (expr->data.symEntry == NULL) {
		sserror(IKS_ERROR_WRONG_PAR_INPUT, NULL);
		exit(IKS_ERROR_WRONG_PAR_INPUT);
	}
	// Tests if identifier is a variable or vector
	if (!check_id_isvariable(expr->data.symEntry)) {
		sserror(IKS_ERROR_WRONG_PAR_INPUT, expr->data.symEntry);
		exit(IKS_ERROR_WRONG_PAR_INPUT);
	}
	if (!check_id_isvector(expr->data.symEntry)) {
		sserror(IKS_ERROR_WRONG_PAR_INPUT, expr->data.symEntry);
		exit(IKS_ERROR_WRONG_PAR_INPUT);
	}			
	// Tests if variable has been defined (typed)
	if (!check_id_declr(expr->data.symEntry)) {
		sserror(IKS_ERROR_UNDECLARED, expr->data.symEntry);
		exit(IKS_ERROR_UNDECLARED);
	}
}
void check_is_valid_output(comp_tree_t *expr)
{
	if (expr->data.semanticType == SYMTYPE_CHAR || expr->data.semanticType == SYMTYPE_BOOL) {
		sserror(IKS_ERROR_WRONG_PAR_OUTPUT, NULL);
		exit(IKS_ERROR_WRONG_PAR_OUTPUT);
	}
}