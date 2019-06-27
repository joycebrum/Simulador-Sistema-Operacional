#ifndef lru
#define lru

#include "variables.h"

/* 
 * queue: vetor onde serão colocados os processos
 * numberOfElements: número de processos na fila
 * head: aponta para o primeiro da fila
 * tail: aponta para o último da fila, ou -1 se a fila estiver vazia*/
typedef struct _LRU {
	int queue[WSL];
    int numberOfElements;
    int head;
    int tail;
}GerenciadorPaginas;

/*Inicializa a fila*/
void initLRU(GerenciadorPaginas *f) {
    f->head = 0;
    f->tail = -1;
    f->numberOfElements = 0;
}
/*Verifica se a fila está vazia*/
int isEmptyLRU(GerenciadorPaginas *f) {
    return f->numberOfElements==0;
}
/*Verifica se a fila está cheia*/
int isFullLRU(GerenciadorPaginas *f) {
    return f->numberOfElements==WSL;
}

/*Se a fila não estiver cheia, insere um elemento no fim da fila*/
void addLRU(GerenciadorPaginas *f, int element) {
	if(!isFullLRU(f)){
		f->tail=(f->tail+1)%WSL;
		f->queue[f->tail] = element;
		f->numberOfElements++;
	}
}
/*Se a fila não estiver vazia, remove o primeiro elemento da fila*/
int popLRU(GerenciadorPaginas *f) {
	if(!isEmptyLRU(f)){
		int elemento = f->queue[f->head];
		f->head = (f->head+1)%WSL;
		f->numberOfElements--;
		return elemento;
	}
	return -1;
}

void updatePagina(int position) {
	
}

//retorna o numero da pagina que será substituida
// -1 caso nenhuma seja substituida
int referenciarPagina(GerenciadorPaginas *f, int pagina) {
	if(isFullLRU(f)) {
		int i = f->head;
		while(true) {
			if(f->queue[i] == pagina) {
				updatePagina(i);
				return -1;
			}			
			if(i==f->tail) {
				int removida = popLRU(f);
				addLRU(f, pagina);
				return removida;
			}
			i=(i+1)%WSL;
		}
	}
	else {
		addLRU(f, pagina);
		return -1;
	}
}

#endif
