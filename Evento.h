#ifndef EVENTO_H
#define EVENTO_H

#include "DataHora.h"

typedef struct evento* Evento;

Evento criarEvento();

int getDuracao(Evento e);
char* getSala(Evento e);
char* getTema(Evento e);
char* getDocente(Evento e);
DataHora getDataHora(Evento e);

void definirEvento(Evento e, int duracao, char* sala, char* uc, char* docente, DataHora dh);
void imprimirEvento(Evento e);

void apagarEvento(Evento e);

int verificarConflito(Evento e1, Evento e2);

#endif