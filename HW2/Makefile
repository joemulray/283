CC = gcc
VIEWER = more

.PHONY: run view clean



run: prob6 prob8 prob10


prob6: prob6.c
	$(CC) prob6.c -o 6
	./6

prob8: prob8.c
	$(CC) prob8.c -o 8
	./8

prob10: prob10.c
	$(CC) prob10.c -o 10
	./10

view: prob6.c prob8.c prob10.c
	$(VIEWER) prob6.c
	$(VIEWER) prob8.c
	$(VIEWER) prob10.c
	
clean:
	- rm 6 8 10


