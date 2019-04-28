#include <stdio.h>
#include "variables.h"

int PID = 10;

Processo createNewProcess(int priority, int PPID) {
	Processo newProcesso;

	newProcesso.priority = priority;
	newProcesso.PID = PID++;
	newProcesso.PPID = PPID;
	newProcesso.status = created;
	newProcesso.waitTime = 0;

	return newProcesso;
}

void increaseWaitTimeProcess(Processo *processo, float timePassed) {
	processo->waitTime += timePassed;
}

void resetWaitTimeProcess(Processo *processo) {
	processo->waitTime = 0;
}

void blockProcess(Processo *processo) {
	//TODO adiciona na fila de blocked e remove da fila de running
	processo->status = blocked;
}

void runProcess(Processo *processo) {
	//TODO remove da fila de waiting
	processo->status = running;
}
void endProcess(Processo *processo) {
	//TODO remove o processo da lista de processos ativos
	processo->status = terminated;
}
void interruptProcess(Processo *processo) {
	processo->status = waiting;
}
void unblockProcess(Processo *processo) {
	//TODO remove o processo da fila de bloqueados
	processo->status = waiting;
}


int main(int argc, char **argv) {
	Processo a = createNewProcess(10, 0);
	Processo b = createNewProcess(10, 0);
	printf("%d , %d", a.PID, b.PID);
	return 0;
}
