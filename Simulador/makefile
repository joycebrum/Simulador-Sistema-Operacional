########### Makefile ##########
all: main
main: main.o
	gcc -o main main.o
main.o: main.c processos.h variables.h
	gcc -o main.o -c main.c -W -Wall -ansi -pedantic
clean:
	rm -rf *.o
mrproper: clean
	rm - rf all
