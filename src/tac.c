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
	if(l2==NULL)	//there's only TACs on l2
		return l1;
	
	//assumes that l1->prev and l2->prev are by default NULL
	tac = l1;
	while (tac->next != NULL) { //concat the very ending of l1 code with the begining of l2 code
		//printf("To andando pro next.");
		tac = tac->next;
	}
	tac->next = l2;
	l2->prev = tac;

	return l1;
}

// TAC* revert_tac (TAC *list)
// {
// 	TAC* aux = list;
// 	TAC* temp = NULL;
// 
// 	if (list == NULL)
// 		return NULL;
// 	
// 	while (aux != NULL)
// 	{
// 		temp = aux->prev;
// 		aux->prev = aux->next;
// 		aux->next = temp;
// 		aux = aux->prev;
// 	}
// 	
// 	list = aux->prev;
// 
// 	return list;	
// 	
// }

void print_tac (TAC *list)
{
	TAC* aux = list;

	if (list == NULL)
		return;

	//printf ("\nTAC: \n");
	while (aux != NULL) {
		switch (aux->type) {
			case TAC_NOP:
				printf("NOP \n"); break;
			case TAC_ADD:
				printf("ADD %s, %s => %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_SUB:
				printf("SUB %s, %s => %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_MUL:
				printf("MUL %s, %s => %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_DIV:
				printf("DIV %s, %s => %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_ADDI:
				printf("ADDI \n"); break;
			case TAC_SUBI:
				printf("SUBI \n"); break;
			case TAC_RSUBI:
				printf("RSUBI \n"); break;
			case TAC_MULTI:
				printf("MULTI \n"); break;
			case TAC_DIVI:
				printf("DIVI \n"); break;
			case TAC_RDIVI:
				printf("RDIVI \n"); break;
			case TAC_LSHIFT:
				printf("LSHIFT \n"); break;
			case TAC_LSHIFTI:
				printf("LSHIFTI \n"); break;
			case TAC_RSHIFT:
				printf("RSHIFT \n"); break;
			case TAC_RSHIFTI:
				printf("RSHIFTI \n"); break;
			case TAC_AND:
				printf("AND \n"); break;
			case TAC_ANDI:
				printf("ANDI \n"); break;
			case TAC_OR:
				printf("OR \n"); break;
			case TAC_ORI:
				printf("ORI \n"); break;
			case TAC_XOR:
				printf("XOR \n"); break;
			case TAC_XORI:
				printf("XORI \n"); break;
			case TAC_LOADI:
				printf("LOADI %d => %s\n", *((int*) aux->op1), aux->res); break;
			case TAC_LOAD:
				printf("LOAD %s => %s\n", aux->op1, aux->res); break;
			case TAC_LOADAI:
				printf("LOADAI \n"); break;
			case TAC_LOADAO:
				printf("LOADAO \n"); break;
			case TAC_STORE:
				printf("STORE %s => %s\n", aux->res, aux->op1); break;
			case TAC_STOREAI:
				printf("STOREAI \n"); break;
			case TAC_STOREAO:
				printf("STOREAO \n"); break;
			case TAC_CSTORE:
				printf("CSTORE \n"); break;
			case TAC_CSTOREAI:
				printf("CSTOREAI \n"); break;
			case TAC_CSTOREAO:
				printf("CSTOREAO \n"); break;
			case TAC_I2I:
				printf("I2I \n"); break;
			case TAC_C2C:
				printf("C2C \n"); break;
			case TAC_C2I:
				printf("C2I \n"); break;
			case TAC_I2C:
				printf("I2C \n"); break;
			case TAC_JUMPI:
				printf("JUMPI -> %s\n", aux->res); break;
			case TAC_JUMP:
				printf("JUMP \n"); break;
			case TAC_CBR:
				printf("CBR %s -> %s, %s\n", aux->res, aux->op1, aux->op2); 
				break;
			case TAC_CMP_LT:
				printf("CMP_LT %s, %s -> %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_CMP_LE:
				printf("CMP_LE %s, %s -> %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_CMP_EQ:
				printf("CMP_EQ %s, %s -> %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_CMP_GE:
				printf("CMP_GE %s, %s -> %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_CMP_GT:
				printf("CMP_GT %s, %s -> %s\n", aux->op1, aux->op2, aux->res); break;
			case TAC_CMP_NE:
				printf("CMP_NE %s, %s -> %s\n", aux->op1, aux->op2, aux->res); break;
				
			case TAC_LABEL:
				printf("%s: \n", aux->res);
		}
		aux = aux->next;
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
	sprintf(rotName, "L%d", rotCnt++);
	
	return rotName;
}
