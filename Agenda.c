#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Agenda.h"


struct agenda {
    Sala* array_salas; // Ponteiro para a Sala (Array Dinâmico)
    int max_salas; // Limite de Salas
    int contagem_salas; // Qtd de salas registadas
};


// Criar uma agenda dinâmica
Agenda criarAgenda(int max_salas) {
    if (max_salas <= 0) return NULL;

    Agenda a = (Agenda)malloc(sizeof(struct agenda));
    if (a == NULL) return NULL;

    a->max_salas = max_salas;
    a->contagem_salas = 0;

    // Alocar espaço para o Array de Salas (max_salas)
    a->array_salas = (Sala*)malloc(sizeof(Sala) * max_salas);
    if (a->array_salas == NULL) {
        free(a);
        return NULL;
    }

    // Inicializar a NULL
    for (int i = 0; i < max_salas; i++) {
        a->array_salas[i] = NULL;
    }

    return a;
}


// Função privada (procura se já existe a sala no Array)
int encontrarPosicaoSala(Agenda a, char* nome_sala) {
    for (int i = 0; i < a->contagem_salas; i++) {
        if (strcmp(getNomeSala(a->array_salas[i]), nome_sala) == 0) {
            return i;
        }
    }

    return -1;
}




//----------------------------- Métodos Privados

// Método de organização (Insertion Sort) para ordenar os ponteiros nas listagens
void ordenarArrayEventos(Evento* array, int tamanho) {

    for (int i = 1; i < tamanho; i++) {

        Evento chave = array[i];
        int j = i - 1;

        // Vai recuando enquanto o elemento anterior for maior
        while (j >= 0) {

            DataHora dh_j = getDataHora(array[j]);
            DataHora dh_chave = getDataHora(chave);

            int deve_trocar = 0;

            // Data e Hora
            if (verMaior(dh_j, dh_chave)) {
                deve_trocar = 1;
            }

            // Se for igual, ordena por ordem alfabética
            else if (verIgual(dh_j, dh_chave)) {
                if (strcmp(getSala(array[j]), getSala(chave)) > 0) {
                    deve_trocar = 1;
                }
            }

            if (deve_trocar) {
                array[j + 1] = array[j];
                j = j - 1;
            }
            else {
                break; // Está no sítio certo e para a procura
            }
        }

        array[j + 1] = chave;
    }
}


// Recolhe, ordena e devolve o array global (e o tamanho total)
Evento* obterArrayGlobalOrdenado(Agenda a, int* total_eventos) {

    *total_eventos = 0;

    // Descobre a qtd de eventos no total
    for (int i = 0; i < a->contagem_salas; i++) {
        *total_eventos += obterQtdEventosSala(a->array_salas[i]);
    }

    if (*total_eventos == 0) return NULL;

    // Cria o Array com o tamanho exato (isto seria uma memória temporária)
    Evento* array_global = (Evento*)malloc(sizeof(Evento) * (*total_eventos));
    if (array_global == NULL) return NULL;

    // Obtém as salas que estão lá dentro
    int indice_atual = 0;
    for (int i = 0; i < a->contagem_salas; i++) {
        extrairEventosSalaParaArray(a->array_salas[i], array_global, &indice_atual);
    }

    // Ordena os eventos agendados
    ordenarArrayEventos(array_global, *total_eventos);

    // Devolve tudo ordenado
    return array_global;
}

//---------------------------------




// Regista o evento e avisa o utilizador
void registrarNovoEvento(Agenda a, Evento e) {
    if (a == NULL || e == NULL) return;


    // Verificação Global (verifica todas as salas á procura de conflitos)
    for (int i = 0; i < a->contagem_salas; i++) {
        if (verificarConflitoSala(a->array_salas[i], e)) {
            printf("ERRO SOBREPOSICAO   ");
            imprimirEvento(e); // Imprimir após o erro
            apagarEvento(e);

            printf("\n");

            return;
        }
    }


    char* nome_sala = getSala(e);
    int posicao = encontrarPosicaoSala(a, nome_sala);

    // Se já existir a sala
    if (posicao != -1) {
        // Tenta agendar na fila dessa sala
        if (agendarEventoSala(a->array_salas[posicao], e)) {
            printf("REGISTADO\n");
        }
    }

    // Se ainda não existir a sala
    else {
        // Limite de salas atingido
        if (a->contagem_salas >= a->max_salas) {

            printf("ERRO SALA   ");
            imprimirEvento(e); // Imprimir após o erro

            printf("\n");

            // Dobra o tamanho do Nº Max de salas
            int novo_max = a->max_salas * 2;
            Sala* novo_array = (Sala*)malloc(sizeof(Sala) * novo_max);

            // Passa as salas do array antigo para o novo
            for (int i = 0; i < a->contagem_salas; i++) {
                novo_array[i] = a->array_salas[i];
            }

            // As novas posições iniciam a NULL
            for (int i = a->contagem_salas; i < novo_max; i++) {
                novo_array[i] = NULL;
            }

            free(a->array_salas); // Apaga o array antigo

            // Atualiza tudo da agenda.c
            a->array_salas = novo_array;
            a->max_salas = novo_max;
        }

        Sala nova_sala = criarSala(nome_sala);
        if (nova_sala == NULL) return;

        a->array_salas[a->contagem_salas] = nova_sala;
        a->contagem_salas++;

        agendarEventoSala(nova_sala, e);
        printf("REGISTADO\n");
    }
}


// Apagar a agenda (tudo)
void apagarAgenda(Agenda a) {
    if (a == NULL) return;

    for (int i = 0; i < a->contagem_salas; i++) {
        if (a->array_salas[i] != NULL) {
            apagarSala(a->array_salas[i]);
        }
    }

    free(a->array_salas);
    free(a);
}




//---------------------------------
// Parte da Interface


// (del) Apagar um evento específico de uma sala
void apagarEventoAgenda(Agenda a, char* nome_sala, int ano, int mes, int dia, int hora, int min) {
    if (a == NULL || nome_sala == NULL) return;

    int posicao = encontrarPosicaoSala(a, nome_sala);

    if (posicao != -1) {
        removerEventoSala(a->array_salas[posicao], ano, mes, dia, hora, min);

        printf("Sala %s apagada.\n", nome_sala);
    }
    else {
        printf("Aviso: A sala '%s' nao existe na agenda.\n", nome_sala);
    }
}


// (delBefore) Apagar eventos anteriores ou iguais a uma data em todas as salas
void apagarEventosAnteriores(Agenda a, int ano, int mes, int dia) {
    if (a == NULL) return;

    for (int i = 0; i < a->contagem_salas; i++) {
        removerEventosAnterioresSala(a->array_salas[i], ano, mes, dia);
    }
    printf("Eventos anteriores a %02d/%02d/%04d apagados.\n", dia, mes, ano);
}


// (listRoom) Listar todos os eventos de uma sala específica
void listarEventosSala(Agenda a, char* nome_sala) {
    if (a == NULL || nome_sala == NULL) return;

    int posicao = encontrarPosicaoSala(a, nome_sala);

    if (posicao != -1) {
        imprimirSala(a->array_salas[posicao]);
    }
    else {
        printf("Aviso: A sala '%s' nao existe na agenda.\n", nome_sala);
    }
}


// (listUntil) Listar todos os eventos até ao momento (todas as salas)
void listarEventosAte(Agenda a, int ano, int mes, int dia, int hora, int min) {
    if (a == NULL) return;

    int total_eventos;
    Evento* array = obterArrayGlobalOrdenado(a, &total_eventos);

    if (array == NULL) {
        printf("Nao existem eventos na agenda.\n");
        return;
    }

    DataHora alvo = criarDataHora();
    definirDataHora(alvo, ano, mes, dia, hora, min);

    int encontrou = 0;
    for (int i = 0; i < total_eventos; i++) {

        DataHora dh = getDataHora(array[i]);

        // Se passar da data procurada, para logo
        if (verMaior(dh, alvo)) {
            break;
        }

        imprimirEvento(array[i]);
        encontrou = 1;
    }

    if (!encontrou) printf("Sem eventos agendados ate a data e hora indicadas.\n");

    apagarDataHora(alvo);

    free(array);
}


// (listBetween) Listar os eventos entre 2 datas
void listarEventosDatas(Agenda a, int ano1, int mes1, int dia1, int ano2, int mes2, int dia2) {
    if (a == NULL) return;
    
    int total_eventos;
    Evento* array = obterArrayGlobalOrdenado(a, &total_eventos);

    if (array == NULL) return;

    DataHora alvo1 = criarDataHora();
    definirDataHora(alvo1, ano1, mes1, dia1, 0, 0);

    DataHora alvo2 = criarDataHora();
    definirDataHora(alvo2, ano2, mes2, dia2, 23, 59); // Fim do dia

    int encontrou = 0;
    for (int i = 0; i < total_eventos; i++) {

        DataHora dh = getDataHora(array[i]);

        // Se passar da 2º data, para logo
        if (verMaior(dh, alvo2)) {
            break;
        }

        // Se for >= data 1
        if (!verMenor(dh, alvo1)) {
            imprimirEvento(array[i]);
            encontrou = 1;
        }

    }

    if (!encontrou) printf("Sem eventos agendados entre as datas indicadas.\n");

    apagarDataHora(alvo1);
    apagarDataHora(alvo2);

    free(array);
}




// Guardar a agenda toda de volta no ficheiro .txt
void guardarAgendaFicheiro(Agenda a, char* nome_ficheiro) {
    if (a == NULL || nome_ficheiro == NULL) return;

    FILE* f = fopen(nome_ficheiro, "w"); // "agenda_new.txt"
    if (f == NULL) {
        printf("Erro ao criar o ficheiro de saida.\n");
        return;
    }
    
    int total_eventos;
    Evento* array = obterArrayGlobalOrdenado(a, &total_eventos); // Apanha tudo já ordenado

    // Escreve a primeira linha (total de evento e salas)
    fprintf(f, "%d %d\n", total_eventos, a->max_salas);

    // Escreve cada evento
    if (array != NULL) {
        for (int i = 0; i < total_eventos; i++) {

            DataHora dh = getDataHora(array[i]);
            Evento e = array[i];

            fprintf(f, "%04d %02d %02d %02d %02d %d %s : %s : %s\n",
                getAno(dh), getMes(dh), getDia(dh), getHora(dh), getMin(dh),
                getDuracao(e), getSala(e), getTema(e), getDocente(e));
        }

        free(array);
    }

    fclose(f);

    printf("Dados guardados com sucesso no ficheiro '%s'.\n", nome_ficheiro);
}


// (count) contar eventos da sala
void contarEventosSalas(Agenda a) {
    if (a == NULL) return;

    for (int i = 0; i < a->contagem_salas; i++) {
        char* nome = getNomeSala(a->array_salas[i]);

        int qtd = obterQtdEventosSala(a->array_salas[i]);

        printf("Sala %s: %d eventos\n", nome, qtd);
    }
}


// (Listin) listar eventos a decorrer num data
void listarEventosADecorrer(Agenda a, int ano, int mes, int dia, int hora, int min) {
    if (a == NULL) return;

    int total_eventos;
    Evento* array = obterArrayGlobalOrdenado(a, &total_eventos);

    if (array == NULL) {
        printf("Nao existem eventos na agenda.\n");
        return;
    }

    DataHora alvo = criarDataHora();
    definirDataHora(alvo, ano, mes, dia, hora, min);

    int encontrou = 0;
    for (int i = 0; i < total_eventos; i++) {

        DataHora inicio = getDataHora(array[i]);

        DataHora fim = criarDataHora();
        definirDataHora(fim, getAno(inicio), getMes(inicio), getDia(inicio), getHora(inicio), getMin(inicio));
        somarDataHora(fim, getDuracao(array[i]));

        if (!verMaior(inicio, alvo) && verMenor(alvo, fim)) {
            imprimirEvento(array[i]);
            printf("\n");
            encontrou = 1;
        }

        apagarDataHora(fim);
    }

    if (!encontrou) printf("Nenhum evento a decorrer.\n");

    apagarDataHora(alvo);
    free(array);
}