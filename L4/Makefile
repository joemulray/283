CC = gcc
VIEWER = more

.PHONY: run view clean

run: l4

l4: l4.c csapp.c
	$(CC) -c csapp.c l4.c
	$(CC) *.o -l pthread
	./a.out	$(HOST) $(PORT) $(FILE)	

view: l4.c
	$(VIEWER) l4.c


clean:
	-rm a.out


