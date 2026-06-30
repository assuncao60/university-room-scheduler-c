#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


typedef struct data_hora* DataHora;

struct data_hora {
    int ano;
    int mes;
    int dia;
    int hora;
    int min;
};


// Criar Data e Hora
DataHora criarDataHora() {

    // Memória é alocada para a struct
    DataHora dh = (DataHora)malloc(sizeof(struct data_hora));

    // Verifica se falha
    if (dh == NULL) return NULL;

    dh->ano = 0;
    dh->mes = 0;
    dh->dia = 0;
    dh->hora = 0;
    dh->min = 0;

    return dh;
}


// Getters
int getAno(DataHora dh) {
    if (dh == NULL) return 0;

    return dh->ano;
}

int getMes(DataHora dh) {
    if (dh == NULL) return 0;

    return dh->mes;
}

int getDia(DataHora dh) {
    if (dh == NULL) return 0;

    return dh->dia;
}

int getHora(DataHora dh) {
    if (dh == NULL) return 0;

    return dh->hora;
}

int getMin(DataHora dh) {
    if (dh == NULL) return 0;

    return dh->min;
}


// Set (carregar os dados para o main)
void definirDataHora(DataHora dh, int ano, int mes, int dia, int hora, int min) {
    if (dh == NULL)return;

    dh->ano = ano;
    dh->mes = mes;
    dh->dia = dia;
    dh->hora = hora;
    dh->min = min;
}


// Imprimir Data e Hora
void imprimirDataHora(DataHora dh) {
    if (dh == NULL) return;

    // Os %0...d vai garantir que as casas vazias são ocupadas por 0
    printf("%04d %02d %02d %02d %02d ", dh->ano, dh->mes, dh->dia, dh->hora, dh->min);
}


// Apagar Data e Hora
void apagarDataHora(DataHora dh) {
    if (dh == NULL) return;

    free(dh);
}


// Função para ver a menor
int verMenor(DataHora dh1, DataHora dh2) {
    if (dh1 == NULL || dh2 == NULL) return 0;

    // Ano
    if (dh1->ano != dh2->ano) return dh1->ano < dh2->ano;

    // Mês
    if (dh1->mes != dh2->mes) return dh1->mes < dh2->mes;

    // Dia
    if (dh1->dia != dh2->dia) return dh1->dia < dh2->dia;

    // Hora
    if (dh1->hora != dh2->hora) return dh1->hora < dh2->hora;

    // Minuto
    return dh1->min < dh2->min;
}


// Função para ver a maior
int verMaior(DataHora dh1, DataHora dh2) {
    if (dh1 == NULL || dh2 == NULL) return 0;

    // Ano
    if (dh1->ano != dh2->ano) return dh1->ano > dh2->ano;

    // Mês
    if (dh1->mes != dh2->mes) return dh1->mes > dh2->mes;

    // Dia
    if (dh1->dia != dh2->dia) return dh1->dia > dh2->dia;

    // Hora
    if (dh1->hora != dh2->hora) return dh1->hora > dh2->hora;

    // Minuto
    return dh1->min > dh2->min;
}


// Função para ver se são iguais
int verIgual(DataHora dh1, DataHora dh2) {
    if (dh1 == NULL || dh2 == NULL) return 0;

    
    if ((dh1->ano == dh2->ano) &&
        (dh1->mes == dh2->mes) &&
        (dh1->dia == dh2->dia) &&
        (dh1->hora == dh2->hora) &&
        (dh1->min == dh2->min)) {

        return 1;
    }

    return 0;
}


// Função Auxiliar para verificar os dias de cada mês
int diasPorMes(int mes, int ano) {
    int dias[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Verificação de Ano Bissexto
    if (mes == 2 && ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))) {
        return 29;
    }
    return dias[mes];
}


// Somar tempo de duração ao evento
void somarDataHora(DataHora dh, int duracao) {
    if (dh == NULL) return;

    // Adiciona a duração aos minutos
    dh->min += duracao;

    // Ajusta minutos e horas
    while (dh->min >= 60) {
        dh->min -= 60;
        dh->hora++;
    }

    // Ajusta horas e dias
    while (dh->hora >= 24) {
        dh->hora -= 24;
        dh->dia++;

        // Verificar dias do mes
        int dia_no_mes = diasPorMes(dh->mes, dh->ano);

        if (dh->dia > dia_no_mes) {
            dh->dia = 1; // O dia volta a 1
            dh->mes++;

            if (dh->mes > 12) {
                dh->mes = 1; // O mes volta a 1
                dh->ano++;
            }
        }
    }
}