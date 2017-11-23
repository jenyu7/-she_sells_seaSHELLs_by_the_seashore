all: shell.o fxn.o
	gcc -o shell shell.o fxn.o
shell.o: shell.c
	gcc -c shell.c
fxn.o: fxn.c
	gcc -c fxn.c
run: all
	./shell

clean:
	rm -f *~ shell *#
