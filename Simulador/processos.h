#ifndef processos
#define processos

#include "variables.h"
#include <time.h>
#include <stdlib.h>

int PID = 10;

void initSrand() {
	srand(time(0));
}

int getValorAleatorio(int minimo, int maximo) {
	return rand() % (maximo + 1 - minimo) + minimo;
}

int getTempoAleatorio(int tempoMaxEspecifico) {
	if(tempoMaxEspecifico == 0) {
		return getValorAleatorio(TEMPO_MINIMO, TEMPO_MAXIMO);
	}
	else {
		return getValorAleatorio(TEMPO_MINIMO, tempoMaxEspecifico);
	}
}
TempoChamadaIO* getTempoBloqueioAleatorio(int quantidadeIO, int tempoMaximoInstancia) {
	TempoChamadaIO *temposChamada;
	temposChamada = (TempoChamadaIO*) malloc(quantidadeIO*sizeof(TempoChamadaIO));

	int *temposUtilizados = (int*) malloc(quantidadeIO*sizeof(int));
	int i;
	for(i = 0; i < quantidadeIO; i++) {
		TempoChamadaIO novoTempoChamada;
		int tempoBloqueio = getTempoAleatorio(tempoMaximoInstancia);

		int j;
		bool unico = false;
		while(!unico) {
			for(j=0; j < i; j++) {
				if(tempoBloqueio == temposUtilizados[j]) {
					tempoBloqueio = getTempoAleatorio(tempoMaximoInstancia);
					continue;
				}
			}
			unico = true;
		}
		temposUtilizados[i] = tempoBloqueio;
		novoTempoChamada.tempoBloqueio = tempoBloqueio;
		novoTempoChamada.tipoIO = tiposIO[getValorAleatorio(0, 2)];

		temposChamada[i] = novoTempoChamada;
	}
	
	return temposChamada;
}

InstanciaProcesso createNewProcess(int priority, int PPID) {
	Processo newProcesso;

	newProcesso.priority = priority;
	newProcesso.PID = PID++;
	newProcesso.PPID = PPID;
	newProcesso.status = created;
	newProcesso.waitTime = 0;

	InstanciaProcesso instancia;
	instancia.PID = newProcesso.PID;
	instancia.tempoServico = getTempoAleatorio(0);

	int quantidadeIO = getValorAleatorio(0, instancia.tempoServico);
	instancia.quantidadeChamadas = quantidadeIO;
	instancia.chamada = getTempoBloqueioAleatorio(quantidadeIO, instancia.tempoServico);
	return instancia;
}

void increaseWaitTimeProcess(Processo *processo, float timePassed) {
	processo->waitTime += timePassed;
}

void resetWaitTimeProcess(Processo *processo) {
	processo->waitTime = 0;
}

void blockProcess(Processo *processo) {
	processo->status = blocked;
}

void runProcess(Processo *processo) {
	processo->status = running;
}
void endProcess(Processo *processo) {
	processo->status = terminated;
}
void interruptProcess(Processo *processo) {
	processo->status = waiting;
}
void unblockProcess(Processo *processo) {
	//TODO remove o processo da fila de bloqueados
	processo->status = waiting;
}

#endif
