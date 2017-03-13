#include "csapp.h"
int main(int argc, char ** argv){

	int i;
	
	for(i = 0; i < 2; i++){
		Fork();
	printf("Hello World!\n");
	exit(0);
	}
}
