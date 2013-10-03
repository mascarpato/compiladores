#include "semantic.h"

int check_declar (comp_tree_t *root) 
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
	root->data.nodeType == IKS_AST_FUNCAO
	) 
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
	


}        
