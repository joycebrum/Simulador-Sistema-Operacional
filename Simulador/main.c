#include <stdio.h>
#include <stdbool.h>
#include "variables.h"
#include "processos.h"
#include "fifo.h"
#include "gerenciadorfilas.h"

int tempoDecorrido;
int tempoExecutando;
int tempoGeracaoProcessos[MAX_PROCESSOS];
int proximoProcessoASerCriado = 0;
Processo *processoExecutando;
bool processadorExecutando;
int tempoExecutadoProcessador;

void inicializacao();
void executarProcesso();
void geraTempoAleatorioParaCriacaoProcessos();
void escalonarProcesso();
void processador();
void criaProcessosSeHouverDemanda();

int main () {
	inicializacao();
	geraTempoAleatorioParaCriacaoProcessos();
	while(1) {

		updateBlockedProcesses();
		
		criaProcessosSeHouverDemanda();

		processador();

		if(!processadorExecutando) {
			escalonarProcesso();
		}

		atualizarTempoEsperaProcessosReady();

		tempoDecorrido = tempoDecorrido + 1;
	}
}

void inicializacao() {
	initSrand();
	initFilas();
	tempoDecorrido = 0;
	tempoExecutando = 0;
	proximoProcessoASerCriado = 0;
	processadorExecutando = 0;
	tempoExecutadoProcessador = 0;
}

void executarProcesso() {
	pedirIO(processoExecutando, tempoDecorrido);
	processoExecutando->tempoExecutado++;
	tempoExecutadoProcessador++;
}

void escalonarProcesso() {
	processadorExecutando = 1;
	tempoExecutadoProcessador = 0;
	processoExecutando = selecionarProximoProcessoAExecutar();
}

void processador() {

	if(tempoExecutadoProcessador > TEMPO_RR) {
		interromperProcesso(processoExecutando);
		processoExecutando = NULL;
		processadorExecutando = 0;
	}
	else {
		executarProcesso();
	}
}

void geraTempoAleatorioParaCriacaoProcessos() {
	//TODO preencher o vetor tempoGeracaoProcessos
}

void criaProcessosSeHouverDemanda() {
	if(tempoGeracaoProcessos[proximoProcessoASerCriado] == tempoDecorrido) {
		adicionarProcessoNovo(createNewProcess(5, 0, tempoDecorrido));
		proximoProcessoASerCriado++;
	}
}
