#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __COMP_SYMBOLS
#define __COMP_SYMBOLS

#define SYMTYPE_UNDEF 0
#define SYMTYPE_INT 1
#define SYMTYPE_FLOAT 2
#define SYMTYPE_CHAR 3
#define SYMTYPE_STRING 4
#define SYMTYPE_BOOL 5
#define SYMTYPE_IDENTIFIER 6

#define SYMTYPE_IDENTIFIER_DECLARED 7

#define TRUE 1
#define FALSE 0

	/** Used to handle boolean variables, which may take a 0 
	* (FALSE) or 1 (TRUE) value. */
typedef short Boolean;

	/** Couples all possible datatypes from the IKS language in this union.
	* 
	*/
typedef union SymValue_un {
	int value_int; //! To store an integer
	double value_float; //! To store a float
	char value_char; //! To store a character
	char *value_string; //! To store a string
	Boolean value_bool; //! To store a boolean value (TRUE or FALSE)
} SymValue_t;

	/** Stores a symbol which may be used across the program
	*/
typedef struct Symbol_t_str {
	char *name; //! Symbol name
	int symType; //! Symbol type. Possible types are found below.
	SymValue_t value;
} Symbol_t;

#endif
