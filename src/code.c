#include "code.h"

// Internal functions //TODO


// Main function
TAC* generateCode(comp_tree_t *node)
{
  

  // TODO 
  TAC* children = generateCode(node->left);
  TAC* brothers = generateCode(node->right);

  // TODO
  switch(node->data.nodeType)
  {
    case TAC_ADD: // return other_function(...);
    break;
      
  }
  
  // TODO
  return NULL;
}
