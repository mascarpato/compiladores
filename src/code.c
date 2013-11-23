#include "code.h"

int zero = 0;
int one = 1;
int four = 4;
int eight = 8;

TAC* geraCod_noLiteral(comp_tree_t *node);
TAC* geraCod_noIdent(comp_tree_t *node);
TAC *geraCod_noAnd(comp_tree_t *node, TAC *expr1, TAC *expr2);
TAC *geraCod_noOr(comp_tree_t *node, TAC *expr1, TAC *expr2);
TAC *geraCod_noIfThenElse(comp_tree_t *ifnode, TAC *expr, TAC *truecode, TAC *falsecode, TAC *next);
TAC* geraCod_aritOpt(comp_tree_t *node, int TAC_type);
TAC *geraCod_noAtrib(comp_tree_t *node, TAC *ident, TAC *expr);
TAC *geraCod_noCmp(int TAC_TYPE, comp_tree_t *node, TAC *expr1, TAC *expr2);
TAC *geraCod_noInversao(comp_tree_t *node);
TAC *geraCod_noWhile(comp_tree_t *node, TAC *expr, TAC *comm);
TAC *geraCod_noDoWhile(comp_tree_t *node, TAC *comm, TAC *expr);
TAC *geraCod_noVetor(comp_tree_t *node, TAC *dim1, TAC *dim2, TAC *dim3);

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
			node->data.code = geraCod_noAtrib(node, babyTAC[0], babyTAC[1]);
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
			node->data.code = geraCod_noAnd(node, babyTAC[0], babyTAC[1]); break;
		case IKS_AST_LOGICO_OU:
			node->data.code = geraCod_noOr(node, babyTAC[0], babyTAC[1]); break;
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
		case IKS_AST_LOGICO_COMP_NEGACAO: break;// TODO Errado
		case IKS_AST_VETOR_INDEXADO: 
			node->data.code = geraCod_noVetor(node, babyTAC[1], babyTAC[2], babyTAC[3]) ; break;
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

TAC *geraCod_noAnd(comp_tree_t *node, TAC *expr1, TAC *expr2)
{
	char* regZero = geraTemp();
	char* rotContinua = geraRot();
	char* rotFim = geraRot();
	
	comp_tree_t *expr1_node = node->left;
	comp_tree_t *expr2_node = node->left->right;
	
	TAC *tac1 = create_tac(TAC_LOADI, regZero, &zero, NULL);
	tac1 = join_tac(tac1, expr1);
	
	TAC *tac2 = create_tac(TAC_CMP_NE, node->data.local, expr1_node->data.local, regZero);
	tac1 = join_tac(tac1, tac2);
	
	TAC *tac3 = create_tac(TAC_CBR, node->data.local, rotContinua, rotFim);
	tac1 = join_tac(tac1, tac3);
	
	TAC *tac4 = create_tac(TAC_LABEL, rotContinua, NULL, NULL);
	tac1 = join_tac(tac1, tac4);
	tac1 = join_tac(tac1, expr2);
	
	TAC *tac5 = create_tac(TAC_CMP_NE, node->data.local, expr2_node->data.local, regZero);
	tac1 = join_tac(tac1, tac5);
	
	TAC *tac6 = create_tac(TAC_LABEL, rotFim, NULL, NULL);
	tac1 = join_tac(tac1, tac6);
	
	return tac1;
}

TAC *geraCod_noOr(comp_tree_t *node, TAC *expr1, TAC *expr2)
{
	char* regZero = geraTemp();
	char* rotContinua = geraRot();
	char* rotFim = geraRot();
	
	comp_tree_t *expr1_node = node->left;
	comp_tree_t *expr2_node = node->left->right;
	
	TAC *tac1 = create_tac(TAC_LOADI, regZero, &zero, NULL);
	tac1 = join_tac(tac1, expr1);
	
	TAC *tac2 = create_tac(TAC_CMP_NE, node->data.local, expr1_node->data.local, regZero);
	tac1 = join_tac(tac1, tac2);
	
	TAC *tac3 = create_tac(TAC_CBR, node->data.local, rotFim, rotContinua);
	tac1 = join_tac(tac1, tac3);
	
	TAC *tac4 = create_tac(TAC_LABEL, rotContinua, NULL, NULL);
	tac1 = join_tac(tac1, tac4);
	tac1 = join_tac(tac1, expr2);
	
	TAC *tac5 = create_tac(TAC_CMP_NE, node->data.local, expr2_node->data.local, regZero);
	tac1 = join_tac(tac1, tac5);
	
	TAC *tac6 = create_tac(TAC_LABEL, rotFim, NULL, NULL);
	tac1 = join_tac(tac1, tac6);
	
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

TAC *geraCod_noAtrib(comp_tree_t *node, TAC *ident, TAC *expr)
{	
	TAC *tac1;
	if(node->left->data.nodeType == IKS_AST_VETOR_INDEXADO){
		// Remove o último LOAD e o reg conterá o endereço
		TAC *aux;
		for (aux = ident; aux->next->next; aux = aux->next);
		free(aux->next);
		aux->next = NULL;
		
		tac1 = join_tac(ident, expr);		
		
	} else {
		tac1 = create_tac(TAC_LOADI, 
					node->data.local, &node->left->data.symEntry->symbol.symAddr, NULL);
		tac1 = join_tac(expr, tac1);
		char *regbss = "rbss";
		char *regarp = "rarp";
		
		// Deve fazer distincao entre variavel local (soma endereco com inicio do reg. ativ) 
		// e var global (soma com pointer para regiao de dados) 
		TAC *tac3;
		if (node->left->data.symEntry->symbol.symType & SYM_IS_LOCALSCOPE) { // vai somar endereco com rarp
			tac3 = create_tac (TAC_ADD, node->data.local, node->data.local, regarp);
		}
		else { // vai somar endereco com bss
			tac3 = create_tac (TAC_ADD, node->data.local, node->data.local, regbss); 
		}
		tac1 = join_tac(tac1, tac3);
	}
	
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

TAC *geraCod_noVetor(comp_tree_t *node, TAC *dim1, TAC *dim2, TAC *dim3)
{
 	char *regbss = "rbss";
	char *regarp = "rarp";
	
	/* Todas informações são guardadas no primeiro filho, o identificador */
	comp_tree_t *ident = node->left;
	
	/* Deve fazer distincao entre variavel local (soma endereco com inicio do reg. ativ) 
	 * e var global (soma com pointer para regiao de dados) */
	TAC *tac1 = create_tac (TAC_LOADI, ident->data.local, &ident->data.symEntry->symbol.symAddr, NULL);
	
	TAC *tac2;

	if (ident->data.symEntry->symbol.symType & SYM_IS_LOCALSCOPE) { // vai somar endereco com rarp
		tac2 = create_tac (TAC_ADD, ident->data.local, ident->data.local, regarp);
	}
	else { // vai somar endereco com bss
		tac2 = create_tac (TAC_ADD, ident->data.local, ident->data.local, regbss); 
	}
	
	tac1 = join_tac(tac1, tac2);
	
	/* Cálculo do índice */
	/* Péssimo código, mas funciona... */
	
	ListNode *dims = ident->data.symEntry->symbol.params;
	if(dim1 == NULL)
		printf("Ops, array not indexed...\n");
		
	char *regIndex = dim1->res;
	tac1 = join_tac(tac1, dim1);
	
	// 3 dimensões
	if(dim3 != NULL) {
		tac1 = join_tac(tac1, dim2);
		tac1 = join_tac(tac1, dim3);
		TAC *tacIndex1 = create_tac (TAC_MULTI, regIndex, regIndex, &dims->next->data);
		tac1 = join_tac(tac1, tacIndex1);
		TAC *tacIndex2 = create_tac (TAC_MULTI, regIndex, regIndex, &dims->next->next->data);
		tac1 = join_tac(tac1, tacIndex2);
		TAC *tacIndex3 = create_tac (TAC_MULTI, dim2->res, dim2->res, &dims->next->next->data);
		tac1 = join_tac(tac1, tacIndex3);
		TAC *tacIndex4 = create_tac (TAC_ADD, regIndex, regIndex, dim2->res);
		tac1 = join_tac(tac1, tacIndex4);
		TAC *tacIndex5 = create_tac (TAC_ADD, regIndex, regIndex, dim3->res);
		tac1 = join_tac(tac1, tacIndex5);		
	  
	// 2 dimensões
	} else if(dim2 != NULL) {
		tac1 = join_tac(tac1, dim2);
		TAC *tacIndex1 = create_tac (TAC_MULTI, regIndex, regIndex, &dims->next->data);
		tac1 = join_tac(tac1, tacIndex1);
		TAC *tacIndex2 = create_tac (TAC_ADD, regIndex, regIndex, dim2->res);
		tac1 = join_tac(tac1, tacIndex2);
	}
	
	/* Multiplicando pelo tamanho do tipo */
	int type = ident->data.symEntry->symbol.symType & MASK_SYMTYPE_TYPE; 
	TAC *tacType;
	switch(type){
	  case SYMTYPE_INT:
	      tacType = create_tac (TAC_MULTI, regIndex, regIndex, &four);
	      tac1 = join_tac(tac1, tacType);
	      break;
	  case SYMTYPE_FLOAT:
	      tacType = create_tac (TAC_MULTI, regIndex, regIndex, &eight);
	      tac1 = join_tac(tac1, tacType);
	      break;
	  case SYMTYPE_BOOL:
	      /* Boolean size = 1 */
	      break;
	  default: printf("Unnaceptable type for arrays"); exit(0); /* erro */
	}

	/* Adicionando ao endereço base */
	TAC *tac3 = create_tac (TAC_ADD, ident->data.local, ident->data.local, regIndex);
	tac1 = join_tac(tac1, tac3);
	
	/* Loading... */
	TAC *tac4 = create_tac (TAC_LOAD, ident->data.local, ident->data.local, NULL);
	tac1 = join_tac(tac1, tac4);
	
	node->data.local = ident->data.local;
	
	return tac1;
}