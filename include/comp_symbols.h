#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <comp_list.h>


#ifndef __COMP_SYMBOLS
#define __COMP_SYMBOLS

//#include "semantic.h"

#define MASK_SYMTYPE_TYPE 7
#define SYMTYPE_UNDEF 0
#define SYMTYPE_INT 1
#define SYMTYPE_FLOAT 2
#define SYMTYPE_CHAR 3
#define SYMTYPE_STRING 4
#define SYMTYPE_BOOL 5

#define SYMTYPE_IDENTIFIER 8

#define SYMTYPE_VAR 1024
#define SYMTYPE_VEC 2048
#define SYMTYPE_FUN 4096
#define SYM_IS_LOCALSCOPE 8192

#define TRUE 1
#define FALSE 0

//Identificadores de Coersão
#define NO_COERSION -1

#define COER_INT_TO_BOOL 1
#define COER_INT_TO_FLOAT 2

#define COER_FLOAT_TO_INT 3
#define COER_FLOAT_TO_BOOL 4

#define COER_BOOL_TO_INT 5
#define COER_BOOL_TO_FLOAT 6

#define IKS_ERROR_STRING_TO_X 7 //coerção impossível do tipo string
#define IKS_ERROR_CHAR_TO_X 8 //coerção impossível do tipo char

/** Tipo INT de 4Bytes na linguagem IKS. */
typedef int32_t iks_int;
/** Tipo FLOAT de 8Bytes na linguagem IKS. */
typedef double iks_float;
/** Tipo CHAR de 1Byte na linguagem IKS. */
typedef char iks_char;
/** Tipo STRING de n*1Byte na linguagem IKS. */
typedef char* iks_string;
/** Tipo BOOLEAN de 1Byte na linguagem IKS. */
typedef int8_t iks_boolean;

	/** Couples all possible datatypes from the IKS language in this union.
	* 
	*/
typedef union SymValue_un {
	iks_int value_int; //! To store an integer
	iks_float value_float; //! To store a float
	iks_char value_char; //! To store a character
	iks_string value_string; //! To store a string
	iks_boolean value_bool; //! To store a boolean value (TRUE or FALSE)
} SymValue_t;

	/** Stores a symbol which may be used across the program
	*/
typedef struct Symbol_t_str {
	char *name; //! Symbol name
	int symType; //! Symbol type. Possible types are found above.
	int symAddr; //! Relative symbol address
	SymValue_t value;
	
	ListNode *params; // Parameters list (used if it's a function) or dimensions list (if it's a vector)
} Symbol_t;

/**
 * Evaluates which type should be infered from type1 and type2
 * @param type1 first type
 * @param type2 second type
 * @param newType1 new type of type1, if a coersion should ocurr
 * @param newType2 new type of type2, if a coersion should ocurr
 * 
 * @return type to be inferred.*/
int eval_infer(int type1, int type2, int *newType1, int *newType2);

int eval_atrib(int type1, int type2, int *newType2);

#endif
