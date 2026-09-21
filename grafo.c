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
    int numArestas;
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
    g->numArestas = 0;
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

int GAcriaAresta(Grafo p, int alfa, int omega) {

    if (p->numArestas >= p->maxArestas) {
        return 0;
    }

    int numVertices = p->vertice[0].primeiraSaida;

    /*Verifica se o alfa e o omega existem */
    if (alfa <= 0 || alfa > numVertices ||
        omega <= 0 || omega > numVertices) {
        return 0;
    }

    p->numArestas++;
    int idAresta = p->numArestas;

    /*Guarda a origem e o destino*/
    p->aresta[idAresta].alfa = alfa;
    p->aresta[idAresta].omega = omega;

    /*Add a aresta na saída do alfa*/
    p->aresta[idAresta].proxSaida = p->vertice[alfa].primeiraSaida;
    p->vertice[alfa].primeiraSaida = idAresta;
    
    /*Add a aresta na saída do omega*/
    p->aresta[idAresta].proxEntrada = p->vertice[omega].primeiraEntrada;
    p->vertice[omega].primeiraEntrada = idAresta;

    return idAresta;
}

int GBexisteIdVertice(Grafo p, int v){
    //armazena a quantidade de vertices
    int numVertices = p->vertice[0].primeiraSaida;

    //verifica se o identificador do vertice é válido
    if (v <= 0 || v > numVertices) {
        return 0;
    }
    
    return 1;
}

int GBexisteIdAresta(Grafo p, int a){
    //verifica se o identificador da aresta é válido
    if (a <= 0 || a > p->numArestas) {
        return 0;
    }
    
    return 1;
}

int GBexisteArestaDir(Grafo p, int v1, int v2){
    // Verifica se os identificadores dos vértices são válidos.
    if (v1 <= 0 || v2 <= 0 ||
        v1 > p->vertice[0].primeiraSaida ||
        v2 > p->vertice[0].primeiraSaida) {
        return 0;
    }

    int aresta = p->vertice[v1].primeiraSaida;

    while (aresta != 0) {

        // Verifica se a aresta chega em v2.
        if (p->aresta[aresta].omega == v2) {
            return 1;
        }

        // Vai para a próxima aresta de saída.
        aresta = p->aresta[aresta].proxSaida;
    }

    return 0;
}

int GApegaArestaDir(Grafo p, int v1, int v2){
    if (p == NULL) {
        return 0;
    }

    /* Validar se os vértices existem no grafo */
    if (!GBexisteIdVertice(p, v1) || !GBexisteIdVertice(p, v2)) {
        return 0;
    }

    /* Percorre a Saída de v1 */
    int aresta = p->vertice[v1].primeiraSaida;

    while (aresta != 0) {
        /* Se a aresta chega até v2 (omega == v2), encontramos a aresta dirigida(retorna id) */
        if (p->aresta[aresta].omega == v2) {
            return aresta; 
        }
        /* Avança para a próxima aresta */
        aresta = p->aresta[aresta].proxSaida;
    }

    return 0; /* Aresta não encontrada */
}

int GBexisteAresta(Grafo p, int v1, int v2) {
    if (p == NULL) {
        return 0;
    }

    /* Verifica se os vértices existem */
    if (!GBexisteIdVertice(p, v1) || !GBexisteIdVertice(p, v2)) {
        return 0;
    }

    /* Verifica v1 -> v2 */
    if (GBexisteArestaDir(p, v1, v2)) {
        return 1;
    }

    /* Verifica v2 -> v1 */
    if (GBexisteArestaDir(p, v2, v1)) {
        return 1;
    }

    return 0;
}

int GApegaAresta(Grafo p, int v1, int v2) {
    if (p == NULL) {
        return 0;
    }

    /* Verifica se os vértices existem */
    if (!GBexisteIdVertice(p, v1) || !GBexisteIdVertice(p, v2)) {
        return 0;
    }

    /* Procura v1 -> v2 */
    int aresta = GApegaArestaDir(p, v1, v2);

    if (aresta != 0) {
        return aresta;
    }

    /* Procura v2 -> v1 */
    aresta = GApegaArestaDir(p, v2, v1);

    if (aresta != 0) {
        return aresta;
    }

    return 0;
}

