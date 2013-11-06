/** @file code.h
 * 
 * Contains function for generating Code given an AST. 
 * 
 */

#include <stdio.h>
#include "comp_dict.h"
#include "comp_tree.h"
#include "tac.h"
#include "iks_ast.h"

/**
 * Generates code given an AST.
 * 
 * @param node Root of the AST.
 * 
 * @return Full TAC for that AST.
 */
TAC* generateCode(comp_tree_t *node);
