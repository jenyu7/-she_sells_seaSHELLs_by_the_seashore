all: shell.o fxn.o
	gcc -g -o shell shell.o fxn.o

shell.o: shell.c
	gcc -c shell.c

fxn.o: fxn.c
	gcc -c fxn.c

run: all
	./shell

valgrind: all
	valgrind --leak-check=yes ./shell

gdb: all
	gdb ./shell

clean:
	rm -f *~ shell *#
