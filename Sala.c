#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Sala.h"


struct sala {
    char* nome; // Identificador da sala
    Fila fila_eventos; // reutiliza o TAI Fila_Eventos
};


// Criar uma sala para armazenar os eventos
Sala criarSala(char* nome) {

	Sala s = (Sala)malloc(sizeof(struct sala));

	if (s == NULL) return NULL;

	s->nome = strdup(nome); // Faz uma cópia do nome
	s->fila_eventos = criarFila(); // Fila para a sala

	return s;
}


// Obter o nome da sala
char* getNomeSala(Sala s) {
	if (s == NULL) return NULL;
	return s->nome;
}


// Agendar eventos, caso não exista sobreposição
int agendarEventoSala(Sala s, Evento e) {
	if (s == NULL || e == NULL) return 0;

	if (verificarConflitoNaFila(s->fila_eventos, e)) {
		return 0;
	}

	inserirNaFila(s->fila_eventos, e);
	return 1;
}


// Verifica se há conflitos com os eventos desta sala
int verificarConflitoSala(Sala s, Evento e) {
	if (s == NULL || e == NULL) return 0;

	return verificarConflitoNaFila(s->fila_eventos, e);
}


// Imprimir eventos de cada sala
void imprimirSala(Sala s) {
	if (s == NULL) return;
	
	imprimirFila(s->fila_eventos);
}


// Apagar evento de cada sala
void apagarSala(Sala s) {
	if (s == NULL) return;

	if (s->nome != NULL) free(s->nome);
	if (s->fila_eventos != NULL) apagarFila(s->fila_eventos);

	free(s);
}




// Remover eventos da sala por data
void removerEventoSala(Sala s, int ano, int mes, int dia, int hora, int min) {
	if (s == NULL) return;

	removerEventoFila(s->fila_eventos, ano, mes, dia, hora, min);
}




// Remover Eventos da Sala anteriores a uma data
void removerEventosAnterioresSala(Sala s, int ano, int mes, int dia) {
	if (s == NULL) return;

	removerEventosAnterioresFila(s->fila_eventos, ano, mes, dia);
}




// Devolve a qtd de eventos que esta sala tem agendados
int obterQtdEventosSala(Sala s) {
	if (s == NULL) return 0;

	return obterQtd(s->fila_eventos);
}


// Extrai os eventos desta sala para um array
void extrairEventosSalaParaArray(Sala s, Evento* array_destino, int* indice_atual) {
	if (s == NULL) return;

	extrairEventosParaArray(s->fila_eventos, array_destino, indice_atual);
}