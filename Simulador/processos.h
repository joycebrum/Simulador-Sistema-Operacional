#ifndef processos
#define processos

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
	return newProcesso;
}

void increaseWaitTimeProcess(Processo *processo) {
	processo->tempoEspera++;
}

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

bool verificaSeProcessoTerminou(Processo *processo) {
	if(processo->tempoExecutado == processo->tempoServico) {
		endProcess(processo);
		return 1;
	}
	return 0;
}

// para exibição de resultado
void printProcesso(Processo processo) {
	printf("PID: %d, tempo serviço: %d\n", processo.PID, processo.tempoServico);

	int quantidade = processo.quantidadeChamadas;
	printf("quantidade IO %d \n", quantidade);
	int i = 0;
	for (i = 0; i < quantidade; i++) {
		printf("(%s, %d) ",  processo.chamada[i].tipoIO.nomeTipo, processo.chamada[i].tempoBloqueio);
	}
	printf("\n");
}

#endif
