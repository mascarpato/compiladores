#include "code.h"

int zero = 0;
int one = 1;

TAC* geraCod_noLiteral(comp_tree_t *node);
TAC* geraCod_noIdent(comp_tree_t *node);
TAC *geraCod_noAnd(comp_tree_t *node);
TAC *geraCod_noOr(comp_tree_t *node);
TAC *geraCod_noIfThenElse(comp_tree_t *ifnode, TAC *expr, TAC *truecode, TAC *falsecode, TAC *next);
TAC* geraCod_aritOpt(comp_tree_t *node, int TAC_type);
TAC *geraCod_noAtrib(comp_tree_t *node, TAC *expr);
TAC *geraCod_noCmp(int TAC_TYPE, comp_tree_t *node, TAC *expr1, TAC *expr2);
TAC *geraCod_noInversao(comp_tree_t *node);
TAC *geraCod_noWhile(comp_tree_t *node, TAC *expr, TAC *comm);
TAC *geraCod_noDoWhile(comp_tree_t *node, TAC *comm, TAC *expr);

TAC* generateCode(comp_tree_t *node)
{
	if (node == NULL)
		return NULL;
	
	// printf("Trancou na recursao.\n");
	
		// 4 childs max
	TAC *babyTAC[] = {NULL, NULL, NULL, NULL};
	babyTAC[0] = generateCode(node->left);
	if (node->left != NULL) {
		babyTAC[1] = generateCode(node->left->right);
		if (node->left->right != NULL) {
			babyTAC[2] = generateCode(node->left->right->right);
			if (node->left->right->right != NULL) {
				babyTAC[3] = generateCode(node->left->right->right->right);
			}
		}
	}
	// Yes it looks ugly. but it works.

	switch(node->data.nodeType)
	{
		case IKS_AST_PROGRAMA:
			node->data.code = babyTAC[0];
			//printf("TAC Prog: \n"); print_tac(node->data.code);
			print_tac(node->data.code); break;
		case IKS_AST_FUNCAO:
			node->data.code = babyTAC[0];
			//printf("TAC Fun: \n"); print_tac(node->data.code);
			node->data.code = join_tac(node->data.code, babyTAC[1]); 
			//printf("TAC Fun+chain: \n"); print_tac(node->data.code); 
			break;
		case IKS_AST_IF_ELSE:
			node->data.code = geraCod_noIfThenElse(node, babyTAC[0], babyTAC[1], babyTAC[2], babyTAC[3]); break;
		case IKS_AST_DO_WHILE:
			node->data.code = geraCod_noDoWhile(node, babyTAC[0], babyTAC[1]);
			node->data.code = join_tac(node->data.code, babyTAC[2]); break;
		case IKS_AST_WHILE_DO:
			node->data.code = geraCod_noWhile(node, babyTAC[0], babyTAC[1]);
			node->data.code = join_tac(node->data.code, babyTAC[2]); break;
		case IKS_AST_INPUT:
		case IKS_AST_OUTPUT:
		case IKS_AST_ATRIBUICAO:
			node->data.code = geraCod_noAtrib(node, babyTAC[1]);
			//printf("TAC Atrib: \n"); print_tac(node->data.code);
			node->data.code = join_tac(node->data.code, babyTAC[2]); // Chain com proximo comando
			//printf("TAC Atrib+chain: \n"); print_tac(node->data.code);
			break;
		case IKS_AST_RETURN:
			node->data.code = NULL; break;
		case IKS_AST_BLOCO:
			node->data.code = babyTAC[0]; break;
		case IKS_AST_IDENTIFICADOR:
			node->data.code = geraCod_noIdent(node);
			//printf("TAC Idenfiticador: \n"); print_tac(node->data.code); 
			break;
		case IKS_AST_LITERAL:
			node->data.code = geraCod_noLiteral(node);
			//printf("TAC Literal: \n"); print_tac(node->data.code); 
			break;
		case IKS_AST_ARIM_SOMA:
			node->data.code = geraCod_aritOpt(node, TAC_ADD); 
			node->data.code = join_tac(babyTAC[1], node->data.code);
			node->data.code = join_tac(babyTAC[0], node->data.code); break;
		case IKS_AST_ARIM_SUBTRACAO:
			node->data.code = geraCod_aritOpt(node, TAC_SUB); 
			node->data.code = join_tac(babyTAC[1], node->data.code);
			node->data.code = join_tac(babyTAC[0], node->data.code); break;
		case IKS_AST_ARIM_MULTIPLICACAO:
			node->data.code = geraCod_aritOpt(node, TAC_MUL);
			node->data.code = join_tac(babyTAC[1], node->data.code);
			node->data.code = join_tac(babyTAC[0], node->data.code); break;
		case IKS_AST_ARIM_DIVISAO:
			node->data.code = geraCod_aritOpt(node, TAC_DIV);
			node->data.code = join_tac(babyTAC[1], node->data.code);
			node->data.code = join_tac(babyTAC[0], node->data.code); break;
		case IKS_AST_ARIM_INVERSAO:
 			node->data.code = geraCod_noInversao(node); 
			node->data.code = join_tac(babyTAC[0], node->data.code); break;
		case IKS_AST_LOGICO_E:
			node->data.code = geraCod_noAnd(node);
			node->data.code = join_tac(babyTAC[1], node->data.code);
			node->data.code = join_tac(babyTAC[0], node->data.code); break;
		case IKS_AST_LOGICO_OU:
			node->data.code = geraCod_noOr(node);
			node->data.code = join_tac(babyTAC[1], node->data.code);
			node->data.code = join_tac(babyTAC[0], node->data.code); break;
		case IKS_AST_LOGICO_COMP_DIF:
			node->data.code = geraCod_noCmp(TAC_CMP_NE, node, babyTAC[0], babyTAC[1]); break;
		case IKS_AST_LOGICO_COMP_IGUAL:
			node->data.code = geraCod_noCmp(TAC_CMP_EQ, node, babyTAC[0], babyTAC[1]); break;
		case IKS_AST_LOGICO_COMP_LE:
			node->data.code = geraCod_noCmp(TAC_CMP_LE, node, babyTAC[0], babyTAC[1]); break;
		case IKS_AST_LOGICO_COMP_GE:
			node->data.code = geraCod_noCmp(TAC_CMP_GE, node, babyTAC[0], babyTAC[1]); break;
		case IKS_AST_LOGICO_COMP_L:
			node->data.code = geraCod_noCmp(TAC_CMP_LT, node, babyTAC[0], babyTAC[1]); break;
		case IKS_AST_LOGICO_COMP_G:
			node->data.code = geraCod_noCmp(TAC_CMP_GT, node, babyTAC[0], babyTAC[1]); break;
		case IKS_AST_LOGICO_COMP_NEGACAO: // TODO Errado
		case IKS_AST_VETOR_INDEXADO: break;
		case IKS_AST_CHAMADA_DE_FUNCAO: break;
  }
  
  return node->data.code;
}

TAC* geraCod_noLiteral(comp_tree_t *node)
{
	TAC *tac = create_tac(TAC_LOADI, node->data.local, &node->data.symEntry->symbol.value, NULL);
	
	return tac;
}

TAC* geraCod_noIdent(comp_tree_t *node)
{
	char *regbss = "rbss";
	char *regarp = "rarp";
	
	/* Deve fazer distincao entre variavel local (soma endereco com inicio do reg. ativ) 
	 * e var global (soma com pointer para regiao de dados) */
	TAC *tac1 = create_tac (TAC_LOADI, node->data.local, &node->data.symEntry->symbol.symAddr, NULL);
	
	TAC *tac3;
	if (node->data.symEntry->symbol.symType & SYM_IS_LOCALSCOPE) { // vai somar endereco com rarp
		tac3 = create_tac (TAC_ADD, node->data.local, node->data.local, regarp);
	}
	else { // vai somar endereco com bss
		tac3 = create_tac (TAC_ADD, node->data.local, node->data.local, regbss); 
	}
	
	tac1 = join_tac(tac1, tac3);
	
	TAC *tac2 = create_tac (TAC_LOAD, node->data.local, node->data.local, NULL);
	tac1 = join_tac(tac1, tac2);
	
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
	TAC *tac2 = create_tac(TAC_CMP_EQ, node->data.local, expr1->data.local, regZero);
	TAC *tac3 = create_tac(TAC_CBR, node->data.local, rotFim, rotContinua);
	TAC *tac4 = create_tac(TAC_LABEL, rotContinua, NULL, NULL);
	TAC *tac5 = create_tac(TAC_CMP_EQ, node->data.local, expr2->data.local, regZero);
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
	TAC *tac2 = create_tac(TAC_CMP_EQ, node->data.local, expr1->data.local, regOne);
	TAC *tac3 = create_tac(TAC_CBR, node->data.local, rotFim, rotContinua);
	TAC *tac4 = create_tac(TAC_LABEL, rotContinua, NULL, NULL);
	TAC *tac5 = create_tac(TAC_CMP_EQ, node->data.local, expr2->data.local, regOne);
	TAC *tac6 = create_tac(TAC_LABEL, rotFim, NULL, NULL);
	
	tac5 = join_tac(tac5, tac6);
	tac4 = join_tac(tac4, tac5);
	tac3 = join_tac(tac3, tac4);
	tac2 = join_tac(tac2, tac3);
	tac1 = join_tac(tac1, tac2);
	
	return tac1;
}

TAC *geraCod_noIfThenElse(comp_tree_t *ifnode, TAC *expr, TAC *truecode, TAC *falsecode, TAC *next)
{
	char* labTrue = geraRot();
	char *labFalse = geraRot();
	char *labFim = geraRot();
	
	TAC *tac1 = create_tac(TAC_CBR, ifnode->left->data.local, labTrue, labFalse);
	tac1 = join_tac(expr, tac1);
	
	TAC *tac2 = create_tac(TAC_LABEL, labTrue, NULL, NULL);
	tac1 = join_tac(tac1, tac2);
		// Codigo true entra aqui
	tac1 = join_tac(tac1, truecode);
	
	/*IF-THEN-ELSE
		-- cond
		-- cod-true
		-- cod-false
		-- next
		
	IF-THEN-ELSE
		-- cond
		-- cod-true
		-- next
		
	IF-THEN-ELSE
		-- cond
		-- cod-true
		-- NULL*/
	
	/*
	expr != null sempre
	trucode != null sempre
	3 casos
		false code != null e next != null --- Há comando para else e há proximo comando
		false code != null e next == null --- falsecode pode ser um else ou proximo comando
		false code == null e next == null --- é só um if (sem else) e ultimo comando do bloco*/
	// 
	
	TAC *tac3 = create_tac(TAC_JUMPI, labFim, NULL, NULL);
	tac1 = join_tac(tac1, tac3);
	
	TAC *tac4 = create_tac(TAC_LABEL, labFalse, NULL, NULL);
	tac1 = join_tac(tac1, tac4);
		// Codigo false entra aqui
	tac1 = join_tac(tac1, falsecode);

	TAC *tac5 = create_tac(TAC_LABEL, labFim, NULL, NULL);
	tac1 = join_tac(tac1, tac5);
	tac1 = join_tac(tac1, next);
	
	return tac1;
}

TAC* geraCod_aritOpt(comp_tree_t *node, int TAC_type)
{
	
	char* regOne = node->left->data.local;;
	char* regTwo = node->left->right->data.local;
	char* regThree = node->data.local;

	TAC *tac = create_tac(TAC_type, regThree, regOne, regTwo);
	
	return tac;
}

TAC *geraCod_noAtrib(comp_tree_t *node, TAC *expr)
{
	TAC *tac1 = create_tac(TAC_LOADI, 
				node->data.local, &node->left->data.symEntry->symbol.symAddr, NULL);
	tac1 = join_tac(expr, tac1);
	char *regbss = "rbss";
	char *regarp = "rarp";
	
	/* Deve fazer distincao entre variavel local (soma endereco com inicio do reg. ativ) 
	 * e var global (soma com pointer para regiao de dados) */
	TAC *tac3;
	if (node->left->data.symEntry->symbol.symType & SYM_IS_LOCALSCOPE) { // vai somar endereco com rarp
		tac3 = create_tac (TAC_ADD, node->data.local, node->data.local, regarp);
	}
	else { // vai somar endereco com bss
		tac3 = create_tac (TAC_ADD, node->data.local, node->data.local, regbss); 
	}
	tac1 = join_tac(tac1, tac3);
	
	TAC *tac2 = create_tac(TAC_STORE,
				node->left->right->data.local, node->data.local, NULL);

	tac1 = join_tac(tac1, tac2);
	
	return tac1;	
}

TAC *geraCod_noCmp(int TAC_TYPE, comp_tree_t *node, TAC *expr1, TAC *expr2)
{
	TAC *tac1 = create_tac(TAC_TYPE,
		node->data.local, node->left->data.local, node->left->right->data.local);
	tac1 = join_tac(expr1, tac1);
	tac1 = join_tac(expr2, tac1);
	
	return tac1;
}

TAC *geraCod_noInversao(comp_tree_t *node) 
{
	TAC *tac1 = create_tac(TAC_LOADI,
		node->data.local, &zero, NULL);
	TAC *tac2 = create_tac(TAC_SUB,
		node->data.local, node->data.local, node->left->data.local);
	
	tac1 = join_tac(tac1, tac2);
	
	return tac1;
}


TAC *geraCod_noWhile(comp_tree_t *node, TAC *expr, TAC *comm)
{
	char *rotControle = geraRot();
	char *rotBloco = geraRot();
	char *rotContinua = geraRot();
	
	TAC *tac1 = create_tac(TAC_LABEL,
		rotControle, NULL, NULL);
	tac1 = join_tac(tac1, expr);
	
	TAC *tac2 = create_tac(TAC_CBR,
			node->left->data.local, rotBloco, rotContinua);
	tac2 = join_tac(tac1, tac2);
	
	TAC *tac3 = create_tac(TAC_LABEL,
			rotBloco, NULL, NULL);
	tac2 = join_tac(tac2, tac3);
	tac2 = join_tac(tac2, comm);
	
	TAC *tac4 = create_tac(TAC_JUMPI,
			rotControle, NULL, NULL);
	tac2 = join_tac(tac2, tac4);
	
	TAC *tac5 = create_tac(TAC_LABEL,
		rotContinua, NULL, NULL);
	tac2 = join_tac(tac2, tac5);
	
	return tac2;
}

TAC *geraCod_noDoWhile(comp_tree_t *node, TAC *comm, TAC *expr)
{
	char *rotBloco = geraRot();
	char *rotContinua = geraRot();
	
	TAC *tac1 = create_tac(TAC_LABEL,
		rotBloco, NULL, NULL);
	tac1 = join_tac(tac1, comm);	
	
	tac1 = join_tac(tac1, expr);
	TAC *tac2 = create_tac(TAC_CBR,
			node->left->right->data.local, rotBloco, rotContinua);
	tac1 = join_tac(tac1, tac2);
	
	TAC *tac3 = create_tac(TAC_LABEL,
			rotContinua, NULL, NULL);
	tac1 = join_tac(tac1, tac3);
	
	return tac1;
}