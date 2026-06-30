/*
Trabalho Prático 2

Trabalho Realizado por:

Filipe Assunção - 26696
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "DataHora.h"
#include "Evento.h"
#include "Fila_Eventos.h"
#include "Sala.h"
#include "Agenda.h"

#define BUFFER_SIZE 2048

// Buffer geral para ler string.
char buffer[BUFFER_SIZE];


char* mystrcopyTrim(char* str) {
	int countLeft = 0;
	int countRight = 0;
	int i;
	int tam = strlen(str);

	// conta os brancos à esquerda
	for (i = 0; i < tam; i++)
		if (str[i] == ' ')
			countLeft++;
		else
			break;

	// conta os brancos à direita
	for (i = tam - 1; i >= 0; i--)
		if (str[i] == ' ')
			countRight++;
		else
			break;

	tam = tam - countLeft - countRight;
	tam = tam < 0 ? 0 : tam;

	char* s2 = malloc(sizeof(char) * (tam + 1));

	if (tam > 0)
		strncpy(s2, str + countLeft, tam);
	s2[tam] = '\0';

	return s2;
}


// Ler a informação de uma linha de eventos
void leLinhaEventos(FILE* fic, Agenda a)
{
	char* sala;
	char* tema;
	char* docente;

	int ano, mes, dia, hora, min, duracao;

	// Ler a data, hora e duração (note o espaço depois o último %i)
	fscanf(fic, "%d%d%d%d%d%d ", &ano, &mes, &dia, &hora, &min, &duracao);
	fgets(buffer, sizeof(buffer), fic);
	buffer[strcspn(buffer, "\r\n")] = 0;
	//buffer[strlen(buffer) - 1] = '\0';


	// Separa a string nas 2 partes
	sala = strtok(buffer, ":");
	tema = strtok(NULL, ":");
	docente = strtok(NULL, ":");


	// opcional: retirar os espaços à esquerda e à direita
	sala = mystrcopyTrim(sala);
	tema = mystrcopyTrim(tema);
	docente = mystrcopyTrim(docente);

	
	//---------------------------------------
	// Cria e preenche a Data e Hora
	DataHora dh = criarDataHora();
	definirDataHora(dh, ano, mes, dia, hora, min);

	// Cria e preenche o Evento
	Evento e = criarEvento();
	definirEvento(e, duracao, sala, tema, docente, dh);

	// Manda o evnto para a agenda.c
	registrarNovoEvento(a, e);
	//---------------------------------------


	// quando não forem necessárias, libertar a memória alocada para as strings pela função mystrcopyTrim
	free(sala);
	free(tema);
	free(docente);
	
}



// Modo interativo (linha de comandos)
void executarModoInterativo(Agenda a, char* nome_ficheiro_agenda) {
	char comando[20];
	char sala[50];
	int ano, mes, dia, hora, min, duracao, n;

	char buffer_cmd[BUFFER_SIZE];

	printf("\nModo Interativo. Selecione um comando: ");

	// Sai quando encontrar o "exit"
	while (1) {
		scanf("%s", comando);

		// (exit) Sair do menu
		if (strcmp(comando, "exit") == 0) {
			printf("A encerrar e a guardar dados.");

			// Apanha o nome do ficheiro e copia
			char nome_saida[256];
			strcpy(nome_saida, nome_ficheiro_agenda);

			// Encontra o .txt e adiciona antes -new.txt
			char* ponto = strrchr(nome_saida, '.'); // strrchr localiza o carácter em específico
			if (ponto != NULL) {
				strcpy(ponto, "-new.txt");
			}
			else {
				strcat(nome_saida, "-new.txt"); // Prevenção de erro (se der erro acima)
			}

			guardarAgendaFicheiro(a, nome_saida); // Grava no novo ficheiro
			break;
		}


		// (del) Apagar evento
		else if (strcmp(comando, "del") == 0) {
			scanf("%d %d %d %d %d %s", &ano, &mes, &dia, &hora, &min, sala);
			printf("A apagar evento na sala %s em %d/%d/%d %d:%d\n", sala, dia, mes, ano, hora, min);

			apagarEventoAgenda(a, sala, ano, mes, dia, hora, min);
		}

		// (delBefore) Apagar os eventos anteriores ou iguais a uma determinada data
		else if (strcmp(comando, "delBefore") == 0) {
			scanf("%d %d %d", &ano, &mes, &dia);
			printf("A apagar eventos anteriores a %d/%d/%d\n", dia, mes, ano);

			apagarEventosAnteriores(a, ano, mes, dia);
		}

		// (listRoom) Listar os eventos de uma sala
		else if (strcmp(comando, "listRoom") == 0) {
			scanf("%s", sala);

			printf("A listar eventos da sala %s:\n", sala);
			listarEventosSala(a, sala);
		}


		// (listUntil) Listar todos os eventos até ao momento (todas as salas)
		else if (strcmp(comando, "listUntil") == 0) {
			scanf("%d %d %d %d %d", &ano, &mes, &dia, &hora, &min);
			printf("A listar eventos ate %04d/%02d/%02d %02d:%02d\n", ano, mes, dia, hora, min);

			// Listar todos até á data
			listarEventosAte(a, ano, mes, dia, hora, min);
		}


		// (listBetween) Listar os eventos entre 2 datas
		else if (strcmp(comando, "listBetween") == 0) {

			int ano2, mes2, dia2;

			scanf("%d %d %d %d %d %d", &ano, &mes, &dia, &ano2, &mes2, &dia2);
			printf("A listar eventos entre %d/%d/%d e %d/%d/%d:\n", dia, mes, ano, dia2, mes2, ano2);
			
			listarEventosDatas(a, ano, mes, dia, ano2, mes2, dia2);
		}


		// (count) contar os eventos
		else if (strcmp(comando, "count") == 0) {
			contarEventosSalas(a);
		}

		// (Listin) Listar a decorrer
		else if (strcmp(comando, "Listin") == 0) {
			scanf("%d %d %d %d %d", &ano, &mes, &dia, &hora, &min);
			printf("Eventos em %04d/%02d/%02d %02d:%02d:\n", ano, mes, dia, hora, min);

			listarEventosADecorrer(a, ano, mes, dia, hora, min);
		}


		// Erro
		else {
			printf("Erro: Comando '%s' nao reconhecido.\n", comando);

			while (getchar() != '\n'); // Limpar Buffer
		}
	}
}



// Main
int main(int argc, char* argv[])
{

	FILE* fagenda;
	FILE* feventos;

	int i, nEventos, nSalas;


	// Verificar se o programa é chamado com o número correto de argumento
	if (argc != 3) {
		printf("Numero de argumentos errados! %i \n", argc);
		exit(0);
	}

	// Verificar se os 2 ficheiros são abertos corretamente
	fagenda = fopen(argv[1], "r");
	if (fagenda == NULL) {
		printf("Ficheiro agenda não encontrado.\n");
		exit(0);
	}

	feventos = fopen(argv[2], "r");
	if (feventos == NULL) {
		printf("Ficheiro eventos não encontrado.\n");
		exit(0);
	}

	

	// **** Processar o ficheiro da agenda ****

	printf("\nConteudo do ficheiro AGENDA\n");

	// Ler o número de eventos e o número de salas no ficheiro agenda (note o espaço depois do %i)
	fscanf(fagenda, "%i%i ", &nEventos, &nSalas);

	Agenda minha_agenda = criarAgenda(nSalas);

	printf("Número de Eventos: %i\n", nEventos);
	printf("Número de Salas: %i\n", nSalas);

	// Ler todos os eventos da Agenda
	for (i = 0; i < nEventos; i++)
		leLinhaEventos(fagenda, minha_agenda);



	// **** Processar o ficheiro da eventos ****

	printf("\nConteudo do ficheiro EVENTOS\n");
	// Ler o número de eventos no ficheiro eventos (note o espaço depois do %i)
	fscanf(feventos, "%i ", &nEventos);

	printf("Número de Eventos: %i\n", nEventos);

	// Ler todos os eventos
	for (i = 0; i < nEventos; i++)
		leLinhaEventos(feventos, minha_agenda);


	// Fechar os ficheiro quando não foremnecessários
	fclose(fagenda);
	fclose(feventos);


	// Executar Modo Interativo
	executarModoInterativo(minha_agenda, argv[1]);

	apagarAgenda(minha_agenda);

	return 0;
}
