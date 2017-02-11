/*
Joseph Mulray
HW2:10.10
Modify so takes optional 
command line argument infile
copy to standart output
*/

#include "csapp.h"
#include <stdio.h>
int main(int argc, char **argv){
int n;
rio_t rio;
char buf[MAXLINE];

//if the user passes an argument in
if(argc == 2){
 
  //take the fd from arg and open the file
  fd = open(argv[1], O_RDONLY, 0);

  //copy stdin to the file
  dup2(fd, STDIN_FILENO);

  //close the file
  close(fd);
	}

Rio_readinitb(&rio, STDIN_FILENO);
while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
Rio_writen(STDOUT_FILENO, buf, n);
}