

/**********************************/
/* 二分探索木　解答例　　　　　　     */
/**********************************/

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int id_count = 0;
char *id_table[100];
int label_count=0;
int loop_flg=0;

/* Node の作成*/
Node *
newNode(int n) {
  Node *obj;
  obj = (Node *)malloc(sizeof(Node));
  obj->value = n;
  obj->child = NULL;
  obj->brother = NULL;
  return obj;
}

/* Node の追加*/
Node *
addNode(Node *obj, int n) {
  /* obj が NULL の場合*/
  if (obj == NULL) {
    return newNode(n);
  }

  /* 追加する場所を探して追加 */
  if (obj->value == n) { /* obj->value == n の時は追加しない*/
    /* do nothing */
  } else if(obj->value < n) {
    obj->brother = addNode(obj->brother, n);
  } else { /* obj->value > n の時*/
    obj->child = addNode(obj->child, n);
  }
    
  return obj;
} /* end of addNode */

/* Nodes を一番右に追加*/
Node *
appendRightEnd(Node *obj, Node *brother) {
  if (obj != NULL) {
    if (obj->brother == NULL) {
      obj->brother = brother;
    } else {
      obj->brother = appendRightEnd(obj->brother, brother);
    }
  }
  return obj;
}

/* Node の削除 */
Node *
deleteNode(Node *obj, int n) {
  Node *newobj;

  if (obj != NULL) {
    if (obj->value == n) {

      if (obj->child == NULL) {
        newobj = obj->brother;
        free(obj);
        return newobj;
      } else if(obj->brother == NULL) {
        newobj = obj->child;
        free(obj);
        return newobj;
      } else {
        /* 右の子供たちのつけかえ */
        obj->child = appendRightEnd(obj->child, obj->brother);
        newobj = obj->child;
        free(obj);
        return newobj;
      }
    } else {
      if (obj->value < n)
        obj->brother = deleteNode(obj->brother, n);
      else
        obj->child = deleteNode(obj->child, n);
      }
  }
  return obj;
}

int assign_cnt=0;
int i;

/* Node の表示*/
void 
printNodes(Node *obj){
  if (obj != NULL) {

    if (obj->type == N_NUM) {
       printf("\tli $t2, %d\n", obj->value);
       printf("\taddi $sp, $sp, -4\n");
       printf("\tsw $t2, 0($sp)\n");
    } else if (obj->type == N_ASSIGN) {
      printNodes(obj->child->brother);
       for (i=0; i<id_count; i++) {
         if (strcmp(id_table[i], obj->child->variable) == 0){
           break;
         }
       }
       printf("\tlw $v0, 0($sp)\n"); 
       printf("\taddi $sp, $sp, 4\n");
       printf("\tsw $v0, %d($fp)\n",i*4);
    } 
 
    switch(obj->type){
      case N_IDENTIFIER:
       for (i=0; i<id_count; i++) {
         if (strcmp(id_table[i], obj->variable) == 0){
           break;
         }
       }
       printf("\tlw $v0, %d($fp)\n",i*4); 
       printf("\taddi $sp, $sp, -4\n");
       printf("\tsw $v0, 0($sp)\n");
        break;
    
     case N_DEFINE:
       id_table[id_count] = obj->child->variable;
       id_count++;
       break;
      
     case N_ARRAY:
       printf("ARRAY\n");
       if (obj->child != NULL) {
         printNodes(obj->child);
       }
       if (obj->brother != NULL) {
         printNodes(obj->brother);
       }
       break;

    case N_PLUS:
     if(obj->child->brother != NULL){
       printNodes(obj->child->brother);
      }
     if (obj->child != NULL) {
        printNodes(obj->child);
        printf("\tlw $v0, 0($sp)\n"); 
        printf("\taddi $sp, $sp, 4\n");
        printf("\tlw $v1, 0($sp)\n"); 
        printf("\taddi $sp, $sp, 4\n");
        printf("\taddu $v0, $v0, $v1\n");
        printf("\taddi $sp, $sp, -4\n");
        printf("\tsw $v0, 0($sp)\n");
     }
     break;
     
     case N_MINUS:
     if(obj->child->brother != NULL){
       printNodes(obj->child->brother);
     }
     if (obj->child != NULL) {
       printNodes(obj->child);
       printf("\tlw $v0, 0($sp)\n"); 
       printf("\taddi $sp, $sp, 4\n");
       printf("\tlw $v1, 0($sp)\n"); 
       printf("\taddi $sp, $sp, 4\n"); 
       printf("\tsub $v0, $v0, $v1\n");
       printf("\taddi $sp, $sp, -4\n");
       printf("\tsw $v0, 0($sp)\n");
     }
     break;
     
    case N_DIV:
      if(obj->child->brother != NULL){
        printNodes(obj->child->brother);
      }
      if (obj->child != NULL) {
        printNodes(obj->child);
        printf("\tlw $v0, 0($sp)\n"); 
        printf("\taddi $sp, $sp, 4\n");
        printf("\tlw $v1, 0($sp)\n"); 
        printf("\taddi $sp, $sp, 4\n"); 
        printf("\tdiv $v0, $v1\n");
        printf("\tmflo $v0\n");
        printf("\taddi $sp, $sp, -4\n");
        printf("\tsw $v0, 0($sp)\n");
      }
      break;
      
    case N_MUL:
      if(obj->child->brother != NULL){
        printNodes(obj->child->brother);
      }
      if (obj->child != NULL) {
        printNodes(obj->child);
        printf("\tlw $v0, 0($sp)\n"); 
        printf("\taddi $sp, $sp, 4\n");
        printf("\tlw $v1, 0($sp)\n"); 
        printf("\taddi $sp, $sp, 4\n"); 
        printf("\tmult $v0, $v1\n");
        printf("\tmflo $v0\n");
        printf("\taddi $sp, $sp, -4\n");
        printf("\tsw $v0, 0($sp)\n");
	  }
      break;

    case N_FOR:
      printf("FOR\n");
      if (obj->child != NULL) {
        printNodes(obj->child);
      }
      if (obj->brother != NULL) {
        printNodes(obj->brother);
      }
      break;
      
    case N_WHILE:
      loop_flg = 0;
      int j;
      if (obj->child != NULL) {
        printf("L%d:\n", label_count);
        j=label_count++;
        printNodes(obj->child);
        if (obj->child->brother != NULL) 
          printNodes(obj->child->brother);
        printf("\tj L%d\n",j);
        printf("\tnop\n");
        printf("END_WHILE:\n");
        printf("\tjr $ra\n");
        printf("\tnop\n");
      }
      break;
      
    case N_IF:
      loop_flg = 1;
      if (obj->child != NULL) {
        printNodes(obj->child);
      }
      if (obj->child->brother != NULL) {
        printf("L%d:\n", label_count++);
        printNodes(obj->child->brother);
      }
      break;

    case N_ELSE:
      loop_flg=2;
      if (obj->child != NULL) {
        printNodes(obj->child);
      }
      if (obj->child->brother != NULL) {
        printf("L%d:\n", label_count++);
        printNodes(obj->child->brother);
        printf("\tj END_ELSE\n"); 
        printf("\tnop\n");
      }
      if (obj->child->brother->brother != NULL) {
        printf("L%d:\n", label_count++);
        printNodes(obj->child->brother->brother);
        printf("\tj END_ELSE\n");
         printf("\tnop\n");
      }
      printf("END_ELSE:\n");
      printf("\tjr $ra\n");
      printf("\tnop\n");
      break;
      
    case N_EQUAL:
      if (obj->child->brother != NULL) {
        printNodes(obj->child->brother);
      }
      if (obj->child != NULL) {
        if(loop_flg == 1) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, L%d\n", label_count);
          printf("\tnop\n");
        } else if (loop_flg == 2) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tj L%d\n", label_count+1);
          printf("\tnop\n");
        } else {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, END_WHILE\n");
          printf("\tnop\n");
        }
      }
      break;
      
    case N_INCREMENT:
      break;

    case N_DECREMENT:
      break;
 
    case N_LESSEQUAL:
        if (obj->child->brother != NULL) {
        printNodes(obj->child->brother);
      }
      if (obj->child != NULL) {
        if(loop_flg == 1) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tslt $t0, $v0, $v1\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
        } else if (loop_flg==2) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tslt $t0, $v0, $v1\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tj L%d\n", label_count+1);
          printf("\tnop\n");
        }else {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, END\n");
          printf("\tnop\n");
          printf("\tslt $t0, $v0, $v1\n");
          printf("\tbeq $t0, $zero, END_WHILE\n");
          printf("\tnop\n");
        }
      }
      break;

    case N_GREATEREQUAL:
       if (obj->child->brother != NULL) {
        printNodes(obj->child->brother);
      }
      if (obj->child != NULL) {
        if(loop_flg == 1) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tslt $t0, $v1, $v0\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
        } else if (loop_flg == 2) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tslt $t0, $v1, $v0\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tj L%d\n", label_count+1);
          printf("\tnop\n");
        }else {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tbeq $v0, $v1, END\n");
          printf("\tnop\n");
          printf("\tslt $t0, $v1, $v0\n");
          printf("\tbeq $t0, $zero, END_WHILE\n");
          printf("\tnop\n");
        }
      }
      break;
    
    case   N_PROGRAM:
      if (obj->child != NULL) {
      printNodes(obj->child);
      }
      if (obj->brother != NULL) {
        printNodes(obj->brother);
      }
    break;
    
    case N_VARDEC:
      if (obj->child != NULL) {
        printNodes(obj->child);
      }
      if (obj->brother != NULL) {
        printNodes(obj->brother);
      }
      break;
      
    case N_STATELIST:
      printNodes(obj->child);
      printNodes(obj->child->brother);
      break;
      
    case N_LESS: 
      if (obj->child->brother != NULL) {
        printNodes(obj->child->brother);
      }
      if (obj->child != NULL) {
        if(loop_flg == 1) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tslt $t0, $v0, $v1\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
        } else if (loop_flg == 2) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tslt $t0, $v0, $v1\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tj L%d\n", label_count+1);
          printf("\tnop\n");
        }else {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tslt $t0, $v0, $v1\n");
          printf("\tbeq $t0, $zero, END_WHILE\n");
          printf("\tnop\n");
        }
      }
      break;

    case N_GREATER:
      if (obj->child->brother != NULL) {
        printNodes(obj->child->brother);
      }
      if (obj->child != NULL) {
        if(loop_flg == 1) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tslt $t0, $v1, $v0\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
        } else if (loop_flg == 2) {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tslt $t0, $v1, $v0\n");
          printf("\tbeq $t0, $zero, L%d\n", label_count);
          printf("\tnop\n");
          printf("\tj L%d\n", label_count+1);
          printf("\tnop\n");
        } else {
          printNodes(obj->child);
          printf("\tlw $v0, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tlw $v1, 0($sp)\n"); 
          printf("\taddi $sp, $sp, 4\n");
          printf("\tslt $t0, $v1, $v0\n");
          printf("\tbeq $t0, $zero, END_WHILE\n");
          printf("\tnop\n");
        }
      }
      break;

    default:
      break;
    } 
  }
}

