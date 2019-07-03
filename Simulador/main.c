#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "variables.h"
#include "processos.h"
#include "fifo.h"
#include "gerenciadorfilas.h"
#include "gerenciadormemoria.h"



/*-Global Variables---------------------------------------------------*/
int tempoDecorrido;//Variável que representa o tempo 
int tempoExecutando;//
int numProcesso;//Índice do último processo criado
int numProcessosFinalizados;//Número de processos que já terminaram
Processo processosFinalizados[MAX_PROCESSOS];//Vetor que guarda a ordem que os processos finalizaram
Processo *processoExecutando;
int tempoExecutadoProcessador;

void inicializacao();
void executarProcesso();
void escalonarProcesso();
void processador();
void criaProcessos();
int compare();
void printTemposProcessos();
void printProcessosFinalizados();
FILE *f;//Arquivo de saída onde serão mostrados estágios do escalonamento
FILE *processLog;
/*-Main---------------------------------------------------------------*/
int main () {
	inicializacao();
	while(numProcessosFinalizados<MAX_PROCESSOS){
		fprintf(f,"\n\nInstante = %d\n",tempoDecorrido);
		updateBlockedProcesses(f);		
		criaProcessos();
		processador();	
		atualizarTempoEsperaProcessosReady(f);
		imprimeTodasAsFilas(f);
		tempoDecorrido++;
	}
	printProcessosFinalizados();
	fclose(f);
	fclose(processLog);
}

/*-Functions----------------------------------------------------------*/
void inicializacao() {
	initSrand();
	initFilas();
	initMemoria();
	tempoDecorrido = 0;
	tempoExecutando = 0;
	numProcesso = 0;
	tempoExecutadoProcessador = 0;
	processoExecutando = NULL;
	f = fopen("log.txt", "w");
	processLog = fopen("processoLog.txt", "w");
	if (f == NULL){
		printf("Error opening file!\n");
		exit(1);
	}
}


/*Processador escalona um novo processo em 2 casos
 *1) Se não houver processo escalonado atualmente
 *2) Se o processo atualmente escalonado fizer um IO no tempo atual*/
void executarProcesso() {
	/*Se há interrupção de IO do processo escalonado, escalona um novo*/
	processoExecutando->tempoExecutado++;
	if(procuraEPedeIO(processoExecutando, tempoDecorrido, f)) {
		printProcessoExecutando(processoExecutando, f);
		processoExecutando = NULL;
	}
	/*Se algum processo for escalonado*/
	else if(processoExecutando) {
		fprintf(f, "Processo de PID: %d consome o processador\n", processoExecutando->PID);
		tempoExecutadoProcessador++;
		printProcessoExecutando(processoExecutando, f);
	}
}

/*Se não houver processos na fila de prontos, processoExecutando == NULL*/
void escalonarProcesso() {
	tempoExecutadoProcessador = 0;
	processoExecutando = selecionarProximoProcessoAExecutar();
	if(processoExecutando) {
		verificaSeFazSwapIn(processoExecutando, f);
		fprintf(f,"Escalonando processo com PID = %d\n", processoExecutando->PID);
	}
	
}

/*Faz o papel do processador: escalona e executa processos*/
void processador() {
	/*Se no quantum anterior ninguém executou*/
	if(!processoExecutando) escalonarProcesso();
	/*Se algum processo foi escalonado*/
	if(processoExecutando){
		if(processoTerminou(processoExecutando)) {
			fprintf(f,"Processo com PID = %d terminou, removendo todas as suas páginas da memória\n", processoExecutando->PID);
			swapOutProcess(processoExecutando, f);
			fprintf(f, "\n");
			processoExecutando->tempoTermino = tempoDecorrido - processoExecutando->tempoEntrada;
			processosFinalizados[numProcessosFinalizados] = *processoExecutando;
			numProcessosFinalizados++;
			escalonarProcesso();
		}
		else if(tempoExecutadoProcessador == TEMPO_RR){
			fprintf(f,"Preemptando processo com PID = %d\n",processoExecutando->PID);
			interromperProcesso(processoExecutando);
			escalonarProcesso();
		}
		
		if(processoExecutando) {
			fprintf(f, "---Gerenciador de Memoria---\n");
			while(processoExecutando && gerenciaMemoria(processoExecutando, f) == 1) {
				fprintf(f,"Bloqueando processo com PID = %d enquanto sua página é carregada\n",processoExecutando->PID);
				pedirIO(processoExecutando, tiposIO[DISCO], &filaDisco);
				printProcessoExecutando(processoExecutando, f);
				escalonarProcesso();
			}
			fprintf(f, "---Fim do Gerenciador de Memoria---\n");
		}
		
	}
	if(processoExecutando) {
		executarProcesso();
	}
	else fprintf(f,"Não há processo disponível para escalonamento\n");
	
}

void criaProcessos() {
	if(tempoDecorrido % 3 == 0 && numProcesso < MAX_PROCESSOS) {
		Processo *processo = createNewProcess((2+rand()%5), 0, tempoDecorrido);
		adicionarProcessoNovo(processo);
		fprintf(f,"Processo PID = %d criado e adicionado à fila de alta prioridade \n", processo->PID);
		printNovoProcesso(processo, processLog, tempoDecorrido);
		numProcesso++;
	}
}

int compare(const void * elem1, const void * elem2) {
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

void printProcessosFinalizados(){
	fprintf(f,"\nProcessos finalizados:\n-------------------------------------\n");
	for(int i=0;i<MAX_PROCESSOS;i++){
		fprintf(f,"| PID = %d  ----  Turnaround = %d\n", processosFinalizados[i].PID, processosFinalizados[i].tempoTermino);
		if(i==MAX_PROCESSOS-1) {
			fprintf(f,"-------------------------------------\n");
		}
	}
}
