/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 * 
 * @file comp_tree.h
 * @brief Tree data structure.
 *
 * This file contain a struct definition for a node of tree as well as
 * some basic operations to work with it.
 * To emulate a tree with a arbitrary number of children, 
 * the "right" pointer points to a node's brother while
 * the "left" one points to one of its children.
 */
//#include "comp_symbols.h"

#ifndef COMP_TREE_H
#define COMP_TREE_H

/** 
 * Data type held by a node. 
 * 
 * @todo Update it.
 */
typedef struct data_t {
	int nodeType;
	DictItem *symEntry;
} Data;

/**
 * This is a binary tree data structure, however each node can have
 * as many children as needed.
 * The node pointed by "left" is its first child while
 * the one pointed by "right" is its brother.
 */
typedef struct treeNode_t {
	Data data;
	struct treeNode_t* left;
	struct treeNode_t* right;
} comp_tree_t;

/**
 * Creates a new node.
 *
 * @param d Data of the new node.
 * @return Pointer to the created node.
 */
comp_tree_t* treeCreate(Data d);

/**
 * Inserts "newNode" in the tree as a son of "fatherNode".
 */
void treeInsert(comp_tree_t* newNode, comp_tree_t* fatherNode);

/**
 * Searches a node which contains a data equals to "d".
 *
 * @param root The root of the tree, where the search begins.
 * @param d Data whose the searched node must contain.
 */
comp_tree_t* treeSearch(comp_tree_t* root, Data d);

/**
 * Makes a depth-first walk in the tree starting from "root".
 * Here, the current node is showed at first and then its branches are visited.
 */
void treePreWalk(comp_tree_t* root);

/**
 * Makes a depth-first walk in the tree starting from "root"
 * Here, the deepest nodes are showed at first.
 */
void treePostWalk(comp_tree_t* root);

/**
 *  Deletes all nodes in the same level of "root" or below it.
 */
void treeDelete(comp_tree_t* root);

#endif
