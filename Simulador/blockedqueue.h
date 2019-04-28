#ifndef blockedqueue
#define blockedqueue

typedef struct _celula {
      int PID; 
      struct _celula *prox;
} Celula;

typedef struct _bloqueados {
	Celula *inicio;
	int tamanho;
} Bloqueados;
#endif

/*Inicializa a fila*/
void init(Bloqueados *bloqueados) {
    bloqueados->inicio = NULL;
    bloqueados->tamanho = 0;
}

void add(Bloqueados *bloqueados, int PID) {
	Celula celula;
	celula.PID = PID;
	celula.prox = NULL;

	if(bloqueados->inicio == NULL) {
		bloqueados->inicio = &celula;
		return;
	}
	
	Celula atual = *(bloqueados->inicio);
	while(atual.prox != NULL) {
		atual = *atual.prox;
	}
	atual.prox = &celula;
	bloqueados->tamanho++;
}

bool remover(Bloqueados *bloqueados, int PID) {
	Celula atual = *(bloqueados->inicio);
	if(atual.PID == PID) {
		bloqueados->inicio = atual.prox;
		bloqueados->tamanho--;
		return true;
	}
	Celula anterior;
	anterior = atual;
	atual = *atual.prox;
	
	while(atual.prox != NULL) {
		if(atual.PID == PID) {
			anterior.prox = atual.prox;
			bloqueados->tamanho--;
			return true;
		}
		anterior = atual;
		atual = *atual.prox
	}

	if(atual.PID == PID) {
		anterior.prox = atual.prox;
		bloqueados->tamanho--;
		return true;
	}
	
	return false;
}

/*Printa o estado atual da fila*/
void printBloqueados(Bloqueados bloqueados) {
	Celula atual = *bloqueados.inicio;
	printf("[");
	while(atual.prox != NULL) {
		printf(" %d,", atual.PID);
		atual = *atual.prox;
	}
	printf(" %d ]\n", atual.PID);
}

