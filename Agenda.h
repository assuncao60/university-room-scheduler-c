#ifndef AGENDA_H
#define AGENDA_H

#include "Evento.h"
#include "Sala.h"

typedef struct agenda* Agenda;

Agenda criarAgenda(int max_salas);

void registrarNovoEvento(Agenda a, Evento e);

void apagarAgenda(Agenda a);


//------------------------------ Interface
// Apagar
void apagarEventoAgenda(Agenda a, char* sala, int ano, int mes, int dia, int hora, int min);
void apagarEventosAnteriores(Agenda a, int ano, int mes, int dia);

// Listagens das Salas
void listarEventosSala(Agenda a, char* nome_sala);

// Listagens Globais
void listarEventosAte(Agenda a, int ano, int mes, int dia, int hora, int min);
void listarEventosDatas(Agenda a, int ano1, int mes1, int dia1, int ano2, int mes2, int dia2);

void guardarAgendaFicheiro(Agenda a, char* nome_ficheiro);

void contarEventosSalas(Agenda a);

void listarEventosADecorrer(Agenda a, int ano, int mes, int dia, int hora, int min);

#endif
