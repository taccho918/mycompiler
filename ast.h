#include <stdio.h>
#include <stdlib.h>
#include "parse.tab.h"

typedef enum{
  N_IDENTIFIER,
  N_DEFINE,
  N_ARRAY,
  N_NUM,
  N_ASSIGN,
  N_PLUS,
  N_MINUS,
  N_DIV,
  N_MUL,
  N_FOR,
  N_WHILE,
  N_IF,
  N_ELSE,
  N_EQUAL,
  N_INCREMENT,
  N_DECREMENT,
  N_LESSEQUAL,
  N_PROGRAM,
  N_VARDEC,
  N_STATELIST,
  N_LESS,
  N_GREATER,
  N_ARRAYNODE,
  N_GREATEREQUAL,
}NType;


typedef struct node{
  int value;
  char* variable;
  struct node *child;
  struct node *brother;
  NType type;
}Node;

typedef struct symbols{
  int symno;
  char *symbolname;
  struct symbols* next;
}Symbols;

