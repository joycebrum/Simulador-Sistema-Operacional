#ifndef gerenciadormemoria
#define gerenciadormemoria

#include "variables.h"
#include "processos.h"
#include "lru.h"
#include "fifo.h"

void initMemoria(){ 
	for(int i=0;i<MEM_PRINCIPAL;i++){
		memoria[i] = -1; // Indica que o espaço está livre
	}
}

/*Insere uma página em um frame*/
int first_fit(){
	for(int i=0; i < MEM_PRINCIPAL; i++){
		if(memoria[i]==-1) {
			memoria[i] = 1;
			return i;
		}
	}
	return -1;
}

/*Remove alguma página do working set baseado no LRU*/
void removePage( Processo* processo, int paginaRemovida, int novaPagina, FILE *f) {
	int numeroFrame = processo->tabelaPaginas[paginaRemovida].num_frame;
	fprintf(f, "-> Substitui a página %d no frame %d\n", paginaRemovida, numeroFrame);
	processo->tabelaPaginas[paginaRemovida].num_frame = -1;
	processo->tabelaPaginas[novaPagina].num_frame = numeroFrame;
}
Processo* getMostRecentlyBlocked(FIFO f) {
	if(!empty(&f)) {
		int position = f.tail;
		while(position != f.head) {
			if(f.queue[position]->status == blocked) {
				return f.queue[position];
			}
			position = (position - 1)%MAX_PROCESSOS;
		}
		if(f.queue[position]->status == blocked) {
			return f.queue[position];
		}
	}
	return NULL;
}

Processo* getLastReadyProcess(FIFO f) {
	if(!empty(&f)) {
		int position = f.tail;
		while(position != f.head) {
			if(f.queue[position]->status == ready) {
				return f.queue[position];
			}
			position = (position - 1)%MAX_PROCESSOS;
		}
		if(f.queue[position]->status == ready) {
			return f.queue[position];
		}
	}
	return NULL;
}

// seleciona o processo que vai ficar mais tempo bloqueado, se não, seleciona o com menor prioridade
Processo* selecionaProcessoParaRemoverDaMemoria() {
	Processo *pImpressora = NULL;
	Processo *pDisc = NULL;
	Processo *pFita = NULL;
	
	pImpressora = getMostRecentlyBlocked(filaImpressora);
	pDisc = getMostRecentlyBlocked(filaDisco);
	pFita = getMostRecentlyBlocked(filaFita);
	
	if(pImpressora != NULL) {
		Processo *returned = pImpressora;
		int worstTime = tiposIO[IMPRESSORA].tempo - pImpressora->tempoBloqueado;
		
		if(pDisc != NULL) {
			if(worstTime < tiposIO[DISCO].tempo - pDisc->tempoBloqueado) {
				returned = pDisc;
				worstTime = tiposIO[DISCO].tempo - pDisc->tempoBloqueado;
			}
		} 
		if(pFita != NULL) {
			if(worstTime < tiposIO[FITA_MAGNETICA].tempo - pFita->tempoBloqueado) {
				returned = pFita;
				worstTime = tiposIO[FITA_MAGNETICA].tempo - pFita->tempoBloqueado;
			}
		}
		return returned;
	}
	
	else if(pDisc != NULL) {
		if(pFita != NULL) {
			if(tiposIO[DISCO].tempo - pDisc->tempoBloqueado < tiposIO[FITA_MAGNETICA].tempo - pFita->tempoBloqueado) {
				return pFita;
			}
		}
		return pDisc;
	} 
	else if(pFita != NULL) {
		return pFita;
	}
	
	if(!empty(&baixaPrioridade)) {
		return getLastReadyProcess(baixaPrioridade);
	} else if(!empty(&altaPrioridade)) {
		return getLastReadyProcess(altaPrioridade);
	}
	
	return NULL;
} 

void swapOut(FILE *f) {
	Processo *processoSwapped = selecionaProcessoParaRemoverDaMemoria();
	fprintf(f, "\nProcesso PID: %d swapped out. ", processoSwapped->PID);
	swapOutProcess(processoSwapped, f);
}

int alocarFrame(Processo *processo, int pagina, FILE *f) {
	int frame = first_fit();
	if(frame == -1) {
		swapOut(f);
		frame = first_fit();
	}
	processo->tabelaPaginas[pagina].num_frame = frame;	
	return frame;
}

void swapIn(Processo *processo, FILE *f) {
	if(!isEmptyLRU(processo->gerenciadorPaginas)) {
		fprintf(f, "Processo %d Swapped In ", processo->PID);
		No *atual = processo->gerenciadorPaginas->head;
		while(atual != NULL) {
			int frame = alocarFrame(processo, atual->valor, f);
			fprintf(f, "\nPáginas do processo %d carregadas: ", processo->PID);
			fprintf(f, "página %d no frame %d ", atual->valor, frame);
			atual = atual->proximo;
		}
		fprintf(f, "\n");
	} else {
		puts("Processo que sofreu swap não tinha páginas na LRU!");
		exit(0);
	}
}


/*Aloca página na tabela de páginas do processo*/
void loadPage(Processo* processo, int pagina, FILE *f) {
	int paginaRemovida = loadPageLRU(processo->gerenciadorPaginas, pagina);
	if(paginaRemovida != -1) {
		removePage(processo, paginaRemovida, pagina, f);
	} else {
		int frame = alocarFrame(processo, pagina, f);
		fprintf(f, "\nPágina %d do PID: %d a ser alocada no frame %d\n", pagina, processo->PID, frame);
	}
}

int verificaECarregaPagina(Processo* processo, int pagina, FILE *f){
	if(processo->tabelaPaginas[pagina].num_frame == -1){
		fprintf(f, " Page Fault - ");
		loadPage(processo, pagina, f);
		return 1;
	}
	else {
		fprintf(f, " Page Hit\n");
		updatePageLRU(processo->gerenciadorPaginas, pagina);
		return 0;
	}
}
void verificaSeFazSwapIn(Processo *processo, FILE *f) {
	if(processo->status == ready_suspend || processo->status == blocked_suspend) {
		swapIn(processo, f);
	}
	runProcess(processo);
}

//retorna 1 em caso de page fault
int gerenciaMemoria(Processo *processo, FILE *f) {
	int parteInteira = processo->tempoExecutado / 3;
	if(parteInteira > processo->paginasReferenciadas.ultimaPaginaReferenciada) {
		processo->paginasReferenciadas.ultimaPaginaReferenciada++;
		int paginaReferenciada = processo->paginasReferenciadas.vetor[processo->paginasReferenciadas.ultimaPaginaReferenciada];	
		fprintf(f, "Referencia à página %d - ", paginaReferenciada);
		return verificaECarregaPagina(processo, paginaReferenciada, f);
	}
	else {
		fprintf(f, "Processo %d não referenciou nenhuma página\n", processo->PID);
	}
	return 0;
}
#endif
