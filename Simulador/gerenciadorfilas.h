#ifndef gerenciadorfilas
#define gerenciadorfilas

#define TEMPO_MAXIMO_BAIXA_PRIORIDADE TEMPO_MAXIMO
#include <stddef.h>
#include "fifo.h"
#include "variables.h"
#include "processos.h"


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
		if(baixaPrioridade.queue[baixaPrioridade.head]->tempoEspera>TEMPO_MAXIMO_BAIXA_PRIORIDADE) {
			add(&altaPrioridade, pop(&baixaPrioridade));
		}
		int i = baixaPrioridade.head;
		while(!empty(&baixaPrioridade)) {
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
	
	return processo;
}

/*Preempção do RR*/
void interromperProcesso(Processo *processo) {
	toReadyProcess(processo);
	add(&baixaPrioridade, processo);
}

void pedirIO(Processo *processo, IO tipo, FIFO *fila) {
	processo->tempoBloqueado = 0;
	add(fila, processo);
	blockProcess(processo);
}

/*Interrupção de IO*/
bool procuraEPedeIO(Processo *processo, int tempo, FILE *f) {
	int i;
	bool achouIO = false;
	TempoChamadaIO tempoChamada;
	for(i = 0; i < processo->quantidadeChamadas; i++) {
		if(processo->chamada[i].tempoBloqueio == processo->tempoExecutado)	{
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
			pedirIO(processo, tiposIO[DISCO], &filaDisco);
			break;
		case IMPRESSORA:
			pedirIO(processo, tiposIO[IMPRESSORA], &filaImpressora);
			break;
		case FITA_MAGNETICA:
			pedirIO(processo, tiposIO[FITA_MAGNETICA], &filaFita);
			break;
		default:
			fprintf(f,"ERROR: Tipo I/O %d inválido\n", tempoChamada.tipoIO.tipoIO);
			break;
	}
	return true;
}
/*Soma 1 ao tempo de bloqueio dos processos em uma fila de IO*/
void updateFilaDeIO(IO tipo, FIFO *filaIO, FILE *f) {
	if(!empty(filaIO)) {
		int i = filaIO->head;
		while(true) {
			filaIO->queue[i]->tempoBloqueado++;
			if(i==filaIO->tail) {
				break;
			}
			i=(i+1)%MAX_PROCESSOS;
		}

		i = filaIO->head;
		while(!empty(filaIO) && filaIO->queue[filaIO->head]->tempoBloqueado >= tipo.tempo) {
			Processo *process = pop(filaIO);
			fprintf(f, "Desbloqueando processo PID: %d do IO %s\n", process->PID, tipo.nomeTipo);
			unblockProcess(process);
			if(tipo.vaiPraAlta) add(&altaPrioridade, process);
			else add(&baixaPrioridade, process);
		}
	}
}

void imprimeFilaIO(IO tipo, FIFO *filaIO, FILE *f) {
	fprintf(f,"Fila de bloqueados IO %s (Tempo %d): [", tipo.nomeTipo, tipo.tempo);
	if(!empty(filaIO)) {
		int i = filaIO->head;
		while(true) {
			fprintf(f," {PID: %d, tempo: %d}",  filaIO->queue[i]->PID, filaIO->queue[i]->tempoBloqueado);			
			if(i==filaIO->tail) {
				break;
			}
			i=(i+1)%MAX_PROCESSOS;
		}
	}
	fprintf(f,"]\n");
}

void imprimeTodasAsFilas(FILE *f) {
	imprimeFilaIO(tiposIO[DISCO], &filaDisco, f);
	imprimeFilaIO(tiposIO[IMPRESSORA], &filaImpressora, f);
	imprimeFilaIO(tiposIO[FITA_MAGNETICA], &filaFita, f);
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
