#include "tac.h"

TAC* create_tac (int type, DictItem *res, DictItem *op1, DictItem *op2)
{
	TAC *tac = NULL;
	tac = (TAC*)malloc(sizeof(TAC));
	tac->type = type;
	tac->res = res;
	tac->op1 = op1;	
	tac->op2 = op2;
	tac->prev = NULL;
	tac->next = NULL;

	return tac;
}

TAC* join_tac (TAC *l1, TAC *l2)
{

	TAC* tac = NULL;
	
	if(l1==NULL) //there's only TACs on l1
		return l2;
	if(l2=NULL)	//there's only TACs on l2
		return l1;
	
	//assumes that l1->prev and l2->prev are by default NULL
	tac = l1;
	while (tac->next != NULL) //concat the very ending of l1 code with the begining of l2 code
		tac = tac->next;
	tac->next = l2;
	l2->prev = tac;

	return l1;
}	  
	

