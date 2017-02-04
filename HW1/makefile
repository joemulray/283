CC = gcc
VIEW = more

.PHONY: run view clean


run: problem1 problem2 problem3

problem1: problem1.c
	$(CC) problem1.c -o 1
	./1 $(arg)


problem2: problem2.c
	$(CC) problem2.c -o 2
	./2 $(arg)

problem3: problem3.c
	$(CC) problem3.c -o 3
	./3 $(arg)


view: problem1.c problem2.c problem3.c
	$(VIEWER) problem1.c
	$(VIEWER) problem2.c
	$(VIEWER) problem3.c


clean:
	rm -f one 1 2 3
