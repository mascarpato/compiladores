#include "code.h"

int zero = 0;
int one = 1;

TAC* geraCod_noLiteral(comp_tree_t *node)
{
	TAC *tac = create_tac(TAC_LOADI, &node->data.local, &node->data.symEntry->symbol.value, NULL);
	
	return tac;
}

TAC* geraCod_noIdent(comp_tree_t *node)
{
	TAC *tac1 = create_tac (TAC_LOADI, &node->data.local, &node->data.symEntry->symbol.symAddr, NULL);
	
	TAC *tac2 = create_tac (TAC_LOAD, node->data.local, &node->data.local, NULL);
	
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

TAC *geraCod_noIfThenElse(comp_tree_t *ifnode, TAC *expr, TAC *truecode, TAC *falsecode)
{
	char* labTrue = geraRot();
	char *labFalse = geraRot();
	char *labFim = geraRot();
	
	TAC *tac1 = create_tac(TAC_CBR, ifnode->left->data.local, labTrue, labFalse);
	
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
		
	regOne = node->left->data.local;
	regTwo = node->left->right->data.local;

	TAC *tac = create_tac(TAC_type, regThree, regOne, regTwo);
	
	return tac;
}

TAC *geraCod_noAtrib(comp_tree_t *node, TAC *expr)
{
	TAC *tac1 = create_tac(TAC_LOADI, 
				node->data.local, &node->left->data.symEntry->symbol.symAddr, NULL);
	TAC *tac2 = create_tac(TAC_STORE,
				node->left->right->data.local, node->data.local, NULL);

	tac1 = join_tac(expr, tac1);
	tac1 = join_tac(tac1, tac2);
	
	return tac1;	
}

TAC *geraCod_noCmpLe(comp_tree_t *node)
{
	TAC *tac1 = create_tac(TAC_CMP_LE,
		node->data.local, node->left->data.local, node->left->right->data.local);
	
	return tac1;
}

TAC* generateCode(comp_tree_t *node)
{
	if (node == NULL)
		return NULL;
	
		// 4 childs max
	TAC *babyTAC[] = {NULL, NULL, NULL, NULL};
	babyTAC[0] = generateCode(node->left);
	if (node->left != NULL)
		babyTAC[1] = generateCode(node->left->right);
	if (node->left->right != NULL)
		babyTAC[2] = generateCode(node->left->right->right);
	if (node->left->right->right != NULL)
		babyTAC[2] = generateCode(node->left->right->right->right);
	// Yes it looks ugly. but it works.

	switch(node->data.nodeType)
	{
		case IKS_AST_PROGRAMA:
		case IKS_AST_FUNCAO:
		case IKS_AST_IF_ELSE:
			node->data.code = geraCod_noIfThenElse(node, babyTAC[0], babyTAC[1], babyTAC[2]); break;
		case IKS_AST_DO_WHILE:
		case IKS_AST_WHILE_DO:
		case IKS_AST_INPUT:
		case IKS_AST_OUTPUT:
		case IKS_AST_ATRIBUICAO:
			node->data.code = geraCod_noAtrib(node, babyTAC[1]); 
			node->data.code = join_tac(node->data.code, babyTAC[2]); // Chain com proximo comando
			break;
		case IKS_AST_RETURN:
			node->data.code = NULL; break;
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
			node->data.code = geraCod_noCmpLe(node); break;
		case IKS_AST_LOGICO_COMP_GE:
		case IKS_AST_LOGICO_COMP_L:
		case IKS_AST_LOGICO_COMP_G:
		case IKS_AST_LOGICO_COMP_NEGACAO:
		case IKS_AST_VETOR_INDEXADO: break;
		case IKS_AST_CHAMADA_DE_FUNCAO: break;
	}
  
  return node->data.code;
}
