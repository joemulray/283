#Joseph Mulray
#CS 283: Shell Project

CC = gcc
VIEWER = more
OUTFILE = tsh
FILES = \
	myint.c \
	myspin.c \
	mysplit.c \
	mystop.c \

MAIN = tsh-ck.c
FLAGS = -g -Wall


.PHONY: run view clean

run: shell

shell: 
	$(CC) $(FLAGS) $(MAIN) -o $(OUTFILE)
	./$(OUTFILE)

view: tsh-ck.c
	$(VIEWER) $(MAIN)



test:
	$(CC) -c $(FILES)
	

clean: 
	-rm $(OUTFILE) 
	-rm *.o
	
