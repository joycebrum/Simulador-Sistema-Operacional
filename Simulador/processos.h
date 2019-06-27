#ifndef processos
#define processos

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "variables.h"

int PID = 10;

/*Inicializa semente*/
void initSrand() {
	srand(time(0));
}

/*Gera valor aleatório entre [mínimo,máximo]*/
int getValorAleatorio(int minimo, int maximo) {
	return rand() % (maximo + 1 - minimo) + minimo;
}


/*Gera o numero de páginas do processo entre 1 e 64*/
int gera_num_paginas() { 
	return getValorAleatorio(1, MAX_VIRT_PAGE);
}

/*Gera um valor aleatório entre [TEMPO_MINIMO,TEMO_MAXIMO]*/
int getTempoAleatorio(int tempoMaxEspecifico) {
	if(tempoMaxEspecifico == 0) {
		return getValorAleatorio(TEMPO_MINIMO, TEMPO_MAXIMO);
	}
	else {
		return getValorAleatorio(TEMPO_MINIMO, tempoMaxEspecifico);
	}
}

/*Gera todas as tuplas de (IO,tempo) de um processo*/
TempoChamadaIO* getTempoBloqueioAleatorio(int quantidadeIO, int tempoServico) {
	TempoChamadaIO *temposChamada;
	temposChamada = (TempoChamadaIO*) malloc(quantidadeIO*sizeof(TempoChamadaIO));

	int i;
	for(i = 0; i < quantidadeIO; i++) {
		int tempoBloqueio = getTempoAleatorio(tempoServico);

		int j;
		for(j=0; j < i; j++) {
			if(tempoBloqueio == temposChamada[j].tempoBloqueio) {
				tempoBloqueio = getTempoAleatorio(tempoServico);
				j=0;
			}
		}
	
		temposChamada[i].tempoBloqueio = tempoBloqueio;
		temposChamada[i].tipoIO = tiposIO[getValorAleatorio(0,2)];
	}
	
	return temposChamada;
}

/*Cria o PCB de um novo processo*/
Processo* createNewProcess(int priority, int PPID, int tempo) {
	Processo *newProcesso = (Processo*) malloc(sizeof(Processo));

	newProcesso->priority = priority;
	newProcesso->PID = PID++;
	newProcesso->PPID = PPID;
	newProcesso->status = novo;
	newProcesso->tempoExecutado = 0;
	newProcesso->tempoEspera = 0;
	newProcesso->tempoEntrada = tempo;	
	newProcesso->tempoServico = getTempoAleatorio(0);

	int quantidadeIO = getValorAleatorio(0, newProcesso->tempoServico);
	newProcesso->quantidadeChamadas = quantidadeIO;
	newProcesso->chamada = getTempoBloqueioAleatorio(quantidadeIO, newProcesso->tempoServico);
	
	newProcesso->numPaginas = gera_num_paginas();
	newProcesso->tabelaPaginas = (Tabela_Paginas *)malloc(MAX_VIRT_PAGE*sizeof(Tabela_Paginas));
	newProcesso->numPaginasAlocadas = 0;
	for(int i=0; i < newProcesso->numPaginas;i++){
		newProcesso->tabelaPaginas[i].num_pagina = i;
		newProcesso->tabelaPaginas[i].num_frame = -1;
	}
	newProcesso->gerenciadorPaginas = (GerenciadorPaginas*)malloc(sizeof(GerenciadorPaginas));
	initLRU(newProcesso->gerenciadorPaginas);

	return newProcesso;
}

/*Incrementa o tempo de espera de um processo caso esteja na fila de prontos.
 *Utilizada para controlar a fila de prioridade.*/
void increaseWaitTimeProcess(Processo *processo) {
	processo->tempoEspera++;
}

/*Reseta o tempo de espera de um processo caso saia de uma das filas de prontos.
 *Indica que o processo ganhou prioridade.*/
void resetWaitTimeProcess(Processo *processo) {
	processo->tempoEspera = 0;
}

void blockProcess(Processo *processo) {
	processo->status = blocked;
}

void runProcess(Processo *processo) {
	processo->status = running;
}
void endProcess(Processo *processo) {
	processo->status = terminado;
}
void toReadyProcess(Processo *processo) {
	processo->status = ready;
} 
void unblockProcess(Processo *processo) {
	toReadyProcess(processo);
	processo->tempoBloqueado = 0;
}

bool processoTerminou(Processo *processo) {
	if(processo->tempoExecutado == processo->tempoServico) {
		endProcess(processo);
		return true;
	}
	return false;
}

// para exibição de resultado
void printProcesso(Processo *processo, FILE *f) {
	fprintf(f,"Informações do processo criado:\n");
	fprintf(f,"-------------------------------------\n");
	fprintf(f,"|PID = %d \n", processo->PID);
	fprintf(f,"|Tempo de Serviço = %d \n", processo->tempoServico);
	fprintf(f,"|Quantidade IO = %d \n", processo->quantidadeChamadas);
	fprintf(f,"|Paginas Referenciadas = [ ] \n");
	fprintf(f,"-------------------------------------\n");
	
	/*int i = 0;
	for (i = 0; i < quantidade; i++) {
		fprintf(f,"(%s, %d) ",  processo.chamada[i].tipoIO.nomeTipo, processo.chamada[i].tempoBloqueio);
	}*/
}

#endif
