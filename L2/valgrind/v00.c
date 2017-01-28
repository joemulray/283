#include <stdlib.h>

void f ()
 {
  int *x;
  x = malloc(10 * sizeof(int));
  x[10] = 0;
  free(x);
 }

int main ()
 {
  f();
  return 0;
 }
