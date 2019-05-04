#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
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

void inicializacao();
void executarProcesso();
void geraTempoAleatorioParaCriacaoProcessos();
void escalonarProcesso();
void processador();
void criaProcessos();

/*-Main---------------------------------------------------------------*/
int main () {
	inicializacao();
	geraTempoAleatorioParaCriacaoProcessos();
	while(true){
		updateBlockedProcesses();		
		criaProcessos();
		processador();		
		atualizarTempoEsperaProcessosReady();
		tempoDecorrido++;
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
}

/*Processador escalona um novo processo em 2 casos
 *1) Se não houver processo escalonado atualmente
 *2) Se o processo atualmente escalonado fizer um IO no tempo atual*/
void executarProcesso() {
	if(!processoExecutando) escalonarProcesso();
	else if(pedirIO(processoExecutando, tempoDecorrido)) escalonarProcesso();
	if(processoExecutando) {
		processoExecutando->tempoExecutado++;
		tempoExecutadoProcessador++;
	}
}

void escalonarProcesso() {
	tempoExecutadoProcessador = 0;
	processoExecutando = selecionarProximoProcessoAExecutar();
}

void processador() {
	if(tempoExecutadoProcessador > TEMPO_RR) {
		interromperProcesso(processoExecutando);
		escalonarProcesso();
	}
	else executarProcesso();
}

void geraTempoAleatorioParaCriacaoProcessos() {
	int i;
	for(i=0;i<MAX_PROCESSOS;i++){
		tempoGeracaoProcessos[i]= rand()%TEMPO_MAX_CHEGADA;
	}
}

void criaProcessos() {
	if(numProcesso<MAX_PROCESSOS && 
	  tempoGeracaoProcessos[numProcesso] == tempoDecorrido) {
		adicionarProcessoNovo(createNewProcess((2+rand()%5), 0, tempoDecorrido));
		numProcesso++;
	}
}
