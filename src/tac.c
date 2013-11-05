#include "tac.h"

int regCnt = 0;
int rotCnt = 0;

TAC* create_tac (int type, void *res, void *op1, void *op2)
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

TAC* revert_tac (TAC *list)
{
	TAC* aux = list;
	TAC* temp = NULL;

	if (list == NULL)
		return NULL;
	
	while (aux != NULL)
	{
		temp = aux->prev;
		aux->prev = aux->next;
		aux->next = temp;
		aux = aux->prev;
	}
	
	list = aux->prev;

	return list;	
	
}

void print_tac (TAC *list)
{
	TAC* aux = list;

	if (list == NULL)
		return NULL;

	printf ("\nTAC LIST: ");		
	
	while (aux != NULL)
	{
		printf ("%d - ",aux->type); 
	}
	printf ("\n");
	
	
}	  
	
char *geraTemp() {
	char *regName = malloc(sizeof(char) * 7);
	sprintf(regName, "r%d", regCnt++);
	
	return regName;
}

char *geraRot() {
	char *rotName = malloc(sizeof(char) * 7);
	sprintf(rotName, "L%d", regCnt++);
	
	return rotName;
}
