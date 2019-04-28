#ifndef fifo
#define fifo

/* FIFO: First in first out, modelo padrão de fila
 * queue: vetor onde serão colocados os processos
 * numberOfElements: número de processos na fila
 * head: aponta para o primeiro da fila
 * tail: aponta para o último da fila, ou -1 se a fila estiver vazia*/
typedef struct _FIFO {
	int queue[MAX_PROCESSOS];
    int numberOfElements;
    int head;
    int tail;
}FIFO;

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
void add(FIFO *f, int element) {
	if(!full(f)){
		f->tail=(f->tail+1)%MAX_PROCESSOS;
		f->queue[f->tail] = element;
		f->numberOfElements++;
	}
	else puts("Fila cheia");    
}
/*Se a fila não estiver vazia, remove o primeiro elemento da fila*/
int pop(FIFO *f) {
	if(!empty(f)){
		int elemento = f->queue[f->head];
		f->head = (f->head+1)%MAX_PROCESSOS;
		f->numberOfElements--;
		return elemento;
	}
	else puts("Fila vazia");
	return -1;
}
/*Printa o estado atual da fila*/
void print(FIFO *f) {
	int i = f->head;
	printf("[");
	while(1) {
		if(i!=f->tail){
			printf("%d,", f->queue[i]);
			i=(i+1)%MAX_PROCESSOS;
		}
		else {
			printf("%d]\n\n", f->queue[i]);
			break;
		} 
	}
}
