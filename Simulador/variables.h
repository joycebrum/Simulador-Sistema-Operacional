#ifndef variables
#define variables

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

//tipos de IO e seus respectivos tempos
IO tiposIO[3] = {{DISCO, 15}, {FITA_MAGNETICA, 30}, {IMPRESSORA, 20}};

enum statusTypes {created, running, waiting, blocked, terminated};

typedef struct _Processo {
	int PID;
	int PPID;
	int priority;
	float waitTime;
	enum statusTypes status;
} Processo;


#endif

