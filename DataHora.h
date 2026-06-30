#ifndef DATAHORA_H
#define DATAHORA_H

typedef struct data_hora* DataHora;

DataHora criarDataHora();

int getAno(DataHora dh);
int getMes(DataHora dh);
int getDia(DataHora dh);
int getHora(DataHora dh);
int getMin(DataHora dh);

void definirDataHora(DataHora dh, int ano, int mes, int dia, int hora, int min);
void imprimirDataHora(DataHora dh);

void apagarDataHora(DataHora dh);

int verMenor(DataHora dh1, DataHora dh2);
int verMaior(DataHora dh1, DataHora dh2);
int verIgual(DataHora dh1, DataHora dh2);

int diasPorMes(int mes, int ano);
void somarDataHora(DataHora dh, int duracao);

#endif