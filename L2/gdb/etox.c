#include <stdio.h>

double getvalue (double, int);
int factorial (int);

int main ()
 {
  int n;
  double x;
  double series;

  printf("This program calculates e^x\n");
  printf("using sum of (x^k)/k!\n");
  printf("Enter x, n : ");

  scanf("%lf%d",&x,&n);
  printf("x,n = %8.4lf %4d\n",x,n);
  
  series = getvalue(x,n);
  printf("e^x = %14.10lf\n",series);

  return(0);
 }

double getvalue (x,n)
double x;
int n;
 {
  int k;
  double value = 0.0;
  double xpow = 1.0;
  for (k = 0; k <= n; k++)
   {
    value += xpow / factorial(k);
    xpow = xpow * x;
   }
  return(value);
 }

int factorial (number)
int number;
 {
  int j;
  int fact = 0;

  for (j = 1; j <= number; j++)
   {
    fact = fact * j;
   }

  return(fact);
 }

