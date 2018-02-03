#include <string.h>
#include "ast.h"
#include "parse.tab.h"

#define MAXBUF 100

Node* build_node1(NType t, Node* p1){
  Node *p;
  if ((p = (Node *)malloc(sizeof(Node))) == NULL) {
    yyerror("out of memory");
  }
  p->type = t;
  p->child = p1;
  p1->brother = NULL;
  p->brother = NULL;

  return p;
}

Node* build_node2(NType t, Node* p1, Node* p2) {
  Node *p;

  if ((p = (Node *)malloc(sizeof(Node))) == NULL) {
    yyerror("out of memory");
  }

  p->type = t;
  p->child = p1;
  p1->brother = p2;
  p->brother = NULL;

  return p;
}

Node* build_node3(NType t, Node* p1, Node* p2, Node* p3) {
  Node *p;
  if ((p = (Node *)malloc(sizeof(Node))) == NULL) {
    yyerror("out of memory");
  }
  
  p->type = t;
  p->child=p1;
  p1->brother = p2;
  p2->brother = p3;
  p->brother = NULL;

  return p;
}

Node* build_node4(NType t, Node* p1, Node* p2, Node* p3, Node* p4) {
  Node *p;
  if ((p = (Node *)malloc(sizeof(Node))) == NULL) {
    yyerror("out of memory");
  }

  
  p->type = t;
  p->child=p1;
  p1->brother = p2;
  p2->brother = p3;
  p3->brother = p4;
  p->brother = NULL;

  return p;
}

Node* build_num_node(NType t, int n){
  Node *p;
  if((p=(Node *)malloc(sizeof(Node)))==NULL){
    yyerror("out of memory");
  }
 
  p->type=t;
  p->value =n;
  p->child =NULL;
  p->brother=NULL;
 
  return p;
}

Node* build_ident_node(NType t, char *s){
  Node *p;
  if((p=(Node *)malloc(sizeof(Node)))==NULL){
      yyerror("out of memory");
  }
  p->type=t;

  if((p->variable = (char *)malloc(sizeof(MAXBUF)))==NULL){
    yyerror("out of memory");
  }
  
  strncpy(p->variable, s, MAXBUF);
  p->child =NULL;
  p->brother = NULL;
      
  return p;
}

Node* build_array_node_num(NType t,char *s,int n) {
  Node *p;
  if((p=(Node *)malloc(sizeof(Node)))==NULL){
      yyerror("out of memory");
  }
  p->type=t;

  if((p->variable = (char *)malloc(sizeof(MAXBUF)))==NULL){
    yyerror("out of memory");
  }
  (p->child)->value =n;
  strncpy(p->variable, s, MAXBUF);
  p->brother=NULL;

  return p;
}

Node* build_array_node_ident(NType t,char *s1, char *s2) {
  Node *p;
  if((p=(Node *)malloc(sizeof(Node)))==NULL){
      yyerror("out of memory");
  }
  p->type=t;

  if((p->variable = (char *)malloc(sizeof(MAXBUF)))==NULL){
    yyerror("out of memory");
  }
  
  strncpy(p->variable, s1, MAXBUF);
  strncpy((p->child)->variable, s2, MAXBUF);
  
  p->brother=NULL;

  return p;
}

Node* build_array_node_num2(NType t,char *s,int n1,int n2) {
  Node *p;
  if((p=(Node *)malloc(sizeof(Node)))==NULL){
      yyerror("out of memory");
  }
  p->type=t;

  if((p->variable = (char *)malloc(sizeof(MAXBUF)))==NULL){
    yyerror("out of memory");
  }
  (p->child)->value =n1;
  ((p->child)->brother)->value =n2;
  strncpy(p->variable, s, MAXBUF);
  p->brother=NULL;

  return p;
}

Node* build_array_node_ident2(NType t,char *s1, char *s2, char *s3) {
  Node *p;
  if((p=(Node *)malloc(sizeof(Node)))==NULL){
      yyerror("out of memory");
  }
  p->type=t;

  if((p->variable = (char *)malloc(sizeof(MAXBUF)))==NULL){
    yyerror("out of memory");
  }
  
  strncpy(p->variable, s1, MAXBUF);
  strncpy((p->child)->variable, s2, MAXBUF);
  strncpy(((p->child)->brother)->variable, s3, MAXBUF);
  p->brother=NULL;

  return p;
}
