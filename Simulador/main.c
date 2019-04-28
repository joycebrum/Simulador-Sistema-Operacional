#include <stdio.h>
#include <stdbool.h>
#include "variables.h"
#include "processos.h"
#include "blockedqueue.h"

void testaCriacaoDeInstancia() {
	InstanciaProcesso processo = createNewProcess(10, 0);
	printf("%d, %d\n", processo.PID, processo.tempoServico);

	int quantidade = processo.quantidadeChamadas;
	printf("quantidade IO %d \n", quantidade);
	int i = 0;
	for (i = 0; i < quantidade; i++) {
		printf("%d, %d\n",  processo.chamada[i].tipoIO.tipoIO, processo.chamada[i].tempoBloqueio);
	}
}

void testaListaEncadeada() {
	Bloqueados bloqueados;
	init(&bloqueados);
	add(&bloqueados, 35);
	add(&bloqueados, 40);
	add(&bloqueados, 42);
	add(&bloqueados, 47);
	
	printBloqueados(bloqueados);

	remover(&bloqueados, 42);
	printBloqueados(bloqueados);
	
}

int main () {
	initSrand();
	testaListaEncadeada();
}
