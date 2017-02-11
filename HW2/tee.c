/*
Joseph Mulray
HW2: tee command
2/10/17
*/

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define SIZE 512
int main(int argc, char **argv){

 int fd;
 char buf[SIZE]; //Variables
 size_t len;

 //take file as command line argument
 fd = open(argv[1],O_WRONLY|O_APPEND);
 
 //if file exists get input, other wise pass
 if (fd >= 0){

  //keep getting input from stdin till EOF
  while(fgets(buf, SIZE, stdin) != NULL){
  	
  	puts(buf);
  	 //get the length of the buffer	
  	len = strlen(buf);
    //append to file  
  	write(fd, buf, len);	
 	
 	}
 	//close the file
	close(fd);
   } 
}
