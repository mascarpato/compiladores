#include "code.h"

int zero = 0;
int one = 1;

TAC* geraCod_noLiteral(comp_tree_t *node)
{
	TAC *tac; /* = malloc(sizeof(TAC));
	
	tac->type = TAC_LOADI;
	tac->res = &node->data.local;
	tac->op1 = &node->data.symEntry->symbol.value;
	tac->prev = NULL;
	tac->next = NULL;*/
	tac = create_tac(TAC_LOADI, &node->data.local, &node->data.symEntry->symbol.value, NULL);
	
	return tac;
}

TAC* geraCod_noIdent(comp_tree_t *node)
{
	TAC *tac1; /* = malloc(sizeof(TAC));
	tac1->type = TAC_LOADI;
	tac1->res = &node->data.local;
	tac1->op1 = &node->data.symEntry->symbol.symAddr;*/
	tac1 = create_tac (TAC_LOADI, &node->data.local, &node->data.symEntry->symbol.symAddr, NULL);
	
	TAC *tac2; /* = malloc(sizeof(TAC));
	tac2->type = TAC_LOAD;
	tac2->res = node->data.local;
	tac2->op1 = &node->data.local;*/
	tac2 = create_tac (TAC_LOAD, node->data.local, &node->data.local, NULL);
	 
	/*tac1->prev = NULL;
	tac1->next = tac2;
	tac2->prev = tac1;
	tac2->next = NULL;*/
	tac1 = join_tac (tac1, tac2);
	
	return tac1;	
}

TAC *geraCod_noAnd(comp_tree_t *node)
{
	char* regZero = geraTemp();
	char* rotContinua = geraRot();
	char* rotFim = geraRot();
	
	comp_tree_t *expr1 = node->left;
	comp_tree_t *expr2 = node->left->right;
	
	TAC *tac1 = create_tac(TAC_LOADI, regZero, &zero, NULL);
	TAC *tac2 = create_tac(TAC_CMP_EQ, &node->data.local, &expr1->data.local, regZero);
	TAC *tac3 = create_tac(TAC_CBR, &node->data.local, rotFim, rotContinua);
	TAC *tac4 = create_tac(TAC_LABEL, rotContinua, NULL, NULL);
	TAC *tac5 = create_tac(TAC_CMP_EQ, &node->data.local, &expr2->data.local, regZero);
	TAC *tac6 = create_tac(TAC_LABEL, rotFim, NULL, NULL);
	
	tac5 = join_tac(tac5, tac6);
	tac4 = join_tac(tac4, tac5);
	tac3 = join_tac(tac3, tac4);
	tac2 = join_tac(tac2, tac3);
	tac1 = join_tac(tac1, tac2);
	
	return tac1;
}

TAC *geraCod_noOr(comp_tree_t *node)
{
	char* regOne = geraTemp();
	char* rotContinua = geraRot();
	char* rotFim = geraRot();
	
	comp_tree_t *expr1 = node->left;
	comp_tree_t *expr2 = node->left->right;
	
	TAC *tac1 = create_tac(TAC_LOADI, regOne, &one, NULL);
	TAC *tac2 = create_tac(TAC_CMP_EQ, &node->data.local, &expr1->data.local, regOne);
	TAC *tac3 = create_tac(TAC_CBR, &node->data.local, rotFim, rotContinua);
	TAC *tac4 = create_tac(TAC_LABEL, rotContinua, NULL, NULL);
	TAC *tac5 = create_tac(TAC_CMP_EQ, &node->data.local, &expr2->data.local, regOne);
	TAC *tac6 = create_tac(TAC_LABEL, rotFim, NULL, NULL);
	
	tac5 = join_tac(tac5, tac6);
	tac4 = join_tac(tac4, tac5);
	tac3 = join_tac(tac3, tac4);
	tac2 = join_tac(tac2, tac3);
	tac1 = join_tac(tac1, tac2);
	
	return tac1;
}

TAC *geraCod_noIfThenElse(comp_tree_t *ifnode, TAC *truecode, TAC *falsecode)
{
	char* labTrue = geraRot();
	char *labFalse = geraRot();
	char *labFim = geraRot();
	
	TAC *tac1 = create_tac(TAC_CBR, ifnode->left, labTrue, labFalse);
	
	TAC *tac2 = create_tac(TAC_LABEL, labTrue, NULL, NULL);
	tac2 = join_tac(tac2, truecode);
	TAC *tac5 = create_tac(TAC_JUMPI, labFim, NULL, NULL);
	tac2 = join_tac(tac2, tac5);
	
	TAC *tac3 = create_tac(TAC_LABEL, labFalse, NULL, NULL);
	tac3 = join_tac(tac2, falsecode);
	
	TAC *tac4 = create_tac(TAC_LABEL, labFim, NULL, NULL);
	
	tac1 = join_tac(tac1, tac2);
	tac1 = join_tac(tac1, tac3);
	tac1 = join_tac(tac1, tac4);
	
	return tac1;
}

TAC* geraCod_aritOpt(comp_tree_t *node, int TAC_type)
{
	
	char* regOne = geraTemp();
	char* regTwo = geraTemp();
	char* regThree = geraTemp();
		
	regOne = node->left->data.local
	regTwo = node->left->right->data.local

	TAC *tac = create_tac(TAC_type, regThree, regOne, regTwo);
	
	tac2 = join_tac (tac2, tac3);
	tac1 = join_tac (tac1, tac2);
	
	return tac;

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
			//node->data.code = geraCod_noIfThenElse(node); break;
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
			node->data.code = geraCod_aritOpt(node, TAC_ADD); break;
		case IKS_AST_ARIM_SUBTRACAO:
			node->data.code = geraCod_aritOpt(node, TAC_SUB); break;
		case IKS_AST_ARIM_MULTIPLICACAO:
			node->data.code = geraCod_aritOpt(node, TAC_MUL); break;
		case IKS_AST_ARIM_DIVISAO:
			node->data.code = geraCod_aritOpt(node, TAC_DIV); break;
		case IKS_AST_ARIM_INVERSAO:
// 			node->data.code = geraCod_noInversao(node); break;
		case IKS_AST_LOGICO_E:
			node->data.code = geraCod_noAnd(node); break;
		case IKS_AST_LOGICO_OU:
			node->data.code = geraCod_noOr(node); break;
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
