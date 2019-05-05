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

void atualizarTempoEsperaProcessosReady(FILE *f) {
	fprintf(f,"Fila de alta prioridade: [");
	if(!empty(&altaPrioridade)) {
		int i = altaPrioridade.head;
		while(true) {
			Processo *processo = altaPrioridade.queue[i];
			increaseWaitTimeProcess(processo);
			fprintf(f,"%d ",  altaPrioridade.queue[i]->PID); 			
			if(i == altaPrioridade.tail) break;
			i=(i+1)%MAX_PROCESSOS;
		}
	}
	fprintf(f,"]\n");
	fprintf(f,"Fila de baixa prioridade: [");
	if(!empty(&baixaPrioridade)) {
		/*Se tempoEspera>TEMPO_MAXIMA_BAIXA_PRIORIDADE => Passa processo para fila de alta prioridade*/
		if(baixaPrioridade.queue[0]->tempoEspera>TEMPO_MAXIMO_BAIXA_PRIORIDADE) add(&altaPrioridade, pop(&baixaPrioridade));
		int i = baixaPrioridade.head;
		while(true) {
			Processo *processo = baixaPrioridade.queue[i];
			increaseWaitTimeProcess(processo);
			fprintf(f,"%d ", baixaPrioridade.queue[i]->PID);
			if(i == baixaPrioridade.tail) break;
			i=(i+1)%MAX_PROCESSOS;
		}
	}
	fprintf(f,"]\n");
	
	
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
	toReadyProcess(processo);
	add(&baixaPrioridade, processo);
}

/*Interrupção de IO*/
bool pedirIO(Processo *processo, int tempo, FILE *f) {
	int i;
	bool achouIO = false;
	TempoChamadaIO tempoChamada;
	for(i = 0; i < processo->quantidadeChamadas; i++) {
		if(processo->chamada[i].tempoBloqueio == tempo)	{
			tempoChamada = processo->chamada[i];
			achouIO = true;			
			break;
		}
	}
	if(!achouIO) return false;
	fprintf(f,"Interrompendo processo com PID = %d para IO = %s. ", processo->PID, tempoChamada.tipoIO.nomeTipo);
	fprintf(f,"Processo retornará no Quantum = %d\n", tempoChamada.tipoIO.tempo + tempo);
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
			fprintf(f,"ERROR: Tipo I/O %d inválido\n", tempoChamada.tipoIO.tipoIO);
			break;
	}
	blockProcess(processo);
	return true;
}
/*Soma 1 ao tempo de bloqueio dos processos em uma fila de IO*/
void updateFilaDeIO(IO tipo, FIFO *filaIO, FILE *f) {
	if(!empty(filaIO)) {
		int i = filaIO->head;
		fprintf(f,"Fila de bloqueados IO %s: [", tipo.nomeTipo);
		while(true) {
			filaIO->queue[i]->tempoBloqueado++;
			fprintf(f," %d",  filaIO->queue[i]->PID);			
			if(i==filaIO->tail) {
				fprintf(f,"]\n");
				break;
			}
			i=(i+1)%MAX_PROCESSOS;
		}

		i = filaIO->head;
		if(filaIO->queue[filaIO->head]->tempoBloqueado == tipo.tempo) {
			Processo *process = pop(filaIO);
			unblockProcess(process);
			if(tipo.vaiPraAlta) add(&altaPrioridade, process);
			else add(&baixaPrioridade, process);
		}
	}
}
/*Atualizar as filas de todos os IOs*/
void updateBlockedProcesses(FILE *f) {
	updateFilaDeIO(tiposIO[DISCO], &filaDisco, f);
	updateFilaDeIO(tiposIO[FITA_MAGNETICA], &filaFita, f);
	updateFilaDeIO(tiposIO[IMPRESSORA], &filaImpressora, f);
}

/*Adiciona um processo novo à fila de prontos de alta prioridade*/
void adicionarProcessoNovo(Processo *processo) {
	add(&altaPrioridade, processo);
	toReadyProcess(processo);
}
#endif
