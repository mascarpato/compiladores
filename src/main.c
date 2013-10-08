/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 */

/*
   main.c

   Arquivo principal do analisador sintÃ¡tico.
*/
#include "main.h"

extern struct treeNode_t *ast;

int yyerror (char *mensagem) {
  fprintf (stderr, "IKS-parser: %s at line %d\n", mensagem, getLineNumber());
}

int sserror(int errCode, DictItem *symEntry) {
	switch(errCode) {
			/* Verificação de declarações */
		case IKS_ERROR_UNDECLARED:
			printf("IKS-ssa: Identificador ' %s ' @L:%d não declarado. \n", symEntry->symbol.name, symEntry->occLine); break;
		case IKS_ERROR_DECLARED:
			printf("IKS-ssa: Redefinindo identificador ' %s ' (definido originalmente @L:%d). \n", symEntry->symbol.name, symEntry->occLine); break;
			
			/* Uso correto de identificadores */
		case IKS_ERROR_VARIABLE:
			printf("IKS-ssa: Identificador ' %s ' deve ser utilizado como variável. \n", symEntry->symbol.name); break;
		case IKS_ERROR_VECTOR:
			printf("IKS-ssa: Identificador ' %s ' deve ser utilizado como vetor.\n", symEntry->symbol.name); break;
		case IKS_ERROR_FUNCTION :
			printf("IKS-ssa: Identificador ' %s ' deve ser utilizado como função.\n", symEntry->symbol.name); break;
			
			/* Tipos e tamanho de dados */
		case IKS_ERROR_WRONG_TYPE: 
			printf("IKS-ssa: Tipos incompatíveis.\n"); break;
		case IKS_ERROR_STRING_TO_X:
			printf("IKS-ssa: Coerção impossível do tipo string.\n"); break;
		case IKS_ERROR_CHAR_TO_X:  
			printf("IKS-ssa: Coerção impossível do tipo char.\n"); break;
			
			/* Argumentos e parâmetros */
		case IKS_ERROR_MISSING_ARGS:
			printf("IKS-ssa: Faltam argumentos para funcao.\n"); break;
		case IKS_ERROR_EXCESS_ARGS: 
			printf("IKS-ssa: Excesso de argumentos para funcao. \n"); break;
		case IKS_ERROR_WRONG_TYPE_ARGS: 
			printf("IKS-ssa: Argumentos incompatíveis para funcao. \n"); break;
			
			/* Verificação de tipos em comandos */
		case IKS_ERROR_WRONG_PAR_INPUT: 
			printf("IKS-ssa: Parâmetro no comando 'input' não é identificador. \n"); break;
		case IKS_ERROR_WRONG_PAR_OUTPUT: 
			printf("IKS-ssa: Parâmetro no comando 'output' não é literal string ou expressão.\n"); break;
		case IKS_ERROR_WRONG_PAR_RETURN: 
			printf("IKS-ssa: Parâmetro no comando 'return' não é expressão compatível com tipo do retorno. \n"); break;
	}
}

int main (int argc, char **argv)
{
  gv_init("saida.dot");
  int resultado = yyparse();
  gv_close();
  //dict_print();
  
  return resultado;
}

