#ifndef fifo
#define fifo

#include <stddef.h>
#include "processos.h"
#include "variables.h"

/* FIFO: First in first out, modelo padrão de fila
 * queue: vetor onde serão colocados os processos
 * numberOfElements: número de processos na fila
 * head: aponta para o primeiro da fila
 * tail: aponta para o último da fila, ou -1 se a fila estiver vazia*/


/*Inicializa a fila*/
void init(FIFO *f) {
    f->head = 0;
    f->tail = -1;
    f->numberOfElements = 0;
}
/*Verifica se a fila está vazia*/
int empty(FIFO *f) {
    return f->numberOfElements==0;
}
/*Verifica se a fila está cheia*/
int full(FIFO *f) {
    return f->numberOfElements==MAX_PROCESSOS;
}

/*Se a fila não estiver cheia, insere um elemento no fim da fila*/
void add(FIFO *f, Processo *element) {
	if(!full(f)){
		f->tail=(f->tail+1)%MAX_PROCESSOS;
		f->queue[f->tail] = element;
		f->numberOfElements++;
	}
	else puts("Fila cheia");    
}
/*Se a fila não estiver vazia, remove o primeiro elemento da fila*/
Processo* pop(FIFO *f) {
	if(!empty(f)){
		Processo *elemento = f->queue[f->head];
		f->head = (f->head+1)%MAX_PROCESSOS;
		f->numberOfElements--;
		return elemento;
	}
	else puts("Fila vazia");
	return NULL;
}
#endif
