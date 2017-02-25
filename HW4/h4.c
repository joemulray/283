	/*
Joseph Mulray
CS283
HW4: File I/O
*/

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#define SIZE 256

int main(int argc, char **argv){

	//variable declaration
	DIR *directory;
	struct dirent *de;
	char * ext;
	char *p1, *p2, *p3;
	char path[SIZE];
	char line[SIZE];
	FILE *fp, *temp;
	int isTrue1=0;
	int isTrue2=0;

	//used first argument name of directory
	directory = opendir(argv[1]);

	p1 = argv[2]; //first parameter
	p2 = argv[3]; //second
	p3 = argv[4]; //third



	printf("\nDirectory: %s\n", argv[1]);



	//cycle throught the directory given
	while (0 != (de = readdir(directory))){
		temp = fopen("temp.txt", "w");
		//check the file extenstions
		if((ext = strrchr(de->d_name, '.')) != NULL){

			//if extension is txt
			if(strcmp(ext, ".txt") == 0){
				printf("FILE: %s\n" , de->d_name);	
				path[0]= 0;

				//create path
				strcat(path, argv[1]);
				strcat(path, "/");
				strcat(path, de->d_name);

				fp = fopen(path, "r+");

				//read each line
				while(fgets(line, SIZE, fp) != NULL){
						
					if((strstr(line, p1)) != NULL) {
						isTrue1=1;
						char *position = strstr(line, p1);
						size_t newlen = strlen(line) - strlen(p1) + strlen(p2);

						//create a new line for file
						char totalLine[newlen + 1];
						 memcpy(totalLine, line, position - line);
						 memcpy(totalLine + (position - line), p2, strlen(p2));

						 strcpy(totalLine + (position - line) + strlen(p2), position + strlen(p1));
						 printf("Current Line: %s\nReplacement Line: %s\n", line, totalLine);
						 fprintf(temp, "%s", totalLine);	
						}

						else{
						//line doesnt exist in file just write line to new file
							fprintf(temp, "%s", line);
						}

				}

				fclose(temp);

				if (isTrue1 ==1){
					fclose(fp);
					//remove path and rename file to path
					remove(path);
					rename("temp.txt", path);
				}
				else{
 					//if file did not contain the paremeter run third parameter through the file
					printf("FILE: %s, did not contain parameter: %s\n", de->d_name, p1);
					printf("Checking other parameter: %s\n", p3);

					//reset the file
					rewind(fp);	
					temp = fopen("temp.txt", "w");

					while(fgets(line, SIZE, fp) != NULL){
						if((strstr(line, p3)) != NULL) {

						isTrue2=1;
						printf("Current Line: %s\n", line );
						char *position = strstr(line, p3);


						size_t testlen = strlen(line) + strlen(p1);
						char totalLine2[testlen + 1];
						memcpy(totalLine2, line, position - line);
						memcpy(totalLine2 + (position - line), p1, strlen(p1));
						strcpy(totalLine2 + (position-line) + strlen(p1), (line+2) + strlen(p3));
						printf("Replacement Line: %s\n", totalLine2);
						fprintf(temp, "%s", totalLine2);	
						}	
						else{
							fprintf(temp, "%s", line);	
						}
					}
					if(isTrue2==0){

						printf("FILE Contained neither argument 1, or 3.\n");
					}
					else{
						//close the file pointer and remove path, set new one
					fclose(fp);
					remove(path);
					rename("temp.txt", path);
					}
				}

				path[0]= 0;

			}
		}
	 }

	 //remove the temp file used.
	 remove("temp.txt");

	return 0;
}
