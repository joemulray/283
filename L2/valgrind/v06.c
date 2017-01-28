#include <stdio.h>

int foo (x)
int x;
 {
  if(x < 10)
   {
    printf("x is less than 10\n");
   }
  return x;
 }

int main()
 {
  int y=1;
  foo(y);
 }
