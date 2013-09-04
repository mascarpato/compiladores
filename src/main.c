/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 */

/*
   main.c

   Arquivo principal do analisador sintÃ¡tico.
*/

#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"

int yyerror (char *mensagem)
{
  fprintf (stderr, "IKS-parser: %s at line %d\n", mensagem, getLineNumber());
}

/*void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem);
}*/

int main (int argc, char **argv)
{
  int resultado = yyparse();
  //dict_print();
  return resultado;
}

