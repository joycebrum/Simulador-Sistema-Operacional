#ifndef variables
#define variables

#define MAX_PROCESSOS 20
#define TEMPO_RR 4
#define DISCO 0
#define FITA_MAGNETICA 1
#define IMPRESSORA 2
#define TEMPO_MAXIMO 20
#define TEMPO_MINIMO 1
#define TEMPO_MAX_CHEGADA 60

// Gerenciados de Memoria
#define MEM_PRINCIPAL = 64
#define INTERV_TEMP_PROC = 3
#define INTERV_PAGS = 3
#define WSL = 4
#define MAX_VIRT_PAGE = 64


/*-Variáveis Globais---------------------------------------------------*/

/*Tupla que indica o tipo de IO e a duração*/
typedef struct _IO {
	int tipoIO;
	char nomeTipo[20];
	int tempo;
	bool vaiPraAlta;
} IO;

//tipos de IO e seus respectivos tempos
IO tiposIO[3] = {{DISCO, "Disco", 6, 0}, {FITA_MAGNETICA, "Fita Magnética", 10, 1}, {IMPRESSORA, "Impressora", 15, 1}};

//não pode ser "new" ou "exit" pq sao simbolos da linguagem
enum statusTypes {novo, ready, running, blocked, terminado};


/*Tuplas da forma (IO, tempo), onde tempo*/
typedef struct _TempoChamadaIO {
	IO tipoIO;
	int tempoBloqueio;
} TempoChamadaIO;

typedef struct _Processo {
	int PID;
	int PPID;
	int priority;
	enum statusTypes status;//Indica o status atual do processo
	int tempoEntrada;//Tempo em que o processo é criado
	int tempoServico;//Tempo previsto de processamento do processo
	int tempoExecutado;//Sempre no intervalo [tempoEntrada,tempoServico]
	int tempoTermino;//Tempo em que o processo termina de executar
	int tempoEspera;//Tempo que processo está ocioso na fila de baixa prioridade (ready)
	int tempoBloqueado; //Tempo que o processo está executando I/O
	int quantidadeChamadas; //Quantidade de IO que o processo irá executar
	TempoChamadaIO *chamada; //Vetor com os tempos de chamada de cada IO
} Processo;


#endif

