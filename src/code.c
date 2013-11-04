#include "code.h"

TAC* geraCod_noLiteral(comp_tree_t *node)
{
	TAC *tac = malloc(sizeof(TAC));
	
	tac->type = TAC_LOADI;
	tac->res = &node->data.local;
	tac->op1 = &node->data.symEntry->symbol.value;
	tac->prev = NULL;
	tac->next = NULL;
	
	return tac;
}

TAC* geraCod_noIdent(comp_tree_t *node)
{
	TAC *tac1 = malloc(sizeof(TAC));
	tac1->type = TAC_LOADI;
	tac1->res = &node->data.local;
	tac1->op1 = &node->data.symEntry->symbol.symAddr;
	
	TAC *tac2 = malloc(sizeof(TAC));
	tac2->type = TAC_LOAD;
	tac2->res = node->data.local;
	tac2->op1 = &node->data.local;
	
	tac1->prev = NULL;
	tac1->next = tac2;
	tac2->prev = tac1;
	tac2->next = NULL;
	
	return tac1;	
}

TAC* generateCode(comp_tree_t *node)
{
  // TODO 
  TAC* children = generateCode(node->left);
  TAC* brothers = generateCode(node->right);

  // TODO
	switch(node->data.nodeType)
	{
		case IKS_AST_PROGRAMA:
		case IKS_AST_FUNCAO:
		case IKS_AST_IF_ELSE:
		case IKS_AST_DO_WHILE:
		case IKS_AST_WHILE_DO:
		case IKS_AST_INPUT:
		case IKS_AST_OUTPUT:
		case IKS_AST_ATRIBUICAO:
		case IKS_AST_RETURN:
		case IKS_AST_BLOCO:
		case IKS_AST_IDENTIFICADOR:
			node->data.code = geraCod_noIdent(node); break;
		case IKS_AST_LITERAL:
			node->data.code = geraCod_noLiteral(node); break;
		case IKS_AST_ARIM_SOMA:
		case IKS_AST_ARIM_SUBTRACAO:
		case IKS_AST_ARIM_MULTIPLICACAO:
		case IKS_AST_ARIM_DIVISAO:
		case IKS_AST_ARIM_INVERSAO:
		case IKS_AST_LOGICO_E:
		case IKS_AST_LOGICO_OU:
		case IKS_AST_LOGICO_COMP_DIF:
		case IKS_AST_LOGICO_COMP_IGUAL:
		case IKS_AST_LOGICO_COMP_LE:
		case IKS_AST_LOGICO_COMP_GE:
		case IKS_AST_LOGICO_COMP_L:
		case IKS_AST_LOGICO_COMP_G:
		case IKS_AST_LOGICO_COMP_NEGACAO:
		case IKS_AST_VETOR_INDEXADO: break;
		case IKS_AST_CHAMADA_DE_FUNCAO: break;
	}
  
  // TODO
  return NULL;
}
