/*
Joseph Mulray
CS 283: HW2
10.8
Write a version of the statcheck program
that takes a descriptor number on the command line
rather than a file name
*/


#include "csapp.h"


int main (int argc, char **argv)
{
struct stat stat;
char *type, *readok;
fstat(argv[1], &stat); /*Use fstat instead of stat for file descriptor*/

if (S_ISREG(stat.st_mode)) /* Determine file type */
    type = "regular";
else if (S_ISDIR(stat.st_mode))
    type = "directory";
else
    type = "other";
if ((stat.st_mode & S_IRUSR)) /* Check read access */
        readok = "yes";
    else
        readok = "no";
printf("type: %s, read: %s\n", type, readok);
exit(0); 
}
