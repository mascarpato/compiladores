#include "comp_symbols.h"

int eval_infer(int type1, int type2, int *newType1, int *newType2) {
	if (type1 == SYMTYPE_INT) {
		if (type2 == SYMTYPE_INT) { // (int, int) -> int
			if (newType1 != NULL) *newType1 = SYMTYPE_INT;
			if (newType2 != NULL) *newType2 = SYMTYPE_INT;
			return SYMTYPE_INT;
		} else if (type2 == SYMTYPE_FLOAT) { // (int, float) -> float
			if (newType1 != NULL) *newType1 = SYMTYPE_FLOAT;
			if (newType2 != NULL) *newType2 = SYMTYPE_FLOAT;
			return SYMTYPE_FLOAT;
		} else if (type2 == SYMTYPE_CHAR) { // (int, char) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_STRING) { // (int, string) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_BOOL) { // (int, boolean) -> int
			if (newType1 != NULL) *newType1 = SYMTYPE_INT;
			if (newType2 != NULL) *newType2 = SYMTYPE_INT;
			return SYMTYPE_INT;
		} else {
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		}
	} else if (type1 == SYMTYPE_FLOAT) {
		if (type2 == SYMTYPE_INT) { // (float, int) -> float
			if (newType1 != NULL) *newType1 = SYMTYPE_FLOAT;
			if (newType2 != NULL) *newType2 = SYMTYPE_FLOAT;
			return SYMTYPE_FLOAT;
		} else if (type2 == SYMTYPE_FLOAT) { // (float, float) -> float
			if (newType1 != NULL) *newType1 = SYMTYPE_FLOAT;
			if (newType2 != NULL) *newType2 = SYMTYPE_FLOAT;
			return SYMTYPE_FLOAT;
		} else if (type2 == SYMTYPE_CHAR) { // (float, char) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_STRING) { // (float, string) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_BOOL) { // (float, boolean) -> float
			if (newType1 != NULL) *newType1 = SYMTYPE_FLOAT;
			if (newType2 != NULL) *newType2 = SYMTYPE_FLOAT;
			return SYMTYPE_FLOAT;
		} else {
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		}
	} else if (type1 == SYMTYPE_CHAR) {
		if (type2 == SYMTYPE_INT) { // (char, int) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_FLOAT) { // (char, float) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_CHAR) { // (char, char) -> char
			if (newType1 != NULL) *newType1 = SYMTYPE_CHAR;
			if (newType2 != NULL) *newType2 = SYMTYPE_CHAR;
			return SYMTYPE_CHAR;
		} else if (type2 == SYMTYPE_STRING) { // (char, string) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_BOOL) { // (char, boolean) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else {
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		}
	} else if (type1 == SYMTYPE_STRING) {
		if (type2 == SYMTYPE_INT) { // (string, int) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_FLOAT) { // (string, float) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_CHAR) { // (string, char) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_STRING) { // (string, string) -> string
			if (newType1 != NULL)
				*newType1 = SYMTYPE_STRING;
			if (newType2 != NULL) *newType2 = SYMTYPE_STRING;
			return SYMTYPE_STRING;
		} else if (type2 == SYMTYPE_BOOL) { // (string, boolean) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else {
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		}
	} else if (type1 == SYMTYPE_BOOL) {
		if (type2 == SYMTYPE_INT) { // (boolean, int) -> int
			if (newType1 != NULL) *newType1 = SYMTYPE_INT;
			if (newType2 != NULL) *newType2 = SYMTYPE_INT;
			return SYMTYPE_INT;
		} else if (type2 == SYMTYPE_FLOAT) { // (boolean, float) -> float
			if (newType1 != NULL) *newType1 = SYMTYPE_FLOAT;
			if (newType2 != NULL) *newType2 = SYMTYPE_FLOAT;
			return SYMTYPE_FLOAT;
		} else if (type2 == SYMTYPE_CHAR) { // (boolean, char) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_STRING) { // (boolean, string) -> ERROR
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		} else if (type2 == SYMTYPE_BOOL) { // (boolean, boolean) -> boolean
			if (newType1 != NULL) *newType1 = SYMTYPE_BOOL;
			if (newType2 != NULL) *newType2 = SYMTYPE_BOOL;
			return SYMTYPE_BOOL;
		} else {
			if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
			if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
			return SYMTYPE_UNDEF;
		}
	} else {
		if (newType1 != NULL) *newType1 = SYMTYPE_UNDEF;
		if (newType2 != NULL) *newType2 = SYMTYPE_UNDEF;
		return SYMTYPE_UNDEF;
	}
}

int eval_atrib(int type1, int *type2){
	int aux;
	eval_infer(type1,*type2, NULL, &aux);
	