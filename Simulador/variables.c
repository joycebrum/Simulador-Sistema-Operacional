#define MAX_PROCESSOS 20;
#define TEMPO_RR 4;
#define DISCO 0;
#define FITA_MAGNETICA 1;
#define IMPRESSORA 2;

typedef struct _IO {
	int tipoIO;
	double tempo;
} IO;

IO tiposIO[3] = [{ .tipoIO = 0, .tempo = 15}, { .tipoIO = 1, .tempo = 30}, { .tipoIO = 2, .tempo = 20}]
