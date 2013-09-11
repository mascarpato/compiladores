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

int yyerror (char *mensagem)
{
  fprintf (stderr, "IKS-parser: %s at line %d\n", mensagem, getLineNumber());
}

int main (int argc, char **argv)
{
  gv_init(NULL);
  int resultado = yyparse();
  gv_close();
  return resultado;
}

