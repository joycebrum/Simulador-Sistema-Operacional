#ifndef gerenciadormemoria
#define gerenciadormemoria

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
void removePage( Processo* processo, int paginaRemovida, int novaPagina) {
	int numeroFrame = processo->tabelaPaginas[paginaRemovida].num_frame;
	processo->tabelaPaginas[paginaRemovida].num_frame = -1;
	processo->tabelaPaginas[novaPagina].num_frame = numeroFrame;
}

void alocarFrame(Processo *processo, int pagina) {
	int frame = first_fit();
	if(frame == -1) {
		//swap();
		return;
		frame = first_fit();
	}
	processo->numPaginasAlocadas++;
	processo->tabelaPaginas[pagina].num_frame = frame;	
}

/*Aloca página na tabela de páginas do processo*/
void loadPage(Processo* processo, int pagina) {
	int paginaRemovida = loadPageLRU(processo->gerenciadorPaginas, pagina);
	if(paginaRemovida != -1) {
		removePage(processo, paginaRemovida, pagina);
	} else {
		alocarFrame(processo, pagina);
	}
}

void verificaECarregaPagina(Processo* processo, int pagina){
	if(processo->tabelaPaginas[pagina].num_frame == -1){
		loadPage(processo, pagina);
	}
	else {
		updatePageLRU(processo->gerenciadorPaginas, pagina);
	}
}

#endif
