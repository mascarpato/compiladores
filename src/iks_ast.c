#include "iks_ast.h"
#include "tac.h"

comp_tree_t *ast_encadeiano_funcao(comp_tree_t *root, comp_tree_t *father, comp_tree_t *child) 
{
	if (root->left == NULL) {
		//printf("Inserindo %s na raiz!\n",
		child->data.symEntry->key;
		treeInsert(child, root);
	} else {
		//printf("Inserindo %s na arvore ja com raiz!\n",
		child->data.symEntry->key;
		treeInsert(child, father);
	}
	
	return child;
}

comp_tree_t *ast_criano_funcao(DictItem *function_sym, comp_tree_t *commands) 
{
	Data data;
	data.nodeType = IKS_AST_FUNCAO;
	data.symEntry = function_sym;

	comp_tree_t *father = treeCreate(data);
	treeInsert(commands, father);
	
	return father;
}

comp_tree_t *ast_criano_bloco(comp_tree_t *seq_comandos)
{
	Data data;
	data.nodeType = IKS_AST_BLOCO;
	data.symEntry = NULL;
	comp_tree_t *father = treeCreate(data);
	treeInsert(seq_comandos, father);
	
	return father;
}

comp_tree_t *ast_criano_ifthenelse(comp_tree_t *expr, comp_tree_t *com_true, comp_tree_t *com_false)
{
	Data data;
	data.nodeType = IKS_AST_IF_ELSE;
	data.symEntry = NULL;
	data.local = geraTemp();
	comp_tree_t *father = treeCreate(data);
				
	treeInsert(expr, father);
	treeInsert(com_true, father);
	treeInsert(com_false, father); // Optional else
	
	return father;
}

comp_tree_t *ast_criano_dowhile(comp_tree_t *com, comp_tree_t *expr) 
{
	Data data;
	data.nodeType = IKS_AST_DO_WHILE;
	data.symEntry = NULL;
	comp_tree_t *father = treeCreate(data);                  

	treeInsert(com, father);
	treeInsert(expr, father);
	
	return father;
}

comp_tree_t *ast_criano_return(comp_tree_t *expr)
{
	Data data;
	data.nodeType = IKS_AST_RETURN;
	data.symEntry = NULL;
	
	comp_tree_t *father = treeCreate(data);
	treeInsert(expr, father);
	
	return father;
}

comp_tree_t *ast_criano_entrada(comp_tree_t *expr)
{
	Data data;
	data.nodeType = IKS_AST_INPUT;
	data.symEntry = NULL;
	comp_tree_t *father = treeCreate(data);
	
	Data data2;
	data2.nodeType = IKS_AST_IDENTIFICADOR;
	data2.symEntry = expr->data.symEntry;
	comp_tree_t *son = treeCreate(data2);
	
	treeInsert(son, father);
	
	return father;
}

comp_tree_t *ast_criano_saida(comp_tree_t *saida) 
{
	Data data;
	data.nodeType = IKS_AST_OUTPUT;
	data.symEntry = NULL;

	comp_tree_t *father = treeCreate(data);
	treeInsert(saida, father);
	
	return father;
}

comp_tree_t *ast_criano_chamadafuncao(DictItem *identifier, comp_tree_t *args)
{
	Data data;
	data.nodeType = IKS_AST_CHAMADA_DE_FUNCAO;
	data.symEntry = NULL;
	data.semanticType = identifier->symbol.symType & MASK_SYMTYPE_TYPE;
	comp_tree_t *father = treeCreate(data);
	
	Data data2;
	data2.nodeType = IKS_AST_IDENTIFICADOR;
	data2.symEntry = identifier;
	comp_tree_t *son = treeCreate(data2);

	treeInsert(son, father);
	treeInsert(args, father);
	
	return father;
}

comp_tree_t *ast_criano_identificador(DictItem *identifer)
{
	Data data;
	data.nodeType = IKS_AST_IDENTIFICADOR;
	data.symEntry = identifer;
	data.local = geraTemp();
	data.semanticType = identifer->symbol.symType & MASK_SYMTYPE_TYPE;
	comp_tree_t *daddy = treeCreate(data);
	
	return daddy;
}

comp_tree_t *ast_criano_vetor(DictItem *identifier, comp_tree_t *exprnode)
{
	Data data;
	data.nodeType = IKS_AST_VETOR_INDEXADO;
	data.symEntry = identifier;
	data.semanticType = identifier->symbol.symType & MASK_SYMTYPE_TYPE;
	comp_tree_t *vader = treeCreate(data);
	
	Data data2;
	data2.nodeType = IKS_AST_IDENTIFICADOR;
	data2.semanticType = identifier->symbol.symType & MASK_SYMTYPE_TYPE;
	data2.symEntry = identifier;
	comp_tree_t *luke = treeCreate(data2);
	
	treeInsert(luke, vader);
	treeInsert(exprnode, vader);
		
	return vader;
}

comp_tree_t *ast_criano_literal(DictItem *identifier, int symType)
{
	Data data;
	data.nodeType = IKS_AST_LITERAL;
	data.semanticType = symType;
	data.local = geraTemp();
	data.symEntry = identifier;
	
	return treeCreate(data);
}

comp_tree_t *ast_criano_atrib(DictItem *identifier, comp_tree_t *expr)
{
	Data data;
	data.nodeType = IKS_AST_ATRIBUICAO;
	data.symEntry = NULL;
	data.local = geraTemp();
	data.semanticType = identifier->symbol.symType & MASK_SYMTYPE_TYPE;
	comp_tree_t *attributionNode = treeCreate(data);
	
	Data data2;
	data2.nodeType = IKS_AST_IDENTIFICADOR;
	data2.symEntry = identifier;
	// Redundante ? data2.semanticType
	comp_tree_t *id = treeCreate(data2);
	
	
	
	treeInsert(id, attributionNode);
	treeInsert(expr, attributionNode);
	
	return attributionNode;
}

comp_tree_t *ast_criano_op(comp_tree_t *expr1, comp_tree_t *expr2, int nodeType)
{
	Data data;
	data.nodeType = nodeType;
	data.symEntry = NULL;
	data.local = geraTemp();
	
	data.semanticType = eval_infer(
				expr1->data.semanticType, expr2->data.semanticType,
				&(expr1->data.semanticType), &(expr2->data.semanticType));
	
	comp_tree_t *father = treeCreate(data);
	treeInsert(expr1, father);
	treeInsert(expr2, father);
	
	return father;
}

comp_tree_t *ast_criano_soma(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_op(expr1, expr2, IKS_AST_ARIM_SOMA);
}

comp_tree_t *ast_criano_sub(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_op(expr1, expr2, IKS_AST_ARIM_SUBTRACAO);
}

comp_tree_t *ast_criano_mult(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_op(expr1, expr2, IKS_AST_ARIM_MULTIPLICACAO);
}

comp_tree_t *ast_criano_div(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_op(expr1, expr2, IKS_AST_ARIM_DIVISAO);
}

comp_tree_t *ast_criano_and(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_op(expr1, expr2, IKS_AST_LOGICO_E);
}

comp_tree_t *ast_criano_or(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_op(expr1, expr2, IKS_AST_LOGICO_OU);
}

comp_tree_t *ast_criano_neg(comp_tree_t *expr) // TODO Tá fazendo isso errado. Nao é nao é nó cmp_negacao.
{
	Data data;
	data.nodeType = IKS_AST_LOGICO_COMP_NEGACAO;
	data.symEntry = NULL;
	data.semanticType = expr->data.semanticType;
	
	data.local = geraTemp();
	
	comp_tree_t *father = treeCreate(data);
	
	treeInsert(expr, father);
	
	return father;
}

comp_tree_t *ast_criano_inv(comp_tree_t *expr)
{
	Data data;
	data.nodeType = IKS_AST_ARIM_INVERSAO;
	data.symEntry = NULL;
	data.semanticType = expr->data.semanticType;
	data.local = geraTemp();
	
	// printf("expr.semType = %d\n", data.semanticType);	
	
	comp_tree_t *father = treeCreate(data);
	
	treeInsert(expr, father);
	
	return father;
}

comp_tree_t *ast_criano_cmp(comp_tree_t *expr1, comp_tree_t *expr2, int nodeType)
{
	Data data;
	data.nodeType = nodeType;
	data.local = geraTemp();
	data.symEntry = NULL;
	
	data.semanticType = eval_infer(
			expr1->data.semanticType, expr2->data.semanticType,
			&(expr1->data.semanticType), &(expr2->data.semanticType));
	comp_tree_t *father = treeCreate(data);
	
	treeInsert(expr1, father);
	treeInsert(expr2, father);
	
	return father;
}

comp_tree_t *ast_criano_cmpLE(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_cmp(expr1, expr2, IKS_AST_LOGICO_COMP_LE);
}

comp_tree_t *ast_criano_cmpGE(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_cmp(expr1, expr2, IKS_AST_LOGICO_COMP_GE);
}

comp_tree_t *ast_criano_cmpEQ(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_cmp(expr1, expr2, IKS_AST_LOGICO_COMP_IGUAL);
}

comp_tree_t *ast_criano_cmpNE(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_cmp(expr1, expr2, IKS_AST_LOGICO_COMP_DIF);
}

comp_tree_t *ast_criano_cmpL(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_cmp(expr1, expr2, IKS_AST_LOGICO_COMP_L);
}

comp_tree_t *ast_criano_cmpG(comp_tree_t *expr1, comp_tree_t *expr2)
{
	return ast_criano_cmp(expr1, expr2, IKS_AST_LOGICO_COMP_G);
}