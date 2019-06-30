#ifndef lru
#define lru


#include <stddef.h>
#include <stdlib.h>

#include "variables.h"

/* 
 * queue: vetor onde serão colocados os processos
 * numberOfElements: número de processos na fila
 * head: aponta para o primeiro da fila
 * tail: aponta para o último da fila, ou -1 se a fila estiver vazia*/

/*Inicializa a fila*/
void initLRU(GerenciadorPaginas *f) {
    f->numberOfElements = 0;
    f->head = NULL;
    f->tail = NULL;
}
/*Verifica se a fila está vazia*/
int isEmptyLRU(GerenciadorPaginas *f) {
    return f->numberOfElements == 0;
}
/*Verifica se a fila está cheia*/
int isFullLRU(GerenciadorPaginas *f) {
    return f->numberOfElements == WSL;
}

/*Se a fila não estiver cheia, insere um elemento no fim da fila*/
void addLRU(GerenciadorPaginas *f, int element) {
	if(!isFullLRU(f)){
		No *novo = (No *) malloc(sizeof(No));
		novo->proximo = NULL;
		novo->valor = element;
		novo->anterior = NULL;
		if(isEmptyLRU(f)) {
			f->head = novo;
			f->tail = f->head;
		}
		else {
			novo->anterior = f->tail;
			f->tail->proximo = novo;
			f->tail = novo;
		}
		f->numberOfElements++;
	}
}
/*Se a fila não estiver vazia, remove o primeiro elemento da fila*/
int popLRU(GerenciadorPaginas *f) {
	if(!isEmptyLRU(f)){
		No *elemento = f->head;
		f->head = f->head->proximo;
		f->numberOfElements--;
		return elemento->valor;
	}
	return -1;
}

void removeLRU(GerenciadorPaginas *f, No* no) {
	No *anterior = no->anterior;
	No *proximo = no->proximo;
	if(anterior != NULL) {
		anterior->proximo = proximo;
	}
	if(proximo != NULL) {
		proximo->anterior = anterior;
	}
	if(f->head == no) {
		f->head = proximo;
	}
	if(f->tail == no) {
		f->tail = anterior;
	}
}

void updatePageLRU(GerenciadorPaginas *f, int pagina) {
	No *atual = f->head;
	while(atual != NULL) {
		if(atual->valor == pagina) {
			removeLRU(f, atual);
			break;
		}
		atual = atual->proximo;
	}	
	addLRU(f, pagina);
}

int loadPageLRU(GerenciadorPaginas *f, int pagina) {
	if(isFullLRU(f)) {
		int pageRemoved = popLRU(f);
		addLRU(f, pagina);
		return pageRemoved;
	} else {
		addLRU(f, pagina);
		return -1;
	}
}

#endif
