CC = gcc
VIEWER = less
OUTFILE = out

.PHONY: run view clean


run: h4

h4: h4.c
	gcc -o $(OUTFILE) h4.c
	./$(OUTFILE) $(DIR) $(ARG1) $(ARG2) $(ARG3)

view: README h4.c
	$(VIEWER) README
	$(VIEWER) h4.c

clean:
	- rm $(OUTFILE)

