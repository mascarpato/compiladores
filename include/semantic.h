#include "comp_tree.h"
#include "iks_ast.h"

#ifndef __SEMANTIC
#define __SEMANTIC

#define IKS_SUCCESS 0 //caso não houver nenhum tipo de erro

/* Verificação de declarações */
#define IKS_ERROR_UNDECLARED 1 //identificador não declarado
#define IKS_ERROR_DECLARED 2 //identificador já declarado
/* Uso correto de identificadores */
#define IKS_ERROR_VARIABLE 3 //identificador deve ser utilizado como variável
#define IKS_ERROR_VECTOR 4 //identificador deve ser utilizado como vetor
#define IKS_ERROR_FUNCTION 5 //identificador deve ser utilizado como função
/* Tipos e tamanho de dados */
#define IKS_ERROR_WRONG_TYPE 6 //tipos incompatíveis
#define IKS_ERROR_STRING_TO_X 7 //coerção impossível do tipo string
#define IKS_ERROR_CHAR_TO_X 8 //coerção impossível do tipo char
/* Argumentos e parâmetros */
#define IKS_ERROR_MISSING_ARGS 9 //faltam argumentos
#define IKS_ERROR_EXCESS_ARGS 10 //sobram argumentos
#define IKS_ERROR_WRONG_TYPE_ARGS 11 //argumentos incompatíveis
/* Verificação de tipos em comandos */
#define IKS_ERROR_WRONG_PAR_INPUT 12 //parâmetro não é identificador
#define IKS_ERROR_WRONG_PAR_OUTPUT 13 //parâmetro não é literal string ou expressão
#define IKS_ERROR_WRONG_PAR_RETURN 14 //parâmetro não é expressão compatível com tipo do retorno

//Erro fora da especificação
#define IKS_ERROR_INDEX_NOT_INT //índice do vetor não corresponde a um inteiro   TODO



int check_id_declr (DictItem *sym);
int check_id_isfunction (DictItem *sym);
int check_id_isvariable (DictItem *sym);
int check_id_isvector (DictItem *sym);
// int check_paramlist (ParamList *paramsDecl, ParamList *paramsCall);
int check_ListNode (ListNode *paramsDecl, ListNode *paramsCall);

#endif
