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
    case TAC_NOP: // return other_function(...);
    	break;
	
    case TAC_ADD: // return other_function(...);
    	break;
    
    case TAC_SUB: // return other_function(...);
    	break;
	
    case TAC_MUL: // return other_function(...);
    	break;
    
    case TAC_DIV: // return other_function(...);
    	break;

    case TAC_ADDI: // return other_function(...);
    	break;
	
    case TAC_SUBI: // return other_function(...);
    	break;
	
    case TAC_RSUBI: // return other_function(...);
    	break;
	
    case TAC_MULTI: // return other_function(...);
    	break;
	
    case TAC_DIVI: // return other_function(...);
    	break;
	
    case TAC_RDIVI: // return other_function(...);
    	break;
	
      
  }
  
  // TODO
  return NULL;
}
