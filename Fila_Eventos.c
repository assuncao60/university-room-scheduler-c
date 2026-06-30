#include <stdio.h>
#include <stdlib.h>

#include "Evento.h"
#include "Fila_Eventos.h"


// Ponteiro para a informação do evento
typedef Evento Item;

// Ponteiro para o node
typedef struct node* link;

// Ponteiro para a fila
typedef struct fila_controlo* Fila;


struct node {
    Item item;
    link next;
};


struct fila_controlo {
    link inicio;  // Aponta para o primeiro node da fila -> O(1)
    link fim; // Aponta para o último elemento da fila -> O(1)
    int count; // Contador
};


// Criar Node (Método privado / Não está no .h)
link criarNode(Item item) {

    link novo = (link)malloc(sizeof(struct node));
    if (novo == NULL) return NULL;

    novo->item = item;
    novo->next = NULL; // Fica livre (à espera do próximo)

    return novo;
}


// Criar Fila
Fila criarFila() {

    // Memória é alocada para a struct
    Fila f = (Fila)malloc(sizeof(struct fila_controlo));

    // Verifica se falha
    if (f == NULL) return NULL;

    f->inicio = NULL;
    f->fim = NULL;
    f->count = 0;

    return f;
}


// Apagar Fila
void apagarFila(Fila f) {
    if (f == NULL) return;

    // Enquanto não estiver vazia, vai retirando e apagando os nodes
    while (f->inicio != NULL) {
        link aux = f->inicio;
        f->inicio = aux->next;

        apagarEvento(aux->item);

        free(aux);
    }

    free(f);
}


// Inserir Item na fila
void inserirNaFila(Fila f, Item item) {
    if (f == NULL) return;

    // É criado o novo Node
    link novo_node = criarNode(item);
    if (novo_node == NULL) return; // Falhou ao criar

    // Se a fila estiver vazia
    if (f->count == 0) {
        f->inicio = novo_node;
        f->fim = novo_node;
    }
    // Se já houver Nodes anteriores
    else {
        f->fim->next = novo_node; // Liga-se ao novo
        f->fim = novo_node; // O novo passa a ser o último (simplesmente atualiza o ponteiro para a ultima posição)
    }

    // Contagem de Nodes
    f->count++;
}


// Retirar Item da Fila
Item retirarDaFila(Fila f) {
    if (isEmpty(f)) {
        return NULL;
    }

    // Guarda as informações de f->inicio em aux antes de serem destruídas
    link aux = f->inicio;
    Item informacao = aux->item;

    // Atualiza o Node (passando para a posição seguinte e esquece a atual)
    f->inicio = aux->next;
    f->count--;

    // Verifica novamente se tem lá algo (se não tiver aponta para NULL)
    if (f->count == 0) {
        f->fim = NULL;
    }

    free(aux);
    return informacao;
}


// Imprimir informações da fila
void imprimirFila(Fila f) {
    if (f == NULL || isEmpty(f)) {
        printf("Fila vazia.\n");
        return;
    }

    link aux = f->inicio;

    printf("\n+---+ Lista de Eventos +---+\n");
    while (aux != NULL) {
        imprimirEvento(aux->item);
        aux = aux->next;
    }
    printf("\n+--------------------------+\n");
}


// Verificar conflito na fila
int verificarConflitoNaFila(Fila f, Evento novo_evento) {
    if (f == NULL || f->inicio == NULL) return 0;

    // Guarda as informações de f->inicio em aux antes de serem destruídas
    link aux = f->inicio;

    while (aux != NULL) {
        if (verificarConflito(aux->item, novo_evento)) {
            return 1; // Encontrou conflito
        }

        // Passa para o próximo node
        aux = aux->next;
    }

    return 0; // Não existe conflito
}


// Verificar se está vazia
int isEmpty(Fila f) {
    if (f == NULL) return 1;

    return f->count == 0;
}


// Obter quantidade de elementos
int obterQtd(Fila f) {
    if (f == NULL) return 0;

    return f->count;
}



//Remover um evento se a data estiver dentro da sua Duração
void removerEventoFila(Fila f, int ano, int mes, int dia, int hora, int min) {
    if (f == NULL || f->inicio == NULL) return;

    link atual = f->inicio;
    link anterior = NULL;

    // A Data e Hora do utilizadpr é criada
    DataHora alvo = criarDataHora();
    definirDataHora(alvo, ano, mes, dia, hora, min);

    while (atual != NULL) {

        Evento e = atual->item;
        DataHora inicio = getDataHora(e);

        // Verifica o instante exato onde o evento termina
        DataHora fim = criarDataHora();
        definirDataHora(fim, getAno(inicio), getMes(inicio), getDia(inicio), getHora(inicio), getMin(inicio));
        somarDataHora(fim, getDuracao(e));


        // Se não for < que o inicio e > que o fim
        if (!verMenor(alvo, inicio) && !verMaior(alvo, fim)) {

            link apagar = atual;

            if (anterior == NULL) {
                f->inicio = atual->next; // Era o primeiro Node
            }
            else {
                anterior->next = atual->next; // Estava a meio
            }

            if (f->fim == apagar) {
                f->fim = anterior; // Era o último Node
            }

            f->count--;

            apagarEvento(apagar->item);
            free(apagar);
            apagarDataHora(fim);
            break;
        }

        apagarDataHora(fim); // Limpa antes de avançar para o próximo evento
        anterior = atual;
        atual = atual->next;
    }

    apagarDataHora(alvo);
}


// Remover Nodes anteriores ou iguais a uma data
void removerEventosAnterioresFila(Fila f, int ano, int mes, int dia) {
    if (f == NULL || f->inicio == NULL) return;

    link atual = f->inicio;
    link anterior = NULL;

    while (atual != NULL) {
        DataHora dh = getDataHora(atual->item);

        // A  data é verificada
        if (getAno(dh) < ano || (getAno(dh) == ano && getMes(dh) < mes) || (getAno(dh) == ano && getMes(dh) == mes && getDia(dh) <= dia)) {

            link remover = atual;

            // Ajusta os ponteiros da fila
            if (anterior == NULL) {
                f->inicio = atual->next;
            }
            else {
                anterior->next = atual->next;
            }

            // Se for o último Node
            if (f->fim == remover) {
                f->fim = anterior;
            }

            f->count--;

            // Avança para o próximo
            atual = atual->next;
            
            apagarEvento(remover->item);
            free(remover);
        }
        else {
            break; // Para o ciclo se a fila já estiver ordenada
        }
    }
}



// Os ponteiros dos eventos são copiados para um array (utilizado para as listagens globais)
void extrairEventosParaArray(Fila f, Evento* array_destino, int* indice_atual) {
    if (f == NULL || isEmpty(f)) return;

    link atual = f->inicio;

    while (atual != NULL) {
        // Ponteiro é guardado no Array
        array_destino[*indice_atual] = atual->item;

        // Incrementa para não sobrepor eventos
        (*indice_atual)++;

        atual = atual->next;
    }
}