#ifndef variables
#define variables

#define MAX_PROCESSOS 20
#define TEMPO_RR 4
#define DISCO 0
#define FITA_MAGNETICA 1
#define IMPRESSORA 2
#define TEMPO_MAXIMO 100
#define TEMPO_MINIMO 1

/*-Variáveis Globais---------------------------------------------------*/
typedef struct _IO {
	int tipoIO;
	int tempo;
} IO;

//tipos de IO e seus respectivos tempos
IO tiposIO[3] = {{DISCO, 15}, {FITA_MAGNETICA, 30}, {IMPRESSORA, 20}};

enum statusTypes {new, ready, running, blocked, exit};

typedef struct _Processo {
	int PID;
	int PPID;
	int priority;
	int waitTime;
	enum statusTypes status;
} Processo;

typedef struct _TempoChamadaIO {
	IO tipoIO;
	int tempoBloqueio;
} TempoChamadaIO;

typedef struct _InstanciaProcesso {
	int PID;
	int tempoServico;
	int quantidadeChamadas;
	TempoChamadaIO *chamada; //vetor com os tempos de chamada de IO
} InstanciaProcesso;

//simula a memória que possui todas as informações do processo
//no geral o que eh manipulado nas filas é o _instanciaProcesso
Processo PCB[MAX_PROCESSOS];

int posFinalPCB = 0;

#endif

