#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "variables.h"
#include "processos.h"
#include "fifo.h"
#include "gerenciadorfilas.h"

/*-Global Variables---------------------------------------------------*/
int tempoDecorrido;//Variável que representa o tempo 
int tempoExecutando;//
int tempoGeracaoProcessos[MAX_PROCESSOS];//Vetor com 20 tempos de chegada de processos aleatórios
int numProcesso; //Índice do último processo criado
Processo *processoExecutando;
int tempoExecutadoProcessador;
FILE *f;//Arquivo de saída onde serão mostrados estágios do escalonamento

void inicializacao();
void executarProcesso();
void geraTempoAleatorioParaCriacaoProcessos();
void escalonarProcesso();
void processador();
void criaProcessos();
int compare();
/*-Main---------------------------------------------------------------*/
int main () {
	inicializacao();
	geraTempoAleatorioParaCriacaoProcessos();
	fprintf(f,"Tempos de chegada de processos gerados: [");
	for(int i=0;i<MAX_PROCESSOS; i++) {
		fprintf(f,"%d ",tempoGeracaoProcessos[i]);
		if(i==MAX_PROCESSOS-1) fprintf(f,"%d]\n", tempoGeracaoProcessos[i]);
	}
	while(true){
		fprintf(f,"\n\nQuantum = %d\n",tempoDecorrido);
		updateBlockedProcesses();		
		criaProcessos();
		processador();		
		atualizarTempoEsperaProcessosReady();
		tempoDecorrido++;
		if(tempoDecorrido==1000) {
			fprintf(f,"Interrompendo escalonador\n");
			exit(-1);
		}
	}
}

/*-Functions----------------------------------------------------------*/
void inicializacao() {
	initSrand();
	initFilas();
	tempoDecorrido = 0;
	tempoExecutando = 0;
	numProcesso = 0;
	tempoExecutadoProcessador = 0;
	processoExecutando = NULL;
	f = fopen("file.txt", "w");
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
	if(pedirIO(processoExecutando, tempoDecorrido, f)) escalonarProcesso();
	/*Se algum processo for escalonado*/
	if(processoExecutando) {
		processoExecutando->tempoExecutado++;
		tempoExecutadoProcessador++;
		fprintf(f,"Informações do PCB do processo em execução:\n");
		fprintf(f,"-------------------------------------\n");
		fprintf(f,"|PID = %d                           |\n", processoExecutando->PID);
		fprintf(f,"|Tempo de Serviço = %d              |\n", processoExecutando->tempoServico);
		fprintf(f,"|Tempo executado = %d                |\n", processoExecutando->tempoExecutado);
		fprintf(f,"-------------------------------------\n");
	}
}

void escalonarProcesso() {
	tempoExecutadoProcessador = 0;
	processoExecutando = selecionarProximoProcessoAExecutar();
	if(processoExecutando) fprintf(f,"Escalonando processo com PID = %d\n", processoExecutando->PID);
	
}

void processador() {
	/*if(processoExecutando){
		if(processoTerminou(processoExecutando)){
			fprintf(f,"Processo com PID = %d terminou\n", processoExecutando->PID);
			escalonarProcesso();	
		}
		else if(tempoExecutadoProcessador == TEMPO_RR) {
			fprintf(f,"Preemptando processo com PID = %d\n",processoExecutando->PID);
			interromperProcesso(processoExecutando);
			escalonarProcesso();
		}
	else executarProcesso();*/
	
	/*Se no quantum anterior ninguém executou*/
	if(!processoExecutando) escalonarProcesso();
	/*Se algum processo foi escalonado*/
	if(processoExecutando){
		if(processoTerminou(processoExecutando)) {
			fprintf(f,"Processo com PID = %d terminou\n", processoExecutando->PID);
			escalonarProcesso();
		}
		else if(tempoExecutadoProcessador == TEMPO_RR){
			fprintf(f,"Preemptando processo com PID = %d\n",processoExecutando->PID);
			interromperProcesso(processoExecutando);
			escalonarProcesso();
		}
		executarProcesso();
	}
	else fprintf(f,"Não há processo disponível para escalonamento\n");
	
}

void geraTempoAleatorioParaCriacaoProcessos() {
	int i;
	for(i=0;i<MAX_PROCESSOS;i++){
		tempoGeracaoProcessos[i]= rand()%TEMPO_MAX_CHEGADA;
	}
	qsort(tempoGeracaoProcessos,MAX_PROCESSOS,sizeof(*tempoGeracaoProcessos),compare);
}

void criaProcessos() {
	if(numProcesso<MAX_PROCESSOS && 
	  tempoGeracaoProcessos[numProcesso] == tempoDecorrido) {
		Processo *processo = createNewProcess((2+rand()%5), 0, tempoDecorrido);
		adicionarProcessoNovo(processo);
		fprintf(f,"Adicionando processo com PID = %d à fila de alta prioridade\n", processo->PID);
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
