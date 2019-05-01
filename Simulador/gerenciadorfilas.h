#ifndef gerenciadorfilas
#define gerenciadorfilas

#include <stddef.h>

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
	// TODO Implementar esquema de aumento de prioridade com base no tempo de espera
	if(!empty(&altaPrioridade)) {
		int i = altaPrioridade.head;

		while(1) {
			if(i > altaPrioridade.tail){
				break;
			}
			
			Processo *processo = altaPrioridade.queue[i];
			increaseWaitTimeProcess(processo);
			
			
			i=(i+1)%MAX_PROCESSOS;
		}
	}
	if(!empty(&baixaPrioridade)) {
		int i = baixaPrioridade.head;

		while(1) {
			if(i > baixaPrioridade.tail){
				break;
			}
			
			Processo *processo = baixaPrioridade.queue[i];
			increaseWaitTimeProcess(processo);
			
			i=(i+1)%MAX_PROCESSOS;
		}
	}
	
}

void transferirFilaBaixaParaAlta() {
	while (!empty(&baixaPrioridade)) {
		Processo *processo = pop(&baixaPrioridade);
		add(&altaPrioridade, processo);
	}
}

Processo* selecionarProximoProcessoAExecutar() {
	if (empty(&altaPrioridade)) {
		if(empty(&baixaPrioridade)) {
			return NULL;
		}
		transferirFilaBaixaParaAlta();
	}
	
	Processo *processo = pop(&altaPrioridade);
	
	runProcess(processo);
	return processo;
}

void interromperProcesso(Processo *processo) {
	if(!verificaSeProcessoTerminou(processo)) {
		toReadyProcess(processo);
		add(&baixaPrioridade, processo);
	}
}

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
void updateFilaDeIO(IO tipo, FIFO *filaIO) {
	if(!empty(filaIO)) {
		int i = filaIO->head;
		while(1) {
			if(i > filaIO->tail){
				break;
			}
			filaIO->queue[i]->tempoBloqueado++;			
			i=(i+1)%MAX_PROCESSOS;
		}

		i = filaIO->head;
		while(filaIO->queue[i]->tempoBloqueado == tipo.tempo) {
			Processo *process = pop(filaIO);
			unblockProcess(process);
			if(tipo.vaiPraAlta) {
				add(&altaPrioridade, process);
			}
			else {
				add(&baixaPrioridade, process);
			}
		}
	}
}
void updateBlockedProcesses() {
	updateFilaDeIO(tiposIO[DISCO], &filaDisco);
	updateFilaDeIO(tiposIO[FITA_MAGNETICA], &filaFita);
	updateFilaDeIO(tiposIO[IMPRESSORA], &filaImpressora);
}
void adicionarProcessoNovo(Processo *processo) {
	add(&altaPrioridade, processo);
	toReadyProcess(processo);
}
#endif
