#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "caminho.h"
#include "grafo.h"

typedef struct {
    float distancia; // Custo acumulado para chegar ao vértice
    int anterior;    // Vértice que antecede este no menor caminho
    int visitado;    // Flag booleana (0 ou 1)
} InfoCaminho; //estrutura para armazenar informações do caminho mínimo


void Gcaminho(Grafo g, float *pesos, int a, int b) {
    if (g == NULL || g->vertice == NULL || g->aresta == NULL) {
        return;
    }

    int numVertices = g->vertice[0].primeiraSaida; // Quantidade atual de vértices no grafo

    if (a <= 0 || a > numVertices || b <= 0 || b > numVertices) {
        printf("Vertices invalidos.\n");
        return;
    }

    /* Aloca a estrutura de informações auxiliares para Dijkstra (1-indexed) */
    InfoCaminho *info = (InfoCaminho *) malloc((numVertices + 1) * sizeof(InfoCaminho));
    if (info == NULL) {
        return;
    }

    /* 1. Inicialização */
    for (int i = 1; i <= numVertices; i++) {
        info[i].distancia = FLT_MAX;
        info[i].anterior = 0;
        info[i].visitado = 0;
    }
    info[a].distancia = 0.0f;

    /* 2. Execução do Algoritmo de Dijkstra */
    for (int count = 1; count <= numVertices; count++) {
        int u = -1;
        float menorDist = FLT_MAX;

        /* Encontra o vértice não visitado de menor distância acumulada */
        for (int v = 1; v <= numVertices; v++) {
            if (!info[v].visitado && info[v].distancia < menorDist) {
                menorDist = info[v].distancia;
                u = v;
            }
        }

        /* Se não encontrou vértice alcançável ou chegou no destino b, encerra a busca */
        if (u == -1 || u == b) {
            break;
        }

        info[u].visitado = 1;

        /* Percorre a Estrela de Saída de u */
        int e = g->vertice[u].primeiraSaida;
        while (e > 0) {
            int v = g->aresta[e].omega;
            float pesoAresta = pesos[e]; // Acessa o peso diretamente pelo ID da aresta e

            /* Relaxamento da aresta */
            if (!info[v].visitado && info[u].distancia != FLT_MAX) {
                if (info[u].distancia + pesoAresta < info[v].distancia) {
                    info[v].distancia = info[u].distancia + pesoAresta;
                    info[v].anterior = u;
                }
            }

            e = g->aresta[e].proxSaida;
        }
    }

    /* 3. Reconstrução e Impressão do Caminho */
    /* Se o destino b não tiver anterior e for diferente de a, não há caminho */
    if (info[b].distancia == FLT_MAX || (b != a && info[b].anterior == 0)) {
        printf("%d\n", a);
    } else {
        /* Reconstrói o caminho do fim para o início usando um vetor temporário */
        int *caminho = (int *) malloc((numVertices + 1) * sizeof(int));
        int tam = 0;
        int atual = b;

        while (atual != 0) {
            caminho[tam++] = atual;
            atual = info[atual].anterior;
        }

        /* Imprime o caminho da origem até o destino no formato "a -> ... -> b" */
        for (int i = tam - 1; i >= 0; i--) {
            if (i == tam - 1) {
                printf("%d", caminho[i]);
            } else {
                printf(" -> %d", caminho[i]);
            }
        }
        printf("\n");

        free(caminho);
    }

    free(info);
}