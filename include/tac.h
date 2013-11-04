#include <stdio.h>
#include "comp_dict.h"

#ifndef COMP_TAC_H
#define COMP_TAC_H

//Instruction opcodes
#define TAC_NOP 0
#define TAC_ADD 1
#define TAC_SUB 2
#define TAC_MUL 3
#define TAC_DIV 4
#define TAC_ADDI 5
#define TAC_SUBI 6
#define TAC_RSUBI 7
#define TAC_MULTI 8
#define TAC_DIVI 9
#define TAC_RDIVI 10
#define TAC_LSHIFT 11
#define TAC_LSHIFTI 12
#define TAC_RSHIFT 13
#define TAC_RSHIFTI 14
#define TAC_AND 15
#define TAC_ANDI 16
#define TAC_OR 17
#define TAC_ORI 18
#define TAC_XOR 19
#define TAC_XORI 20
#define TAC_LOADI 21
#define TAC_LOAD 22
#define TAC_LOADAI 23
#define TAC_LOADAO 24
#define TAC_STORE 25
#define TAC_STOREAI 26
#define TAC_STOREAO 27
#define TAC_CSTORE 28
#define TAC_CSTOREAI 29
#define TAC_CSTOREAO 30
#define TAC_I2I 31
#define TAC_C2C 32
#define TAC_C2I 33
#define TAC_I2C 34
#define TAC_JUMPI 35
#define TAC_JUMP 36
#define TAC_CBR 37
#define TAC_CMP_LT 38
#define TAC_CMP_LE 39
#define TAC_CMP_EQ 40
#define TAC_CMP_GE 41
#define TAC_CMP_GT 42
#define TAC_CMP_NE 43

// Other TAC types
#define TAC_SYMBOL 256
//#define TAC_BLABLABLA 257

// Doubly linked list
typedef struct tacstruct
{
    int type;
    void *res, *op1, *op2;
    struct tacstruct *prev, *next;
} TAC;

//TAC operations
TAC* create_tac (int type, DictItem *res, DictItem *op1, DictItem *op2);
TAC* join_tac (TAC *l1, TAC *l2);

char *geraTemp();

#endif
