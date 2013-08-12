#include <stdio.h>
#include "comp_tree.h"

int vector[] = {0, 1, 2, 3, 4, 5, 6};

/** Tests the tree functions defined in \file comp_tree.h
  * This testbench makes the following tree structure:
  *           0
  *         __|__
  *        1     2
  *      __|__ 
  *     3  4  6
  *        |
  *        5
 */
int main() 
{
  comp_tree_t* root;
  comp_tree_t* node[5];
  comp_tree_t* elem;
  int i;
  
  //Creates each node
  root = treeCreate(vector[0]);
  for(i=0;i<6;i++)
  	node[i] = treeCreate(vector[i+1]);

  //Builds the tree structure
  treeInsert(node[0],root);
  treeInsert(node[1],root);
  treeInsert(node[2],node[0]);
  treeInsert(node[3],node[0]);
  elem = treeSearch(root, vector[4]); //node[3]
  treeInsert(node[4],elem);
  treeInsert(node[5],node[0]);
  
  treePreWalk(root);  // 0 1 3 4 5 6 2
  treePostWalk(root); // 5 6 4 3 2 1 0
  
  treeDelete(root);
 }  
