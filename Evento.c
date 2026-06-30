#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Evento.h"
#include "DataHora.h"


struct evento {
	int duracao;
	char* sala;
	char* tema;
	char* docente;
	DataHora dataHora;
};


// Cria novo evento
Evento criarEvento() {

	Evento e = (Evento)malloc(sizeof(struct evento));

	if (e == NULL) return NULL;

	e->duracao = 0;
	e->sala = NULL;
	e->tema = NULL;
	e->docente = NULL;
	e->dataHora = NULL;

	return e;
}


// Gets
int getDuracao(Evento e) {
	return e->duracao;
}

char* getSala(Evento e) {
	return e->sala;
}

char* getTema(Evento e) {
	return e->tema;
}

char* getDocente(Evento e) {
	return e->docente;
}

DataHora getDataHora(Evento e) {
	return e->dataHora;
}


// Set (carregar os dados para o main)
void definirEvento(Evento e, int duracao, char* sala, char* tema, char* docente, DataHora data_hora) {
	if (e == NULL) return;

	e->duracao = duracao;

	// Faz uma cópia da string na RAM
	e->sala = strdup(sala);
	e->tema = strdup(tema);
	e->docente = strdup(docente);
	e->dataHora = data_hora; // Aponta para o TAI DataHora
}


// Imprimir Evento
void imprimirEvento(Evento e) {
	if (e == NULL) return;

	imprimirDataHora(e->dataHora);

	printf("%d %s : %s : %s", e->duracao, e->sala, e->tema, e->docente);
}


// Apagar evento
void apagarEvento(Evento e) {
	if (e == NULL) return;

	// Liberta as Stings
	if (e->sala != NULL) free(e->sala);
	if (e->tema != NULL) free(e->tema);
	if (e->docente != NULL) free(e->docente);

	// Liberta o TAI Data Hora
	if (e->dataHora != NULL) apagarDataHora(e->dataHora);

	free(e);
}


// Verificar se existe conflito entre eventos
int verificarConflito(Evento e1, Evento e2) {
	if (e1 == NULL || e2 == NULL) return 0;


	// Verifica se está na mesma sala ou se tem o mesmo docente
	if (strcmp(e1->sala, e2->sala) == 0 || strcmp(e1->docente, e2->docente) == 0) {

		// Fim do evento 1
		DataHora fim1 = criarDataHora();
		definirDataHora(fim1, getAno(e1->dataHora), getMes(e1->dataHora), getDia(e1->dataHora), getHora(e1->dataHora), getMin(e1->dataHora));
		somarDataHora(fim1, e1->duracao);

		// Fim do evento 2
		DataHora fim2 = criarDataHora();
		definirDataHora(fim2, getAno(e2->dataHora), getMes(e2->dataHora), getDia(e2->dataHora), getHora(e2->dataHora), getMin(e2->dataHora));
		somarDataHora(fim2, e2->duracao);

		// Verifica se existe sobreposição
		int conflito = 0;
		if (verMenor(e1->dataHora, fim2) && verMenor(e2->dataHora, fim1)) {
			conflito = 1;
		}

		// Limpa a memória
		apagarDataHora(fim1);
		apagarDataHora(fim2);

		return conflito; // Confirma-se conflito
	}

	return 0; // Não se confirma
}