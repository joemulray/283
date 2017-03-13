CC = gcc
FLAGS = -lpthread
VIEWER = more

.PHONY: run view clean

run: prob1

dot: dotpr-hw.c csapp.c
	$(CC) -c dotpr-hw.c csapp.c
	$(CC) *.o $(FLAGS)
	./a.out


hello: hello-hw.c csapp.c
	$(CC) -c hello-hw.c csapp.c
	$(CC) *.o $(FLAGS)
	./a.out

prob1: hello.c csapp.c
	$(CC) -c hello.c csapp.c
	$(CC) *.o $(FLAGS)
	./a.out $(ARG)

view: hello.c dotpr-hw.txt hello-hw.txt
	$(VIEWER) hello.c
	$(VIEWER) dotpr-hw.txt
	$(VIEWER) hello-hw.txt
	
clean:
	-rm *.o
	-rm a.out

