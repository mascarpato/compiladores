/*
  iks_ast.h

  Este arquivo contém as constantes para os tipos dos nós da AST.
*/
#include "comp_tree.h"

#ifndef __IKS_AST_H
#define __IKS_AST_H

#define IKS_AST_PROGRAMA             0
#define IKS_AST_FUNCAO               1
#define IKS_AST_IF_ELSE              2
#define IKS_AST_DO_WHILE             3
#define IKS_AST_WHILE_DO             4
#define IKS_AST_INPUT                5
#define IKS_AST_OUTPUT               6
#define IKS_AST_ATRIBUICAO           7
#define IKS_AST_RETURN               8
#define IKS_AST_BLOCO                9	//TODO ???
#define IKS_AST_IDENTIFICADOR       10
#define IKS_AST_LITERAL             11
#define IKS_AST_ARIM_SOMA           12
#define IKS_AST_ARIM_SUBTRACAO      13
#define IKS_AST_ARIM_MULTIPLICACAO  14
#define IKS_AST_ARIM_DIVISAO        15
#define IKS_AST_ARIM_INVERSAO       16 // - (operador unário -)
#define IKS_AST_LOGICO_E            17 // &&
#define IKS_AST_LOGICO_OU           18 // ||
#define IKS_AST_LOGICO_COMP_DIF     19 // !=
#define IKS_AST_LOGICO_COMP_IGUAL   20 // ==
#define IKS_AST_LOGICO_COMP_LE      21 // <=
#define IKS_AST_LOGICO_COMP_GE      22 // >=
#define IKS_AST_LOGICO_COMP_L       23 // <
#define IKS_AST_LOGICO_COMP_G       24 // >
#define IKS_AST_LOGICO_COMP_NEGACAO 25 //TODO !
#define IKS_AST_VETOR_INDEXADO      26 // para var[exp] quando o índice exp é acessado no vetor var
#define IKS_AST_CHAMADA_DE_FUNCAO   27
#endif

comp_tree_t *ast_encadeiano_funcao(comp_tree_t *root, comp_tree_t *father, comp_tree_t *child);
comp_tree_t *ast_criano_funcao(DictItem *function_sym, comp_tree_t *commands);
comp_tree_t *ast_criano_bloco(comp_tree_t *seq_comandos);
comp_tree_t *ast_criano_ifthenelse(comp_tree_t *expr, comp_tree_t *com_tree, comp_tree_t *com_false);
comp_tree_t *ast_criano_dowhile(comp_tree_t *com, comp_tree_t *expr);
comp_tree_t *ast_criano_return(comp_tree_t *expr);
comp_tree_t *ast_criano_entrada(comp_tree_t *expr);
comp_tree_t *ast_criano_saida(comp_tree_t *saida);
comp_tree_t *ast_criano_chamadafuncao(DictItem *identifier, comp_tree_t *args);
comp_tree_t *ast_criano_identificador(DictItem *identifer);
comp_tree_t *ast_criano_vetor(DictItem *identifier, comp_tree_t *exprnode);
comp_tree_t *ast_criano_literal(DictItem *identifier, int symType);
comp_tree_t *ast_criano_atrib(DictItem *identifier, comp_tree_t *expr);

comp_tree_t *ast_criano_soma(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_sub(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_mult(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_div(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_and(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_or(comp_tree_t *expr1, comp_tree_t *expr2);

comp_tree_t *ast_criano_neg(comp_tree_t *expr);
comp_tree_t *ast_criano_inv(comp_tree_t *expr);

comp_tree_t *ast_criano_cmpLE(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_cmpGE(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_cmpEQ(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_cmpNE(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_cmpL(comp_tree_t *expr1, comp_tree_t *expr2);
comp_tree_t *ast_criano_cmpG(comp_tree_t *expr1, comp_tree_t *expr2);
