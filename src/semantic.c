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