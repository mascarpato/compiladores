/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maur�cio Altieri.
 */

/*
   main.c

   Arquivo principal do analisador sintático.
*/
#include "main.h"

int yyerror (char *mensagem)
{
  fprintf (stderr, "IKS-parser: %s at line %d\n", mensagem, getLineNumber());
}

int main (int argc, char **argv)
{
  //gv_init(NULL);
  int resultado = yyparse();
  //gv_close();
  return resultado;
}

