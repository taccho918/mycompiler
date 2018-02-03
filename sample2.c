#include<stdio.h>
int main() {
  int i;
  int fact;

  fact=1;
  i=1;
  while(i<6) {
    fact = fact*i;
    i=i+1;
  }
  printf("fact: %d\n", fact);
}
