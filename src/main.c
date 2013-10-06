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

int yyerror (char *mensagem)
{
  fprintf (stderr, "IKS-parser: %s at line %d\n", mensagem, getLineNumber());
}

int main (int argc, char **argv)
{
  gv_init("saida.dot");
  int resultado = yyparse();
  gv_close();
  //dict_print();
  
  switch(resultado) {
	case IKS_SUCCESS: break;
		/* Verificação de declarações */
	case IKS_ERROR_UNDECLARED:
		printf("identificador não declarado\n"); break;
	case IKS_ERROR_DECLARED:
		printf("identificador já declarado\n"); break;
		/* Uso correto de identificadores */
	case IKS_ERROR_VARIABLE:
		printf("identificador deve ser utilizado como variável\n"); break;
	case IKS_ERROR_VECTOR:
		printf("identificador deve ser utilizado como vetor\n"); break;
	case IKS_ERROR_FUNCTION :
		printf("identificador deve ser utilizado como função\n"); break;
		/* Tipos e tamanho de dados */
	case IKS_ERROR_WRONG_TYPE: 
		printf("tipos incompatíveis\n"); break;
	case IKS_ERROR_STRING_TO_X:
		printf("coerção impossível do tipo string\n"); break;
	case IKS_ERROR_CHAR_TO_X:  
		printf("coerção impossível do tipo char\n"); break;
		/* Argumentos e parâmetros */
	case IKS_ERROR_MISSING_ARGS:
		printf("faltam argumentos\n"); break;
	case IKS_ERROR_EXCESS_ARGS: 
		printf("sobram argumentos\n"); break;
	case IKS_ERROR_WRONG_TYPE_ARGS: 
		printf("argumentos incompatíveis\n"); break;
		/* Verificação de tipos em comandos */
	case IKS_ERROR_WRONG_PAR_INPUT: 
		printf("parâmetro não é identificador\n"); break;
	case IKS_ERROR_WRONG_PAR_OUTPUT: 
		printf("parâmetro não é literal string ou expressão\n"); break;
	case IKS_ERROR_WRONG_PAR_RETURN: 
		printf("parâmetro não é expressão compatível com tipo do retorno\n"); break;
  }
  
  return resultado;
}

