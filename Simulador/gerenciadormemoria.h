#ifndef gerenciadormemoria
#define gerenciadormemoria

#include <stdio.h>

#include "variables.h"
#include "lru.h"


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

void alocarFrame(Processo *processo, int pagina, FILE *f) {
	int frame = first_fit();
	if(frame == -1) {
		//swap();
		return;
		frame = first_fit();
	}
	
	fprintf(f, "-> Página alocada no frame %d\n", frame);
	processo->numPaginasAlocadas++;
	processo->tabelaPaginas[pagina].num_frame = frame;	
}

/*Aloca página na tabela de páginas do processo*/
void loadPage(Processo* processo, int pagina, FILE *f) {
	int paginaRemovida = loadPageLRU(processo->gerenciadorPaginas, pagina);
	if(paginaRemovida != -1) {
		removePage(processo, paginaRemovida, pagina, f);
	} else {
		alocarFrame(processo, pagina, f);
	}
}

void verificaECarregaPagina(Processo* processo, int pagina, FILE *f){
	if(processo->tabelaPaginas[pagina].num_frame == -1){
		fprintf(f, " Page Fault ");
		loadPage(processo, pagina, f);
	}
	else {
		fprintf(f, " Page Hit\n");
		updatePageLRU(processo->gerenciadorPaginas, pagina);
	}
}

void gerenciaMemoria(Processo *processo, FILE *f) {
	if(processo->tempoExecutado % 3 == 0) {
		processo->paginasReferenciadas.ultimaPaginaReferenciada++;
		int paginaReferenciada = processo->paginasReferenciadas.vetor[processo->paginasReferenciadas.ultimaPaginaReferenciada];	
		fprintf(f, "Referencia à página %d - ", paginaReferenciada);
		verificaECarregaPagina(processo, paginaReferenciada, f);
	}
}

#endif
