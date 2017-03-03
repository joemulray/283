/*
Joseph Mulray
CS 283: H5
CSAPP3 Problem 12.16
*/


#include "csapp.h"

void *thread(void *vargp);

int main(int argc, char ** argv){

	//variable declaration
	int number, count;
	number = atoi(argv[1]);
	pthread_t tid;
	void* return_value;

	/*create and join threads until count is greater 
	than the number passed in by an argument*/
	for (count = 0; count < number; count++){
		pthread_create(&tid, NULL, thread, NULL);
		pthread_join(tid, return_value);
	}

	exit(0);
}


void *thread(void *vargp){
	//thread function
	printf("Hello, World!\n");
	return NULL;
}
