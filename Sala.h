#ifndef SALA_H
#define SALA_H

#include "Evento.h"
#include "Fila_Eventos.h"

typedef struct sala* Sala;

Sala criarSala(char* nome);
char* getNomeSala(Sala s);

int agendarEventoSala(Sala s, Evento e);
int verificarConflitoSala(Sala s, Evento e);

void imprimirSala(Sala s);
void apagarSala(Sala s);

void removerEventoSala(Sala s, int ano, int mes, int dia, int hora, int min);
void removerEventosAnterioresSala(Sala s, int ano, int mes, int dia);

int obterQtdEventosSala(Sala s);
void extrairEventosSalaParaArray(Sala s, Evento* array_destino, int* indice_atual);

#endif
