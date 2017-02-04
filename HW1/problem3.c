/*
Joseph Mulray
CS 283: HW1
Problem 3:  Write a C program to find out
how many bits a variable of type long
int is.
*/


#include <stdio.h>

int main(int argc, char *argv[]){

  long int var, size;

  //add one to the memory address and subtract from the original location
  //will return number of bytes, 8 bytes for every bit
  size = 8 * ((long int)(&var + 1) - (long int)(&var));

  //print size of long int
  printf("%lu\n", size);

}
