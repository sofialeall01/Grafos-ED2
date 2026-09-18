#include <stdio.h>
#include <stdlib.h>
#include "caminho.h"

typedef struct {
    float distancia; // Custo acumulado para chegar ao vértice
    int anterior;    // Vértice que antecede este no menor caminho
    int visitado;    // Flag booleana (0 ou 1)
} InfoCaminho; //estrutura para armazenar informações do caminho mínimo

