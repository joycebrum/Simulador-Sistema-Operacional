#define NUMERO_MAXIMO_PROCESSOS_CRIADOS 20;
#define FATIA_TEMPO_POR_PROCESSO 4;
#define DISCO 0;
#define FITA_MAGNETICA 1;
#define IMPRESSORA 2;

typedef struct {
	int tipoIO;
	float tempo;
} IO;

IO tiposIO[3] = [{ .tipoIO = 0, .tempo = 15}, { .tipoIO = 1, .tempo = 30}, { .tipoIO = 2, .tempo = 20}]