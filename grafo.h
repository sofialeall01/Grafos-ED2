#ifndef GRAFO_H
#define GRAFO_H

/* 
 * ============================================================================
 * DECLARAÇÃO OPACA DO TAD GRAFO
 * ============================================================================
 * O ponteiro 'Grafo' oculta os detalhes de implementação das structs internas,
 * garantindo o encapsulamento do Tipo Abstrato de Dados.
 */
struct grafo;
typedef struct grafo *Grafo;

/* ============================================================================
 * 2.2.1 Operações relacionadas ao grafo
 * ============================================================================ */

/* Cria e inicializa um grafo estático com capacidade máxima de 'v' vértices e 'a' arestas */
Grafo GGcriaGrafo(int v, int a);

/* Destrói o grafo liberando toda a memória alocada dinamicamente */
Grafo GGdestroiGrafo(Grafo p);

/* Cria um novo vértice no grafo e retorna o seu identificador (ID) */
int GVcriaVertice(Grafo p);

// /* Cria uma aresta ligando alfa a omega e retorna o seu identificador (ID) */
int GAcriaAresta(Grafo p, int alfa, int omega);

// /* Verifica se existe um vértice com o ID informado no grafo */
int GBexisteIdVertice(Grafo p, int v);

// /* Verifica se existe uma aresta com o ID informado no grafo */
int GBexisteIdAresta(Grafo p, int a);

// /* Verifica se existe uma aresta dirigida partindo de v1 e chegando em v2 */
int GBexisteArestaDir(Grafo p, int v1, int v2);

// /* Verifica se existe uma aresta não dirigida entre v1 e v2 */
int GBexisteAresta(Grafo p, int v1, int v2);

// /* Retorna o ID da aresta dirigida que parte de v1 e chega em v2 (ou 0 se não existir) */
int GApegaArestaDir(Grafo p, int v1, int v2);

// /* Retorna o ID da aresta não dirigida entre v1 e v2 (ou 0 se não existir) */
int GApegaAresta(Grafo p, int v1, int v2);

// /* Retorna o ID do primeiro vértice do grafo (menor identificador) */
 int GVprimeiroVertice(Grafo p);

/* Retorna o ID do próximo vértice no grafo maior que v1 (ou 0 se não houver) */
int GVproximoVertice(Grafo p, int v1);

/* Retorna o ID da primeira aresta do grafo (menor identificador) */
int GAprimeiraAresta(Grafo p);

/* Retorna o ID da próxima aresta no grafo maior que a1 (ou 0 se não houver) */
int GAproximaAresta(Grafo p, int a1);

/* Retorna o número atual de vértices criados no grafo */
int GInumeroVertices(Grafo p);

/* Retorna a capacidade máxima prevista de vértices do grafo */
int GInumeroVerticesMax(Grafo p);

/* Retorna o número atual de arestas criadas no grafo */
int GInumeroArestas(Grafo p);

/* Retorna a capacidade máxima prevista de arestas do grafo */
int GInumeroArestasMax(Grafo p);

/* Carrega a estrutura de um grafo armazenado em um arquivo de texto */
Grafo GGcarregaGrafo(char *f);

/* Salva a estrutura do grafo atual em um arquivo de texto */
int GBsalvaGrafo(Grafo p, char *f);

// /* ============================================================================
//  * 2.2.2 Operações relacionadas aos vértices do grafo
//  * ============================================================================ */

/* Retorna o grau do vértice v (somatório de entradas e saídas) */
int GIpegaGrau(Grafo p, int v);

/* Retorna a primeira aresta na estrela do vértice v (para grafos não dirigidos) */
int GAprimaAresta(Grafo p, int v);

/* Retorna a próxima aresta na estrela do vértice v após a1 (não dirigido) */
int GAproxAresta(Grafo p, int v, int a1);

/* Retorna a primeira aresta na estrela de entrada do vértice v (dirigidos) */
int GAprimaEntrada(Grafo p, int v);

/* Retorna a próxima aresta na estrela de entrada do vértice v após a1 (dirigidos) */
int GAproxEntrada(Grafo p, int v, int a1);

/* Retorna a primeira aresta na estrela de saída do vértice v (dirigidos) */
int GAprimaSaida(Grafo p, int v);

/* Retorna a próxima aresta na estrela de saída do vértice v após a1 (dirigidos) */
int GAproxSaida(Grafo p, int v, int a1);

/* Remove uma aresta específica da lista de saída do vértice 'alfa' */
void removeDaEstrelaSaida(Grafo p, int alfa, int idAresta);

/* Remove uma aresta específica da lista de entrada do vértice 'omega' */
void removeDaEstrelaEntrada(Grafo p, int omega, int idAresta);

 /* Remove um vértice do grafo, liberando suas arestas e atualizando as listas de adjacência */
int GAremoveVertice(Grafo p, int v) ;

/* Remove uma aresta do grafo, atualizando as listas de adjacência dos vértices envolvidos */
int GAremoveAresta(Grafo p, int idAresta);

// /* ============================================================================
//  * 2.2.3 Operações relacionadas às arestas do grafo
//  * ============================================================================ */

// /* Verifica se a aresta 'a' é um laço (liga um vértice a ele mesmo) */
int GBarestaLaco(Grafo p, int a);

// /* Retorna o vértice de partida (origem/alfa) da aresta 'a' */
int GValfa(Grafo p, int a);

// /* Retorna o vértice de chegada (destino/omega) da aresta 'a' */
int GVomega(Grafo p, int a);

// /* Retorna o vértice oposto a v1 conectado pela aresta 'a' */
int GVvizinho(Grafo p, int a, int v1);

#endif /* GRAFO_H */