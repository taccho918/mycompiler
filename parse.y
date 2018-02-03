%{
#include <stdio.h>
#include "parse.tab.h"
#include "ast.h"
  extern int yylex();
  extern int yyerror();
  extern char yytext[];
  extern Node* parse_result; 
%}

%union {
  char *id;
  int num;
  struct Node* node;
}

%token <num> NUM
%token <id> IDENTIFIER
%token DEFINE
%token ARRAY
%token WHILE
%token IF
%token ELSE
%token EQUAL
%token FOR
%token INCREMENT
%token DECREMENT
%token LESSEQUAL
%token GREATEREQUAL
%token PLUS
%token MINUS
%token MUL
%token DIV
%token ASSIGN
%token LESS
%token GREATER
%type <node> program var_declaration declaration statement_list statement assignment loop conditional_branch condition expression term factor variable 
%%

program: var_declaration statement_list { parse_result = build_node2(N_PROGRAM, $1, $2);}
;

var_declaration : declaration var_declaration { $$ = build_node2(N_VARDEC, $1, $2);}
| declaration;

declaration: DEFINE IDENTIFIER';' { $$ = build_node1(N_DEFINE, build_ident_node(N_IDENTIFIER, $2));}
| ARRAY IDENTIFIER '['NUM']'';' { $$ = build_node2(N_ARRAY,build_ident_node(N_IDENTIFIER, $2) , build_num_node(N_NUM, $4)) ; }
| ARRAY IDENTIFIER '['NUM']''['NUM']'';'{/* $$ = build_node3(N_ARRAY,build_ident_node(N_IDENTIFIER, $2) , build_num_node(N_NUM, $4) ,build_num_node(N_NUM, $7)) ; */}
| ARRAY IDENTIFIER '['IDENTIFIER']'';' { $$ = build_node2(N_ARRAY,build_ident_node(N_IDENTIFIER, $2) , build_ident_node(N_IDENTIFIER, $4)) ; }; 

statement_list: statement statement_list { $$ = build_node2(N_STATELIST, $1, $2); }
| statement;

statement: assignment | loop | conditional_branch;

assignment: IDENTIFIER ASSIGN expression';' { $$ = build_node2(N_ASSIGN, build_ident_node(N_IDENTIFIER, $1), $3);}

| expression INCREMENT { $$ = build_node1(N_INCREMENT,$1);}

| expression DECREMENT { $$ = build_node1(N_DECREMENT,$1);}

| IDENTIFIER'['NUM']' ASSIGN expression';' { $$ = build_node3(N_ASSIGN,build_ident_node(N_IDENTIFIER, $1) , build_num_node(N_NUM, $3),$6);}

| IDENTIFIER'['IDENTIFIER']' ASSIGN expression';' { $$ = build_node3(N_ASSIGN,build_ident_node(N_IDENTIFIER, $1) , build_ident_node(N_IDENTIFIER, $3),$6);}

| IDENTIFIER'['IDENTIFIER']''['IDENTIFIER']' ASSIGN expression';'{ /*$$ = build_node4(N_ASSIGN,build_ident_node(N_IDENTIFIER, $1) , build_ident_node(N_IDENTIFIER, $3), build_ident_node(N_IDENTIFIER, $6),$9);*/};

| IDENTIFIER'['NUM']''['NUM']' ASSIGN expression';'{ /*$$ = build_node4(N_ASSIGN,build_ident_node(N_IDENTIFIER, $1) , build_num_node(N_NUM, $3), build_num_node(N_NUM, $6),$9);*/};

expression: expression PLUS term  { $$ = build_node2(N_PLUS, $1, $3);}
| expression DECREMENT { $$ = build_node1(N_DECREMENT,$1);}
| term;  

term: term MUL factor { $$ = build_node2(N_MUL, $1, $3);} 
|term DIV factor { $$ = build_node2(N_DIV, $1, $3);} 
| factor
;

factor: variable 
| '('expression')' { $$ = $2; }

variable: IDENTIFIER { $$ = build_ident_node(N_IDENTIFIER, $1);} 
| NUM { $$ = build_num_node(N_NUM, $1);} 
| IDENTIFIER'['NUM']'{$$ = build_array_node_num(N_ARRAYNODE,build_ident_node(N_IDENTIFIER, $1), build_num_node(N_NUM, $3));} 
| IDENTIFIER'['IDENTIFIER']' {$$ = build_array_node_ident(N_ARRAYNODE,build_ident_node(N_IDENTIFIER, $1), build_ident_node(N_IDENTIFIER, $3));}

| IDENTIFIER'['NUM']''['NUM']' {/*$$ = build_array_node_num2(N_ARRAYNODE,build_ident_node(N_IDENTIFIER, $1), build_num_node(N_NUM, $3), build_num_node(N_NUM, $6));*/} 
| IDENTIFIER'['NUM']''['IDENTIFIER']' 
| IDENTIFIER'['IDENTIFIER']''['IDENTIFIER']' {/*$$ = build_array_node_ident2(N_ARRAYNODE,build_ident_node(N_IDENTIFIER, $1), build_ident_node(N_IDENTIFIER, $3), build_ident_node(N_IDENTIFIER, $6));*/}
| IDENTIFIER'['IDENTIFIER']''['NUM']';

loop: WHILE '('condition')''{'statement_list'}' { $$ = build_node2(N_WHILE, $3, $6); }
| FOR '('assignment condition';'condition')''{'statement_list'}' {$$ = build_node4(N_FOR, $3, $4, $6, $9);}
;

conditional_branch: IF '('condition')''{'statement_list'}' { $$ = build_node2(N_IF, $3, $6);}
| IF '('condition')''{'statement_list'}' ELSE '{'statement_list'}' { $$ = build_node3(N_ELSE, $3, $6, $10);}
;

condition: expression EQUAL expression { $$ = build_node2(N_EQUAL, $1, $3);}
| expression LESS expression { $$ = build_node2(N_LESS, $1, $3);}
| expression GREATER expression { $$ = build_node2(N_GREATER, $1, $3);}
| expression LESSEQUAL expression { $$ = build_node2(N_LESSEQUAL, $1, $3);}
| expression GREATEREQUAL expression { $$ = build_node2(N_GREATEREQUAL, $1, $3);}
;

%%

