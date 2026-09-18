#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"


typedef struct {
    int alfa;        /* Identificador do vértice de partida (origem) */
    int omega;       /* Identificador do vértice de chegada (destino) */
    int proxSaida;   /* Próxima aresta pertencente à Estrela de Saída de 'alfa' */
    int proxEntrada; /* Próxima aresta pertencente à Estrela de Entrada de 'omega' */
} Aresta;


typedef struct {
    int primeiraSaida;   /* ID da primeira aresta na Estrela de Saída deste vértice */
    int primeiraEntrada; /* ID da primeira aresta na Estrela de Entrada deste vértice */
} Vertice;


struct grafo {
    int maxVertices;   /* Capacidade maxima prevista para vertices */
    int maxArestas;    /* Capacidade maxima prevista para arestas */

    Vertice *vertice;  /* Vetor alocado dinamicamente para os Vertices */
    Aresta *aresta;    /* Vetor alocado dinamicamente para as Arestas */
};



/*
 * Operação :: GGcriaGrafo
 * Descrição: Cria e inicializa um grafo estático.
 * Entradas  : v (máximo de vértices), a (máximo de arestas)
 * Pré-requisitos: v > 0 e a > 0
 */
Grafo GGcriaGrafo(int v, int a) {
    /* Validacao dos pre-requisitos especificados */
    if (v <= 0 || a <= 0) {
        return NULL;
    }

    /* 1. Aloca a estrutura principal do Grafo */
    Grafo g = (Grafo) malloc(sizeof(struct grafo));
    if (g == NULL) {
        return NULL; /* Falha de alocacao */
    }

    /* 2. Configura as capacidades maximas */
    g->maxVertices = v;
    g->maxArestas = a;

    /* 3. Aloca o vetor de vertices (tamanho v + 1 para usar indices 1..v) */
    g->vertice = (Vertice *) malloc((v + 1) * sizeof(Vertice));
    if (g->vertice == NULL) {
        free(g);
        return NULL;
    }

    /* 4. Aloca o vetor de arestas (tamanho a + 1 para usar indices 1..a) */
    g->aresta = (Aresta *) malloc((a + 1) * sizeof(Aresta));
    if (g->aresta == NULL) {
        free(g->vertice);
        free(g);
        return NULL;
    }

    /* 5. Inicializa os controles na posicao 0 de cada vetor */
    /* vertice[0].primeiraSaida armazena a quantidade de vertices criados (inicia em 0) */
    g->vertice[0].primeiraSaida = 0;
    g->vertice[0].primeiraEntrada = 0;

    /* aresta[0].alfa armazena a quantidade de arestas criadas (inicia em 0) */
    g->aresta[0].alfa = 0;
    g->aresta[0].omega = 0;
    g->aresta[0].proxSaida = 0;
    g->aresta[0].proxEntrada = 0;

    /* 6. Inicializa as posicoes de 1 ate 'v' como NAO CRIADAS (-1) */
    for (int i = 1; i <= v; i++) {
        g->vertice[i].primeiraSaida = -1;  /* -1 indica vertice NAO CRIADO */
        g->vertice[i].primeiraEntrada = -1;
    }

    /* 7. Inicializa as posicoes de 1 ate 'a' para evitar lixo de memoria */
    for (int i = 1; i <= a; i++) {
        g->aresta[i].alfa = 0;
        g->aresta[i].omega = 0;
        g->aresta[i].proxSaida = 0;
        g->aresta[i].proxEntrada = 0;
    }

    return g;
}

/*
 * Operação :: GGdestroiGrafo
 * Descrição: Libera toda a memória alocada para o grafo e retorna NULL.
 * Entrada  : g (ponteiro para o grafo)
 * Retorno  : NULL
 */
Grafo GGdestroiGrafo(Grafo g) {
    if (g != NULL) {
        if (g->vertice != NULL) {
            free(g->vertice);
            g->vertice = NULL;
        }

        if (g->aresta != NULL) {
            free(g->aresta);
            g->aresta = NULL;
        }

        free(g);
    }

    return NULL;
}


int GVcriaVertice(Grafo p) {
    if (p == NULL || p->vertice == NULL) {
        return 0;
    }

    /* Lê a quantidade atual de vértices armazenada na posição de controle 0 */
    int numVerticesCriados = p->vertice[0].primeiraSaida;

    /* PRÉ-REQUISITO: |V| < maxVertices */
    if (numVerticesCriados >= p->maxVertices) {
        return 0; /* Limite máximo atingido */
    }

    /* Incrementa a quantidade total */
    numVerticesCriados++;
    
    /* Atualiza o contador na posição 0 */
    p->vertice[0].primeiraSaida = numVerticesCriados;

    int v = numVerticesCriados;

    /* PÓS-REQUISITO: O vértice 'v' passa a existir sem arestas vinculadas (0) */
    p->vertice[v].primeiraSaida = 0;
    p->vertice[v].primeiraEntrada = 0;

    return v;
}