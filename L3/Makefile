CC = cc
VIEWER = more

.PHONY: run view clean

run: l3.c
	$(CC) -o l3 l3.c
	./l3 $(DIR1) $(DIR2)

view: l3.c
	$(VIEWER) l3.c

clean:
	- rm l3

