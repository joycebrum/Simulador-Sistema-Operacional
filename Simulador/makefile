########### Makefile ##########
all: final
final: processos.o
	gcc -o final processos.o
main.o: processos.c variables.h
	gcc -o processos.o -c processos.c -W -Wall -ansi -pedantic
clean:
	rm -rf *.o
mrproper: clean
	rm - rf all
