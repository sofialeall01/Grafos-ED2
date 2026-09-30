#ifndef CAMINHO_H
#define CAMINHO_H
#include "grafo.h"

struct caminho;
typedef struct caminho *Caminho;

void Gcaminho(Grafo g, float *pesos, int a, int b);

#endif /* CAMINHO_H */