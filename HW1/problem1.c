/*
Joseph Mulray
CS 283: HW1
Problem 1: Write a C program to print the
contents of a text file line-by-line but
backwards so that the last line is printed first.
*/

#include <stdio.h>
#include <stdlib.h>
#define SIZE 128

int main(int argc, char *argv[]) {
  int n, num, count;
  char line[SIZE];

  //open file create file pointer
  FILE * fPointer;
  fPointer = fopen(argv[1], "r+");

  //move the pointer to the end of the file
  fseek(fPointer, 0, SEEK_END);

  //ftell returns the current file position
  // while > 1 keep interating through
  while (ftell(fPointer) > 1)
  {

    //move back to the next byte
    fseek(fPointer, -2, SEEK_CUR);

    //get the value of current position
    n = fgetc(fPointer);
    count = 0;

    //check if position is end of file
    while (n != '\n')
     {
       //if end of line, add to line and keep track of
       //number of characters
      line[count++] = n;
      if (ftell(fPointer) < 2)
        break;
      fseek(fPointer, -2, SEEK_CUR);
      n = fgetc(fPointer);
     }

    count--;
    //cycle through the char array, printing the current line
    for (num = count; num >= 0; num--)
    printf("%c", line[num]);
    printf("\n");

  }
  //closet the file
  fclose(fPointer);
}
