#include "csapp.h"


void serve_static(int fd, char *filename, int filesize, char * fname);


int main(int argc, char **argv){


//variable declaration	
int listenfd, connfd, port, clientlen;
struct sockaddr_in clientaddr;
struct hostent *hp;
char *haddrp;
struct stat sbuf;
char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
char filename[MAXLINE], cgiargs[MAXLINE];
rio_t rio;
char * fname;
port = atoi(argv[2]);
fname = argv[3];

//use the port number to listen to client, could not get host to work
listenfd = open_listenfd(port);


	//continue until server closes
	while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
	sizeof(clientaddr.sin_addr.s_addr), AF_INET);

	//connect print connected client
	haddrp = inet_ntoa(clientaddr.sin_addr);
	printf("Connected: %s %s\n", hp->h_name, haddrp);
	Close(connfd);
	}

}


void serve_static(int fd, char *filename, int filesize, char * fname) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 	
    strcat(filename, fname);
    /* Send response headers to client */
    strcpy(filetype, "text/html");     
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);   
    srcp = (char *)malloc(filesize);
    Rio_readn(srcfd, srcp, filesize);
    Close(srcfd);                        
    Rio_writen(fd, srcp, filesize);

    
}
