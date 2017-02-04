/*
Joseph Mulray
CS 283: HW1
Problem 2: Write a C program to convert
 a decimal number to binary in 32 bits
*/

#include <stdio.h>
#include <stdlib.h>
#define SIZE 128

int main(int argc, char *argv[]){
  int count, op, num;

  //read in the argument, convert to long int
  long int arg1 = atol(argv[1]);

  //convert to 32 bits, cycle
  for (count = 31; count >= 0; count--)
    {
      //bitshift, shift count to the right by arg bits
      op = arg1 >> count;

      //use and operator to check if op, equal to 1,
      num = op &1;

      //if op bit is equal equals 1 or 0: print
      if (num == 1)
      {
        printf("1");
      }
      else
      {
      printf("0");
      }
    }

    //close the file
    printf("\n");

}
