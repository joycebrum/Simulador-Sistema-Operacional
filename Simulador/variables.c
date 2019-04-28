#include <stdio.h>

#define MAX_PROCESSOS 20
#define TEMPO_RR 4
#define DISCO 0
#define FITA_MAGNETICA 1
#define IMPRESSORA 2

/*-Variáveis Globais---------------------------------------------------*/
typedef struct _IO {
	int tipoIO;
	double tempo;
} IO;

//IO tiposIO[3] = [{ .tipoIO = 0, .tempo = 15}, { .tipoIO = 1, .tempo = 30}, { .tipoIO = 2, .tempo = 20}]
IO tiposIO[3] = {{DISCO, 15}, {FITA_MAGNETICA, 30}, {IMPRESSORA, 20}};



int main(int argc, char **argv) {
	
	return 0;
}
