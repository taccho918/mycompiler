#include "ast.h"
#include "parse.tab.h"

extern int yylex();
extern int yyerror();
extern id_count;
extern char*id_table[100];

Node *parse_result = NULL;

int main(void)
{
  int result;
  
  result = yyparse();

  printf("INITIAL_GP = 0x10008000\n");
  printf("INITIAL_SP = 0x7ffffffc\n");
  printf("# system call service number\n");
  printf("stop_service = 99\n");

  printf("\t.text\n");
  printf("init:\n");
  printf("# initialize $gp (global pointer) and $sp (stack pointer)\n");
  printf("la  $gp, INITIAL_GP    # $gp <- 0x10008000 (INITIAL_GP)\n");
  printf("la  $sp, INITIAL_SP    # $sp <- 0x7ffffffc (INITIAL_SP)\n");
  printf("jal main              # jump to 'main'\n");
  printf("syscall\n");
  printf("nop                   #  (delay slot)\n");
  printf("li  $v0, stop_service # $v0 <- 99 (stop_service)\n");
  printf("nop                   \n");
  printf("# not reach here\n");
  
  printf("stop:                  # if syscall return\n");
  printf("j stop                 # infinite loop\n");
  printf("nop\n                  #  (delay slot)");

  printf(".text  0x00001000\n");

  if(!result) {
    printf("main: \n");
    printf("\taddi $sp, $sp, -32\n");
    printf("\tsw $ra, 20($sp)\n");
    printf("\tsw $fp, 16($sp)\n");
    printf("\taddiu $fp, $sp, 28\n\n");
    printNodes(parse_result);
  }
  int i;
  printf("\n\tlw $ra, 20($sp)\n");
  printf("\tlw $fp, 16($sp)\n");
  printf("\taddi $sp, $sp, -32\n");
  printf("\tjr $ra\n");
}
