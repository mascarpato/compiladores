/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 */

#include <stdlib.h>
#include <stdio.h>
#include "comp_tree.h"
#include "iks_ast.h"

comp_tree_t* treeCreate(Data d)
{
	comp_tree_t* root = (comp_tree_t*) malloc(sizeof(comp_tree_t));
	root->data = d;
	root->left = NULL;
	root->right = NULL;

	if (d.nodeType == IKS_AST_FUNCAO)
		gv_declare(d.nodeType, root, root->data.symEntry->key);
	else if (d.nodeType == IKS_AST_IDENTIFICADOR)
		gv_declare(d.nodeType, root, root->data.symEntry->key);
	else if (d.nodeType == IKS_AST_LITERAL) {
		gv_declare(d.nodeType, root, d.symEntry->key);
	} else
		gv_declare(d.nodeType, root, NULL);
	
	return root;
}

void treeInsert(comp_tree_t* newNode, comp_tree_t* fatherNode)
{
	if(fatherNode->left == NULL)
		fatherNode->left = newNode;
	else{
		comp_tree_t* a = fatherNode->left;
		while(a->right != NULL)
			a = a->right;
		a->right = newNode;
	}
	
	if (newNode != NULL) // Prevents from trying to add an empty node to the drawing
		gv_connect(fatherNode, newNode);
}			
		

// comp_tree_t* treeSearch(comp_tree_t* root, Data d)
// {
// 	if(root->data == d)
// 		return root;
// 	else{
// 		comp_tree_t* a = NULL;
// 		if(root->right != NULL)
// 			a = treeSearch(root->right, d);
// 		if(root->left != NULL && a == NULL)
// 			a = treeSearch(root->left, d);
// 		return a;
// 	}
// }

void treePreWalk(comp_tree_t* root)
{
	if(root != NULL){
		printf("%i ",root->data);
		treePreWalk(root->left);
		treePreWalk(root->right);
	}
}

void treePostWalk(comp_tree_t* root)
{
	if(root != NULL){
		treePostWalk(root->left);
		treePostWalk(root->right);
		printf("%i ",root->data);
	}
}

void treeDelete(comp_tree_t* root){
	if(root->right != NULL)
		treeDelete(root->right);
	if(root->left != NULL)
		treeDelete(root->left);
	free(root);
}
