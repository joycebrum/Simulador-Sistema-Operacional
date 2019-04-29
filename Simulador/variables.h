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

/*Tupla que indica o tipo de IO e a duração*/
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
	enum statusTypes status;//Indica o status atual do processo
} Processo;

/*Tuplas da forma (IO, tempo), onde tempo*/
typedef struct _TempoChamadaIO {
	IO tipoIO;
	int tempoBloqueio;
} TempoChamadaIO;

typedef struct _InstanciaProcesso {
	int PID;
	int tempoEntrada;//Tempo em que o processo é escalonado pela primeira vez
	int tempoServico;//Tempo decorrido desde o escalonamento do processo (apenas incrementado enquanto o processo está escalonado)
	int tempoExecutado;//Sempre no intervalo [tempoEntrada,tempoServico]
	int tempoEspera;//Tempo que processo está ocioso na fila de baixa prioridade (ready)
	int quantidadeChamadas; //Quantidade de IO que o processo irá executar
	TempoChamadaIO *chamada; //Vetor com os tempos de chamada de cada IO
} InstanciaProcesso;

/*Simula a memória que possui todas as informações do processo (PCB, Process Control Block).
 *No geral, o que é manipulado nas filas é o InstanciaProcesso.*/
Processo PCB[MAX_PROCESSOS];

/*Índice que diz a quantidade de processos criados até o momento menos 1.
 *Será usado para iterar sobre a variável PCB */
int posFinalPCB = 0;

#endif

