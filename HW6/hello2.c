
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void doit(){
	{

		if(fork() == 0){
			fork();
			printf("hello\n");	
			exit(0);
		}
	
	}
	return;

}

int main(){

doit();
printf("hello\n");
exit(0);

}
