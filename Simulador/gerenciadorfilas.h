#ifndef gerenciadorfilas
#define gerenciadorfilas

#define TEMPO_MAXIMO_BAIXA_PRIORIDADE 100
#include <stddef.h>
#include "fifo.h"
#include "processos.h"
#include "variables.h"



FIFO altaPrioridade;
FIFO baixaPrioridade;
FIFO filaDisco;
FIFO filaFita;
FIFO filaImpressora;

void initFilas() {
	init(&altaPrioridade);
	init(&baixaPrioridade);
	init(&filaDisco);
	init(&filaFita);
	init(&filaImpressora);
}

void atualizarTempoEsperaProcessosReady() {
	if(!empty(&altaPrioridade)) {
		int i = altaPrioridade.head;
		while(true) {
			Processo *processo = altaPrioridade.queue[i]; 
			increaseWaitTimeProcess(processo);			
			if(i == altaPrioridade.tail) break;
			i=(i+1)%MAX_PROCESSOS;
		}
	}
	if(!empty(&baixaPrioridade)) {
		/*Se tempoEspera>TEMPO_MAXIMA_BAIXA_PRIORIDADE => Passa processo para fila de alta prioridade*/
		if(baixaPrioridade.queue[0]->tempoEspera>TEMPO_MAXIMO_BAIXA_PRIORIDADE) add(&baixaPrioridade, pop(&altaPrioridade));
		int i = baixaPrioridade.head;
		while(true) {
			Processo *processo = baixaPrioridade.queue[i];
			increaseWaitTimeProcess(processo);
			if(i == baixaPrioridade.tail) break;
			i=(i+1)%MAX_PROCESSOS;
		}
	}
	
}

/*Escalona um processo da fila de alta prioridade, se houver
 *Se não, escalona um processo da fila de baixa prioridade, se houve
 *Se não, retorna NULL*/
Processo* selecionarProximoProcessoAExecutar() {
	Processo *processo;
	
	if (!empty(&altaPrioridade)) processo = pop(&altaPrioridade);
	else if(!empty(&baixaPrioridade)) processo = pop(&baixaPrioridade);
	else return NULL;
	
	runProcess(processo);
	return processo;
}

/*Preempção do RR*/
void interromperProcesso(Processo *processo) {
	if(!processoTerminou(processo)) {
		toReadyProcess(processo);
		add(&baixaPrioridade, processo);
	}
}

/*Interrupção de IO*/
void pedirIO(Processo *processo, int tempo) {
	int i;
	TempoChamadaIO tempoChamada;
	bool achou = 0;
	for(i = 0; i < processo->quantidadeChamadas; i++) {
		if(processo->chamada[i].tempoBloqueio == tempo) {
			tempoChamada = processo->chamada[i];
			achou = 1;
		}
	}
	if(!achou) {
		return;
	}
	switch(tempoChamada.tipoIO.tipoIO) {
		case DISCO:
			add(&filaDisco, processo);
			break;
		case IMPRESSORA:
			add(&filaImpressora, processo);
			break;
		case FITA_MAGNETICA:
			add(&filaFita, processo);
			break;
		default:
			printf("ERROR: Tipo I/O %d inválido\n", tempoChamada.tipoIO.tipoIO);
			break;
	}
	blockProcess(processo);
}
/*Soma 1 ao tempo de bloqueio dos processos em uma fila de IO*/
void updateFilaDeIO(IO tipo, FIFO *filaIO) {
	if(!empty(filaIO)) {
		int i = filaIO->head;
		while(true) {
			filaIO->queue[i]->tempoBloqueado++;			
			if(i==filaIO->tail) break;
			i=(i+1)%MAX_PROCESSOS;
		}

		i = filaIO->head;
		if(filaIO->queue[i]->tempoBloqueado == tipo.tempo) {
			Processo *process = pop(filaIO);
			unblockProcess(process);
			if(tipo.vaiPraAlta) add(&altaPrioridade, process);
			else add(&baixaPrioridade, process);
		}
	}
}
/*Atualizar as filas de todos os IOs*/
void updateBlockedProcesses() {
	updateFilaDeIO(tiposIO[DISCO], &filaDisco);
	updateFilaDeIO(tiposIO[FITA_MAGNETICA], &filaFita);
	updateFilaDeIO(tiposIO[IMPRESSORA], &filaImpressora);
}

/*Adiciona um processo novo à fila de prontos de alta prioridade*/
void adicionarProcessoNovo(Processo *processo) {
	add(&altaPrioridade, processo);
	toReadyProcess(processo);
}
#endif
