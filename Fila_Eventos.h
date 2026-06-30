#ifndef FILA_EVENTOS_H
#define FILA_EVENTOS_H

#include "Evento.h"

typedef Evento Item;
typedef struct fila_controlo* Fila;

Fila criarFila();
void apagarFila(Fila f);

void inserirNaFila(Fila f, Item item);
Item retirarDaFila(Fila f);

void imprimirFila(Fila f);

int verificarConflitoNaFila(Fila f, Evento novo_evento);
int isEmpty(Fila f);
int obterQtd(Fila f);

void removerEventoFila(Fila f, int ano, int mes, int dia, int hora, int min);
void removerEventosAnterioresFila(Fila f, int ano, int mes, int dia);

void extrairEventosParaArray(Fila f, Evento* array_destino, int* indice_atual);

#endif