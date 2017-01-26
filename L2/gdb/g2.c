#include <stdio.h> 
#include <stdlib.h> 

int main () 
 {
  char * buf;
  int num;
  num = 1 << 31;
  buf = malloc(num) ;
  fgets (buf, 1024, stdin) ;
  printf ("%s\n", buf) ;
  return(1);
 }
