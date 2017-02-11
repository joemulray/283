/*
Joseph Mulray
CS 283: L3 
File I/O Programming
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

#define SIZE 512

//Prototypes:
void removeFile(struct dirent *p, char *dir_name);
void replicateFile(struct dirent *p, char *dir_name, char*dir_name2);
void mostRecent(struct dirent *p1, char *dir_name, char*dir_name2);



int main(int argc, char **argv)
{

 int count1=0;
 int count2=0;
 int isFalse;

//Variable Declaration
DIR *directory1;
DIR *directory2;

struct dirent *de1;
struct dirent *de2;
struct dirent *pf1[SIZE];
struct dirent *pf2[SIZE];
char *dir_name1 = argv[1];
char *dir_name2 = argv[2];

//open the two dirs, passed as arguments
directory1 = opendir(dir_name1);
directory2 = opendir(dir_name2);



printf("\nDirectory: %s\n", dir_name1);
while (0 != (de1 = readdir(directory1))){
//ingore .. and . files
 if(!(strcmp(de1->d_name,".")==0 || strcmp(de1->d_name,"..")==0)){
 	printf("%s\n", de1->d_name);
 	pf1[count1] = de1;
 	count1 ++;
 }
}



printf("\nDirectory: %s\n", dir_name2);
//compare contents of each file
while (0 != (de2 = readdir(directory2))) {
	if(!(strcmp(de2->d_name,".")==0 || strcmp(de2->d_name,"..")==0)){
	printf("Found file: %s\n", de2->d_name);
	 pf2[count2] = de2;
	 count2 ++;	
	}
}


 printf("\n");
 //Cycle through  DIR1, and compare values with DIR2
 for (int num = 0; num < count1; num++)
 {
 	//is value does not exist will remain 0, and call replicate
 	isFalse = 0;
 	
 	for (int value =0; value<count2; value++){

 		//If the two file names are equal get mostRecent File to update
 		if(strcmp(pf1[num]->d_name, pf2[value]->d_name) == 0){
 			printf("File %s: exists in both directories\n", pf1[num]->d_name );

 			//calling most recent function
 			mostRecent(pf1[num], dir_name1, dir_name2);
 			isFalse = 1;
 		}
	}
	if(isFalse == 0){
		//file in DIR1 does not exist in DIR2
		printf("File does not exist in %s: %s\n\n", dir_name2 ,pf1[num]->d_name);
		replicateFile(pf1[num], dir_name1, dir_name2);
	}
 }


//do the same thing for directory B now,
 for(int value=0; value <count2; value++){
 	isFalse = 0;

 	//if file in B does not exist in A, remove the file.
 	for(int num=0; num< count1; num++){
 		if(strcmp(pf1[num]->d_name, pf2[value]->d_name) == 0)
 		{
 			isFalse=1;
 		}
 	}

 	if(isFalse == 0){
 		//file in DIR 2 does not exist in DIR1
		printf("This file does not exist in %s: %s\n", dir_name1, pf2[value]->d_name);
		removeFile(pf2[value], dir_name2);
	}

 	}


//Close the directories
closedir(directory1);
closedir(directory2);
}




void removeFile(struct dirent *p, char *dir_name){
	//remove file function

	printf("REMOVING file: %s\n", p->d_name);


	char path[SIZE];
	path[0] = 0;

	//create the path from file name and directory
	strcat(path, dir_name);
	strcat(path, "/");
	strcat(path, p->d_name);

	//remove the path and check for success or failure
	int rem = remove(path);

   	if(rem == 0) {
      printf("%s: deleted successfully\n", p->d_name);
   	}
   	else {
      printf("Error: unable to delete %s file\n", p->d_name);
   	}

   	printf("\n");
}


void replicateFile(struct dirent *p, char *dir_name1, char*dir_name2){
	//replicate function

	printf("Replicating File: %s into: %s\n", p->d_name, dir_name2);

	//delcaring variables
	char path1[SIZE], path2[SIZE];
	FILE *fp1;
	FILE *fp2;
	char line[SIZE];

	path1[0]= 0;
	path2[0]= 0;
	
	//create both paths
	strcat(path1, dir_name1);
	strcat(path1, "/");
	strcat(path1, p->d_name);

	strcat(path2, dir_name2);
	strcat(path2, "/");
	strcat(path2, p->d_name);

	//open both files
	fp1 = fopen(path1, "r");
	fp2 = fopen(path2, "w");

	//file readline != Null, take that line and store into file
	while(fgets(line, SIZE, fp1) != NULL){
		fputs(line, fp2);
	}

	//close both files
	fclose(fp1);
	fclose(fp2);
}

void mostRecent(struct dirent *p1, char *dir_name1, char*dir_name2){
	//function to test most recent variable

	//delcaring variables
	char path1[SIZE], path2[SIZE], line[SIZE];
	FILE *fp1;
	FILE *fp2;

	path1[0]= 0;
	path2[0]= 0;
	line[0]=0;

	//create paths
	strcat(path1, dir_name1);
	strcat(path1, "/");
	strcat(path1, p1->d_name);

	strcat(path2, dir_name2);
	strcat(path2, "/");
	strcat(path2, p1->d_name);


	printf("Most Recent File for:%s, %s\n", path1, path2);

	//create struct stat for files to get modification.
	struct stat stat1, stat2;
    stat(path1, &stat1);
    stat(path2, &stat2);

    //compare times and update the files accordingly
	 if(&stat1.st_mtime > &stat2.st_mtime){
	 	printf("File: %s last modified\n\n", path1);
	 	replicateFile(p1, dir_name1, dir_name2);
	 }
	 else{
	 	printf("File: %s last modified\n\n", path2);
	 	replicateFile(p1, dir_name2, dir_name1);
	 }

}


