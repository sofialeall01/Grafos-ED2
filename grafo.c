#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "grafo.h"



/*
 * Operação :: GGcriaGrafo
 * Descrição: Cria e inicializa um grafo estático.
 * Entradas  : v (máximo de vértices), a (máximo de arestas)
 * Pré-requisitos: v > 0 e a > 0
 */
Grafo GGcriaGrafo(int v, int a)
{
    /* Validacao dos pre-requisitos especificados */
    if (v <= 0 || a <= 0)
    {
        return NULL;
    }

    /* 1. Aloca a estrutura principal do Grafo */
    Grafo g = (Grafo)malloc(sizeof(struct grafo));
    if (g == NULL)
    {
        return NULL; /* Falha de alocacao */
    }

    /* 2. Configura as capacidades maximas */
    g->maxVertices = v;
    g->maxArestas = a;

    /* 3. Aloca o vetor de vertices (tamanho v + 1 para usar indices 1..v) */
    g->vertice = (Vertice *)malloc((v + 1) * sizeof(Vertice));
    if (g->vertice == NULL)
    {
        free(g);
        return NULL;
    }

    /* 4. Aloca o vetor de arestas (tamanho a + 1 para usar indices 1..a) */
    g->aresta = (Aresta *)malloc((a + 1) * sizeof(Aresta));
    if (g->aresta == NULL)
    {
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
    for (int i = 1; i <= v; i++)
    {
        g->vertice[i].primeiraSaida = -1; /* -1 indica vertice NAO CRIADO */
        g->vertice[i].primeiraEntrada = -1;
    }

    /* 7. Inicializa as posicoes de 1 ate 'a' para evitar lixo de memoria */
    for (int i = 1; i <= a; i++)
    {
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
Grafo GGdestroiGrafo(Grafo g)
{
    if (g != NULL)
    {
        if (g->vertice != NULL)
        {
            free(g->vertice);
            g->vertice = NULL;
        }

        if (g->aresta != NULL)
        {
            free(g->aresta);
            g->aresta = NULL;
        }

        free(g);
    }

    return NULL;
}

int GVcriaVertice(Grafo p)
{
    if (p == NULL || p->vertice == NULL)
    {
        return 0;
    }

    /* Lê a quantidade atual de vértices armazenada na posição de controle 0 */
    int numVerticesCriados = p->vertice[0].primeiraSaida;

    /* PRÉ-REQUISITO: |V| < maxVertices */
    if (numVerticesCriados >= p->maxVertices)
    {
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

int GAcriaAresta(Grafo p, int alfa, int omega)
{

    if (p->numArestas >= p->maxArestas)
    {
        return 0;
    }

    int numVertices = p->vertice[0].primeiraSaida;

    /* Verifica se o alfa e o omega existem */
    if (alfa <= 0 || alfa > numVertices ||
        omega <= 0 || omega > numVertices)
    {
        return 0;
    }

    /* -------------------------------------------------------------
     * VALIDAÇÃO DE ARESTAS DUPLICADAS:
     * Percorre a Estrela de Saída de 'alfa' para verificar se
     * já existe uma aresta que chegue em 'omega'.
     * ------------------------------------------------------------- */
    int a = p->vertice[alfa].primeiraSaida;
    while (a > 0)
    {
        if (p->aresta[a].omega == omega)
        {
            /* Aresta duplicada encontrada! Interrompe a criação. */
            return 0;
        }
        a = p->aresta[a].proxSaida;
    }

    /* Criação da nova aresta caso não haja duplicata */
    p->numArestas++;
    int idAresta = p->numArestas;

    /* Guarda a origem e o destino */
    p->aresta[idAresta].alfa = alfa;
    p->aresta[idAresta].omega = omega;

    /* Add a aresta na Estrela de Saída de alfa */
    p->aresta[idAresta].proxSaida = p->vertice[alfa].primeiraSaida;
    p->vertice[alfa].primeiraSaida = idAresta;

    /*Add a aresta na saída do omega*/
    p->aresta[idAresta].proxEntrada = p->vertice[omega].primeiraEntrada;
    p->vertice[omega].primeiraEntrada = idAresta;

    return idAresta;
}

int GBexisteIdVertice(Grafo p, int v)
{
    // armazena a quantidade de vertices
    int numVertices = p->vertice[0].primeiraSaida;

    // verifica se o identificador do vertice é válido
    if (v <= 0 || v > numVertices)
    {
        return 0;
    }

    return 1;
}

int GBexisteIdAresta(Grafo p, int a)
{
    // verifica se o identificador da aresta é válido
    if (a <= 0 || a > p->numArestas)
    {
        return 0;
    }

    return 1;
}

int GBexisteArestaDir(Grafo p, int v1, int v2)
{
    // Verifica se os identificadores dos vértices são válidos.
    if (v1 <= 0 || v2 <= 0 ||
        v1 > p->vertice[0].primeiraSaida ||
        v2 > p->vertice[0].primeiraSaida)
    {
        return 0;
    }

    int aresta = p->vertice[v1].primeiraSaida;

    while (aresta != 0)
    {

        // Verifica se a aresta chega em v2.
        if (p->aresta[aresta].omega == v2)
        {
            return 1;
        }

        // Vai para a próxima aresta de saída.
        aresta = p->aresta[aresta].proxSaida;
    }

    return 0;
}

int GApegaArestaDir(Grafo p, int v1, int v2)
{
    if (p == NULL)
    {
        return 0;
    }

    /* Validar se os vértices existem no grafo */
    if (!GBexisteIdVertice(p, v1) || !GBexisteIdVertice(p, v2))
    {
        return 0;
    }

    /* Percorre a Saída de v1 */
    int aresta = p->vertice[v1].primeiraSaida;

    while (aresta != 0)
    {
        /* Se a aresta chega até v2 (omega == v2), encontramos a aresta dirigida(retorna id) */
        if (p->aresta[aresta].omega == v2)
        {
            return aresta;
        }
        /* Avança para a próxima aresta */
        aresta = p->aresta[aresta].proxSaida;
    }

    return 0; /* Aresta não encontrada */
}

int GBexisteAresta(Grafo p, int v1, int v2)
{
    if (p == NULL)
    {
        return 0;
    }

    /* Verifica se os vértices existem */
    if (!GBexisteIdVertice(p, v1) || !GBexisteIdVertice(p, v2))
    {
        return 0;
    }

    /* Verifica v1 -> v2 */
    if (GBexisteArestaDir(p, v1, v2))
    {
        return 1;
    }

    /* Verifica v2 -> v1 */
    if (GBexisteArestaDir(p, v2, v1))
    {
        return 1;
    }

    return 0;
}

int GApegaAresta(Grafo p, int v1, int v2)
{
    if (p == NULL)
    {
        return 0;
    }

    /* Verifica se os vértices existem */
    if (!GBexisteIdVertice(p, v1) || !GBexisteIdVertice(p, v2))
    {
        return 0;
    }

    /* Procura v1 -> v2 */
    int aresta = GApegaArestaDir(p, v1, v2);

    if (aresta != 0)
    {
        return aresta;
    }

    /* Procura v2 -> v1 */
    aresta = GApegaArestaDir(p, v2, v1);

    if (aresta != 0)
    {
        return aresta;
    }

    return 0;
}

int GVprimeiroVertice(Grafo p)
{
    if (p == NULL)
    {
        return 0;
    }

    for (int v = 1; v <= p->maxVertices; v++)
    {
        if (p->vertice[v].primeiraSaida != -1)
        {
            return v;
        }
    }

    return 0;
}

int GVproximoVertice(Grafo p, int v1)
{
    /* Valida se a estrutura do grafo existe */
    if (p == NULL || p->vertice == NULL)
    {
        return 0;
    }

    /* Ajusta v1 para 0 caso seja passado um valor negativo */
    if (v1 < 0)
    {
        v1 = 0;
    }

    /* Procura o menor ID de vértice que seja maior que v1 */
    for (int i = v1 + 1; i <= p->maxVertices; i++)
    {
        /* Na sua estrutura, um vértice existe se primeiraSaida for diferente de -1 */
        if (p->vertice[i].primeiraSaida != -1)
        {
            return i; /* Retorna o proximo vertice existente (v2) */
        }
    }

    /* Retorna 0 caso nao exista nenhum vertice com ID maior que v1 */
    return 0;
}

int GAproximaAresta(Grafo p, int a1)
{
    /* 1. Valida se a estrutura do grafo e o vetor de arestas existem */
    if (p == NULL || p->aresta == NULL)
    {
        return 0;
    }

    /* 2. Ajusta a1 caso seja passado um valor negativo */
    if (a1 < 0)
    {
        a1 = 0;
    }

    /* 3. Procura o menor ID de aresta que seja maior que a1 */
    for (int i = a1 + 1; i <= p->maxArestas; i++)
    {
        /* Uma aresta valida possui alfa e omega definidos (diferentes de 0) */
        if (p->aresta[i].alfa != 0 && p->aresta[i].omega != 0)
        {
            return i; /* Retorna o ID da proxima aresta existente (a2) */
        }
    }

    /* 4. Retorna 0 caso nao exista nenhuma aresta com ID maior que a1 */
    return 0;
}

int GAprimeiraAresta(Grafo p)
{
    /* Retorna o menor ID de aresta existente (maior que 0) */
    return GAproximaAresta(p, 0);
}

int GInumeroVertices(Grafo p)
{
    if (p == NULL || p->vertice == NULL)
    {
        return 0;
    }

    int nv = 0;
    for (int i = 1; i <= p->maxVertices; i++)
    {
        if (p->vertice[i].primeiraSaida != -1)
        {
            nv++;
        }
    }

    return nv;
}

int GInumeroVerticesMax(Grafo p)
{
    /* 1. Valida se a estrutura do grafo existe */
    if (p == NULL)
    {
        return 0;
    }

    /* 2. Retorna a capacidade maxima de vertices previstos */
    return p->maxVertices;
}

int GInumeroArestas(Grafo p)
{
    /* 1. Valida se a estrutura do grafo existe */
    if (p == NULL)
    {
        return 0;
    }

    /* 2. Retorna a quantidade atual de arestas |A| */
    return p->numArestas;
}

int GInumeroArestasMax(Grafo p)
{
    /* 1. Valida se a estrutura do grafo existe */
    if (p == NULL)
    {
        return 0;
    }

    /* 2. Retorna a capacidade maxima de arestas previstos */
    return p->maxArestas;
}

Grafo GGcarregaGrafo(char *f)
{
    if (f == NULL)
        return NULL;

    FILE *arquivo = fopen(f, "r");
    if (arquivo == NULL)
    {
        printf(">> Erro: Nao foi possivel abrir o arquivo '%s'.\n", f);
        return NULL;
    }

    int maxV, maxA;
    if (fscanf(arquivo, "%d %d", &maxV, &maxA) != 2)
    {
        fclose(arquivo);
        return NULL;
    }

    Grafo p = GGcriaGrafo(maxV, maxA);
    if (p == NULL)
    {
        fclose(arquivo);
        return NULL;
    }

    int alfa, omega;
    while (fscanf(arquivo, "%d %d", &alfa, &omega) == 2)
    {
        /* Cria vértices dinamicamente caso o ID lido seja maior que a quantidade atual */
        while (GInumeroVertices(p) < alfa)
        {
            GVcriaVertice(p);
        }
        while (GInumeroVertices(p) < omega)
        {
            GVcriaVertice(p);
        }

        /* Cria a aresta */
        GAcriaAresta(p, alfa, omega);
    }

    fclose(arquivo);
    return p;
}

int GBsalvaGrafo(Grafo p, char *f)
{
    /* 1. Validações prévias */
    if (p == NULL || f == NULL)
    {
        return 0;
    }

    /* 2. Tenta abrir o arquivo para escrita */
    FILE *arquivo = fopen(f, "w");
    if (arquivo == NULL)
    {
        printf(">> Erro: Nao foi possivel criar ou abrir o arquivo '%s' para escrita.\n", f);
        return 0;
    }

    /* 3. Escreve o cabeçalho: maxVertices e maxArestas */
    fprintf(arquivo, "%d %d\n", p->maxVertices, p->maxArestas);

    /* 4. Percorre o vetor de arestas e salva apenas as ativas */
    for (int i = 1; i <= p->maxArestas; i++)
    {
        if (p->aresta[i].alfa != 0 && p->aresta[i].omega != 0)
        {
            fprintf(arquivo, "%d %d\n", p->aresta[i].alfa, p->aresta[i].omega);
        }
    }

    /* 5. Fecha o arquivo e retorna sucesso */
    fclose(arquivo);
    return 1;
}

int GIpegaGrau(Grafo p, int v)
{
    /* 1. Valida se o grafo e o vetor de vertices existem */
    if (p == NULL || p->vertice == NULL)
    {
        return 0;
    }

    /* 2. Valida se o identificador do vertice e valido */
    if (v <= 0 || v > p->maxVertices)
    {
        return 0;
    }

    /* 3. Verifica se o vertice existe/esta ativo no grafo */
    /* Assumindo que um vertice inativo possui primeiraSaida e primeiraEntrada como -1 ou sem registro */
    if (p->vertice[v].primeiraSaida == -1 && p->vertice[v].primeiraEntrada == -1)
    {
        return 0;
    }

    int grau = 0;

    /* 4. Conta as arestas na Estrela de Saida (Arestas saindo de v) */
    int a = p->vertice[v].primeiraSaida;
    while (a > 0)
    {
        grau++;
        a = p->aresta[a].proxSaida;
    }

    /* 5. Conta as arestas na Estrela de Entrada (Arestas chegando em v) */
    a = p->vertice[v].primeiraEntrada;
    while (a > 0)
    {
        grau++;
        a = p->aresta[a].proxEntrada;
    }

    return grau;
}

int GAprimaAresta(Grafo p, int v)
{
    if (p == NULL || p->vertice == NULL || v <= 0 || v > p->maxVertices)
    {
        return 0;
    }

    int menorAresta = 0;

    /* 1. Percorre toda a estrela de SAÍDA procurando o menor ID de aresta */
    int a = p->vertice[v].primeiraSaida;
    while (a > 0)
    {
        if (menorAresta == 0 || a < menorAresta)
        {
            menorAresta = a;
        }
        a = p->aresta[a].proxSaida;
    }

    /* 2. Percorre toda a estrela de ENTRADA procurando se há algum ID menor ainda */
    a = p->vertice[v].primeiraEntrada;
    while (a > 0)
    {
        if (menorAresta == 0 || a < menorAresta)
        {
            menorAresta = a;
        }
        a = p->aresta[a].proxEntrada;
    }

    /* Retorna o menor ID encontrado (ou 0 se não houver arestas) */
    return menorAresta;
}

int GAproxAresta(Grafo p, int v, int a1)
{
    /* 1. Validações preliminares */
    if (p == NULL || p->vertice == NULL || p->aresta == NULL)
    {
        return 0;
    }

    if (v <= 0 || v > p->maxVertices)
    {
        return 0;
    }

    int menorProxima = 0;

    /* 2. Percorre a Estrela de Saída de v */
    int a = p->vertice[v].primeiraSaida;
    while (a > 0)
    {
        if (a > a1)
        {
            if (menorProxima == 0 || a < menorProxima)
            {
                menorProxima = a;
            }
        }
        a = p->aresta[a].proxSaida;
    }

    /* 3. Percorre a Estrela de Entrada de v */
    a = p->vertice[v].primeiraEntrada;
    while (a > 0)
    {
        if (a > a1)
        {
            if (menorProxima == 0 || a < menorProxima)
            {
                menorProxima = a;
            }
        }
        a = p->aresta[a].proxEntrada;
    }

    /* Retorna o menor ID encontrado que e maior que a1 (ou 0 se nao existir) */
    return menorProxima;
}

int GAprimaEntrada(Grafo p, int v)
{
    /* 1. Validações preliminares */
    if (p == NULL || p->vertice == NULL || p->aresta == NULL)
    {
        return 0;
    }

    if (v <= 0 || v > p->maxVertices)
    {
        return 0;
    }

    int menorAresta = 0;

    /* 2. Percorre apenas a Estrela de Entrada de v (EE(v)) */
    int a = p->vertice[v].primeiraEntrada;
    while (a > 0)
    {
        if (menorAresta == 0 || a < menorAresta)
        {
            menorAresta = a;
        }
        a = p->aresta[a].proxEntrada;
    }

    /* Retorna o menor ID encontrado (ou 0 se EE(v) for vazia) */
    return menorAresta;
}

int GAproxEntrada(Grafo p, int v, int a1)
{
    /* 1. Validações preliminares */
    if (p == NULL || p->vertice == NULL || p->aresta == NULL)
    {
        return 0;
    }

    if (v <= 0 || v > p->maxVertices)
    {
        return 0;
    }

    int menorProxima = 0;

    /* 2. Percorre apenas a Estrela de Entrada de v (EE(v)) */
    int a = p->vertice[v].primeiraEntrada;
    while (a > 0)
    {
        if (a > a1)
        {
            if (menorProxima == 0 || a < menorProxima)
            {
                menorProxima = a;
            }
        }
        a = p->aresta[a].proxEntrada;
    }

    /* Retorna o menor ID encontrado maior que a1 (ou 0 se nao existir) */
    return menorProxima;
}

int GAprimaSaida(Grafo p, int v)
{
    /* 1. Validações preliminares */
    if (p == NULL || p->vertice == NULL || p->aresta == NULL)
    {
        return 0;
    }

    if (v <= 0 || v > p->maxVertices)
    {
        return 0;
    }

    int menorAresta = 0;

    /* 2. Percorre apenas a Estrela de Saída de v (ES(v)) */
    int a = p->vertice[v].primeiraSaida;
    while (a > 0)
    {
        if (menorAresta == 0 || a < menorAresta)
        {
            menorAresta = a;
        }
        a = p->aresta[a].proxSaida;
    }

    /* Retorna o menor ID encontrado (ou 0 se ES(v) estiver vazia) */
    return menorAresta;
}

int GAproxSaida(Grafo p, int v, int a1)
{
    /* 1. Validações preliminares de segurança */
    if (p == NULL || p->vertice == NULL || p->aresta == NULL)
    {
        return 0;
    }

    if (v <= 0 || v > p->maxVertices)
    {
        return 0;
    }

    int menorProxima = 0;

    /* 2. Percorre a Estrela de Saída de v (ES(v)) */
    int a = p->vertice[v].primeiraSaida;
    while (a > 0)
    {
        /* Filtra apenas arestas com ID estritamente maior que a1 */
        if (a > a1)
        {
            /* Busca o menor ID entre os válidos (min(a2) > a1) */
            if (menorProxima == 0 || a < menorProxima)
            {
                menorProxima = a;
            }
        }
        a = p->aresta[a].proxSaida;
    }

    /* Retorna o menor ID encontrado que atende à condição (ou 0 se não existir) */
    return menorProxima;
}

/* Remove uma aresta específica da lista de saída do vértice 'alfa' */
void removeDaEstrelaSaida(Grafo p, int alfa, int idAresta)
{
    int a = p->vertice[alfa].primeiraSaida;
    int ant = 0;

    while (a > 0)
    {
        if (a == idAresta)
        {
            if (ant == 0)
            {
                p->vertice[alfa].primeiraSaida = p->aresta[a].proxSaida;
            }
            else
            {
                p->aresta[ant].proxSaida = p->aresta[a].proxSaida;
            }
            break;
        }
        ant = a;
        a = p->aresta[a].proxSaida;
    }
}

/* Remove uma aresta específica da lista de entrada do vértice 'omega' */
void removeDaEstrelaEntrada(Grafo p, int omega, int idAresta)
{
    int a = p->vertice[omega].primeiraEntrada;
    int ant = 0;

    while (a > 0)
    {
        if (a == idAresta)
        {
            if (ant == 0)
            {
                p->vertice[omega].primeiraEntrada = p->aresta[a].proxEntrada;
            }
            else
            {
                p->aresta[ant].proxEntrada = p->aresta[a].proxEntrada;
            }
            break;
        }
        ant = a;
        a = p->aresta[a].proxEntrada;
    }
}

int GAremoveVertice(Grafo p, int v)
{
    /* 1. Validações preliminares */
    if (p == NULL || p->vertice == NULL || p->aresta == NULL)
    {
        return 0;
    }

    int numVertices = p->vertice[0].primeiraSaida;
    if (v <= 0 || v > numVertices)
    {
        return 0;
    }

    /* Se o vértice já foi removido (marcado como -1) */
    if (p->vertice[v].primeiraSaida == -1 && p->vertice[v].primeiraEntrada == -1)
    {
        return 0;
    }

    /* 2. Remover todas as arestas da Estrela de Saída de v (arestas que SAEM de v) */
    int a = p->vertice[v].primeiraSaida;
    while (a > 0)
    {
        int proximaAresta = p->aresta[a].proxSaida;
        int dest = p->aresta[a].omega;

        /* Remove a aresta 'a' da estrela de entrada do vértice de destino */
        removeDaEstrelaEntrada(p, dest, a);

        /* Invalida a aresta */
        p->aresta[a].alfa = 0;
        p->aresta[a].omega = 0;
        p->numArestas--;

        a = proximaAresta;
    }

    /* 3. Remover todas as arestas da Estrela de Entrada de v (arestas que ENTRAM em v) */
    a = p->vertice[v].primeiraEntrada;
    while (a > 0)
    {
        int proximaAresta = p->aresta[a].proxEntrada;
        int orig = p->aresta[a].alfa;

        /* Remove a aresta 'a' da estrela de saída do vértice de origem */
        removeDaEstrelaSaida(p, orig, a);

        /* Invalida a aresta */
        p->aresta[a].alfa = 0;
        p->aresta[a].omega = 0;
        p->numArestas--;

        a = proximaAresta;
    }

    /* 4. Marcar o vértice v como removido */
    p->vertice[v].primeiraSaida = -1;
    p->vertice[v].primeiraEntrada = -1;

    return 1; /* Sucesso */
}

int GAremoveAresta(Grafo p, int idAresta)
{
    /* 1. Validações preliminares */
    if (p == NULL || p->vertice == NULL || p->aresta == NULL)
    {
        return 0;
    }

    if (idAresta <= 0 || idAresta > p->maxArestas)
    {
        return 0;
    }

    int alfa = p->aresta[idAresta].alfa;
    int omega = p->aresta[idAresta].omega;

    /* Verifica se a aresta já está inativa/removida */
    if (alfa <= 0 || omega <= 0)
    {
        return 0;
    }

    /* 2. Remoção da Estrela de Saída do vértice 'alfa' */
    int a = p->vertice[alfa].primeiraSaida;
    int ant = 0;

    while (a > 0)
    {
        if (a == idAresta)
        {
            if (ant == 0)
            {
                p->vertice[alfa].primeiraSaida = p->aresta[a].proxSaida;
            }
            else
            {
                p->aresta[ant].proxSaida = p->aresta[a].proxSaida;
            }
            break;
        }
        ant = a;
        a = p->aresta[a].proxSaida;
    }

    /* 3. Remoção da Estrela de Entrada do vértice 'omega' */
    a = p->vertice[omega].primeiraEntrada;
    ant = 0;

    while (a > 0)
    {
        if (a == idAresta)
        {
            if (ant == 0)
            {
                p->vertice[omega].primeiraEntrada = p->aresta[a].proxEntrada;
            }
            else
            {
                p->aresta[ant].proxEntrada = p->aresta[a].proxEntrada;
            }
            break;
        }
        ant = a;
        a = p->aresta[a].proxEntrada;
    }

    /* 4. Limpeza da aresta no vetor e atualização de numArestas */
    p->aresta[idAresta].alfa = 0;
    p->aresta[idAresta].omega = 0;
    p->aresta[idAresta].proxSaida = 0;
    p->aresta[idAresta].proxEntrada = 0;

    p->numArestas--;

    return 1; /* Sucesso */
}

int GBarestaLaco(Grafo p, int a)
{
    if (p == NULL)
    {
        return 0;
    }

    if (!GBexisteIdAresta(p, a))
    {
        return 0;
    }

    return p->aresta[a].alfa == p->aresta[a].omega;
}

int GValfa(Grafo p, int a)
{
    if (p == NULL)
    {
        return 0;
    }

    if (!GBexisteIdAresta(p, a))
    {
        return 0;
    }

    return p->aresta[a].alfa;
}

int GVomega(Grafo p, int a)
{
    if (p == NULL)
    {
        return 0;
    }

    if (!GBexisteIdAresta(p, a))
    {
        return 0;
    }

    return p->aresta[a].omega;
}

int GVvizinho(Grafo p, int a, int v1)
{
    if (p == NULL)
    {
        return 0;
    }

    if (!GBexisteIdAresta(p, a))
    {
        return 0;
    }

    if (p->aresta[a].alfa == v1)
    {
        return p->aresta[a].omega;
    }

    if (p->aresta[a].omega == v1)
    {
        return p->aresta[a].alfa;
    }

    return 0;
}

/*FUNÇOES DO TP2 TAD ALGORITMOS EM GRAFOS*/

void Gagm(Grafo g, float *pesos) {
    if (g == NULL || g->vertice == NULL || g->aresta == NULL || pesos == NULL) {
        printf(">> Grafo ou vetor de pesos invalido.\n");
        return;
    }

    int numVertices = g->vertice[0].primeiraSaida; // Quantidade atual de vértices

    if (numVertices <= 0) {
        printf(">> Grafo vazio.\n");
        return;
    }

    /* Vetores auxiliares para o Algoritmo de Prim */
    float *chave = (float *) malloc((numVertices + 1) * sizeof(float));
    int *paiAresta = (int *) malloc((numVertices + 1) * sizeof(int)); // Guarda o ID da aresta escolhida
    int *pertenceAGM = (int *) calloc((numVertices + 1), sizeof(int));

    if (chave == NULL || paiAresta == NULL || pertenceAGM == NULL) {
        printf(">> Erro de alocacao de memoria.\n");
        return;
    }

    /* 1. Inicialização */
    for (int i = 1; i <= numVertices; i++) {
        chave[i] = FLT_MAX;
        paiAresta[i] = 0;
    }

    /* Começa pelo vértice 1 */
    chave[1] = 0.0f;

    /* 2. Construção da Árvore Geradora Mínima */
    for (int count = 1; count <= numVertices; count++) {
        int u = -1;
        float minChave = FLT_MAX;

        /* Seleciona o vértice fora da AGM com a menor chave de conexão */
        for (int v = 1; v <= numVertices; v++) {
            if (!pertenceAGM[v] && chave[v] < minChave) {
                minChave = chave[v];
                u = v;
            }
        }

        /* Se não encontrou vértice alcançável (grafo desconexo) */
        if (u == -1) {
            break;
        }

        pertenceAGM[u] = 1;

        /* Percorre a Estrela de Saída do vértice u para atualizar vizinhos */
        int e = g->vertice[u].primeiraSaida;
        while (e > 0) {
            int v = g->aresta[e].omega;
            float pesoAresta = pesos[e];

            /* Se v não está na AGM e o peso desta aresta for menor que a chave atual de v */
            if (!pertenceAGM[v] && pesoAresta < chave[v]) {
                chave[v] = pesoAresta;
                paiAresta[v] = e; // Guarda o ID da aresta que conecta u -> v
            }

            e = g->aresta[e].proxSaida;
        }
    }

    /* 3. Impressão do Resultado (Lista de Arestas da AGM) */
    printf("\n--- Arestas que formam a Arvore Geradora Minima (AGM) ---\n");
    float custoTotal = 0.0f;
    int arestasContadas = 0;

    for (int v = 2; v <= numVertices; v++) {
        int idAresta = paiAresta[v];
        if (idAresta > 0) {
            int orig = g->aresta[idAresta].alfa;
            int dest = g->aresta[idAresta].omega;
            float p = pesos[idAresta];

            printf("Aresta %d: (%d -> %d) | Peso: %.2f\n", idAresta, orig, dest, p);
            custoTotal += p;
            arestasContadas++;
        }
    }

    if (arestasContadas == 0) {
        printf(">> Nenhuma arvore geradora foi encontrada (grafo desconexo ou sem arestas).\n");
    } else {
        printf("Custo Total da AGM: %.2f\n", custoTotal);
    }

    /* Libera a memória alocada */
    free(chave);
    free(paiAresta);
    free(pertenceAGM);
}

void GBel(Grafo g, int inicio)
{
    if (g == NULL || !GBexisteIdVertice(g, inicio))
        return;

    int n = GInumeroVertices(g);

    int *visitado = calloc(n + 1, sizeof(int));
    int *fila = malloc((n + 1) * sizeof(int));

    if (visitado == NULL || fila == NULL)
    {
        free(visitado);
        free(fila);
        return;
    }

    int frente = 0;
    int tras = 0;

    /* Visita o vértice inicial */
    visitado[inicio] = 1;
    fila[tras++] = inicio;

    printf("v%d", inicio);

    while (frente < tras)
    {
        int v = fila[frente++];

        int a = GAprimaSaida(g, v);

        while (a != 0)
        {
            int vizinho = GVomega(g, a);

            if (!visitado[vizinho])
            {
                visitado[vizinho] = 1;

                fila[tras++] = vizinho;

                printf(",a%d,v%d", a, vizinho);
            }

            a = GAproxSaida(g, v, a);
        }
    }

    printf("\n");

    free(visitado);
    free(fila);
}

void GBep(Grafo g, int inicio)
{
    if (g == NULL || !GBexisteIdVertice(g, inicio))
    {
        return;
    }

    int n = GInumeroVertices(g);

    int *visitado = (int *)calloc(n + 1, sizeof(int));
    int *pilha = (int *)malloc((n + 1) * sizeof(int));

    if (visitado == NULL || pilha == NULL)
    {
        free(visitado);
        free(pilha);
        return;
    }

    int topo = 0;

    pilha[topo++] = inicio;
    visitado[inicio] = 1;

    printf("v%d", inicio);

    while (topo > 0)
    {
        int atual = pilha[topo - 1];

        int a = GAprimaSaida(g, atual);
        int encontrou = 0;

        while (a != 0)
        {
            int vizinho = GVomega(g, a);

            if (!visitado[vizinho])
            {
                visitado[vizinho] = 1;

                printf(",a%d,v%d", a, vizinho);

                pilha[topo++] = vizinho;

                encontrou = 1;
                break;
            }

            a = GAproxSaida(g, atual, a);
        }

        if (!encontrou)
        {
            topo--;
        }
    }

    printf("\n");

    free(visitado);
    free(pilha);
}


void GAcmc(Grafo g, float *pesos, int inicio, int fim)
{
    if (g == NULL || pesos == NULL)
    {
        return;
    }

    /* Verifica se os vértices existem */
    if (!GBexisteIdVertice(g, inicio) || !GBexisteIdVertice(g, fim))
    {
        return;
    }

    int n = GInumeroVertices(g);

    /* Vetores utilizados pelo algoritmo de Dijkstra */
    float *dist = (float *)malloc((n + 1) * sizeof(float));
    int *pai = (int *)malloc((n + 1) * sizeof(int));
    int *visitado = (int *)calloc(n + 1, sizeof(int));

    if (dist == NULL || pai == NULL || visitado == NULL)
    {
        free(dist);
        free(pai);
        free(visitado);
        return;
    }

    /*
     * Inicialização:
     * dist[v] = menor distância conhecida até v
     * pai[v] = aresta utilizada para chegar em v
     */
    for (int v = 1; v <= n; v++)
    {
        dist[v] = FLT_MAX;
        pai[v] = 0;
    }

    /* A distância do vértice inicial até ele mesmo é 0 */
    dist[inicio] = 0.0f;

    /*
     * Algoritmo de Dijkstra
     */
    for (int i = 1; i <= n; i++)
    {
        int u = -1;
        float menorDistancia = FLT_MAX;

        /*
         * Procura o vértice não visitado com a menor distância.
         */
        for (int v = 1; v <= n; v++)
        {
            if (!visitado[v] && dist[v] < menorDistancia)
            {
                menorDistancia = dist[v];
                u = v;
            }
        }

        /*
         * Não existe mais nenhum vértice alcançável.
         */
        if (u == -1)
        {
            break;
        }

        visitado[u] = 1;

        /*
         * Se já chegamos ao destino, podemos parar.
         */
        if (u == fim)
        {
            break;
        }

        /*
         * Percorre as arestas que saem de u.
         */
        int a = GAprimaSaida(g, u);

        while (a != 0)
        {
            int v = GVomega(g, a);

            /*
             * Calcula a nova distância passando pela aresta a.
             */
            float novaDistancia = dist[u] + pesos[a];

            /*
             * Se encontramos um caminho melhor para v,
             * atualizamos a distância e a aresta predecessora.
             */
            if (!visitado[v] && novaDistancia < dist[v])
            {
                dist[v] = novaDistancia;
                pai[v] = a;
            }

            a = GAproxSaida(g, u, a);
        }
    }

    /*
     * Verifica se o destino foi alcançado.
     */
    if (dist[fim] == FLT_MAX)
    {
        printf("Nao existe caminho entre v%d e v%d.\n", inicio, fim);

        free(dist);
        free(pai);
        free(visitado);

        return;
    }

    /*
     * Reconstrói o caminho.
     *
     * pai[v] contém a aresta utilizada para chegar em v.
     * A origem da aresta indica o vértice anterior.
     */
    int *caminhoArestas = (int *)malloc((n + 1) * sizeof(int));
    int quantidadeArestas = 0;

    int atual = fim;

    while (atual != inicio)
    {
        int a = pai[atual];

        /*
         * Segurança: caso não exista predecessor.
         */
        if (a == 0)
        {
            printf("Nao existe caminho entre v%d e v%d.\n", inicio, fim);

            free(caminhoArestas);
            free(dist);
            free(pai);
            free(visitado);

            return;
        }

        caminhoArestas[quantidadeArestas++] = a;

        atual = g->aresta[a].alfa;
    }

    /*
     * Imprime o caminho na ordem correta:
     *
     * v1,a3,v2,a5,v4
     */
    printf("Caminho mais curto: v%d", inicio);

    for (int i = quantidadeArestas - 1; i >= 0; i--)
    {
        int a = caminhoArestas[i];

        printf(",a%d,v%d", a, g->aresta[a].omega);
    }

    printf("\n");

    /*
     * Distância total percorrida.
     */
    printf("Distancia percorrida: %.2f\n", dist[fim]);

    /*
     * Libera memória.
     */
    free(caminhoArestas);
    free(dist);
    free(pai);
    free(visitado);
}

void GBcc(Grafo g)
{
    if (g == NULL)
    {
        return;
    }

    int n = GInumeroVertices(g);

    int *visitado = (int *)calloc(n + 1, sizeof(int));

    if (visitado == NULL)
    {
        return;
    }

    printf("Componentes conexos:\n");

    for (int inicio = 1; inicio <= n; inicio++)
    {
        if (!GBexisteIdVertice(g, inicio) || visitado[inicio])
        {
            continue;
        }

        printf("{");

        int primeiro = 1;

        /* Fila para realizar uma busca em largura */
        int *fila = (int *)malloc((n + 1) * sizeof(int));

        if (fila == NULL)
        {
            free(visitado);
            return;
        }

        int frente = 0;
        int fim = 0;

        fila[fim++] = inicio;
        visitado[inicio] = 1;

        while (frente < fim)
        {
            int u = fila[frente++];

            if (!primeiro)
            {
                printf(",");
            }

            printf("%d", u);
            primeiro = 0;

            /* Percorre as arestas de saída */
            int a = GAprimaSaida(g, u);

            while (a != 0)
            {
                int v = GVomega(g, a);

                if (!visitado[v])
                {
                    visitado[v] = 1;
                    fila[fim++] = v;
                }

                a = GAproxSaida(g, u, a);
            }

            /* Percorre as arestas de entrada */
            a = GAprimaEntrada(g, u);

            while (a != 0)
            {
                int v = GValfa(g, a);

                if (!visitado[v])
                {
                    visitado[v] = 1;
                    fila[fim++] = v;
                }

                a = GAproxEntrada(g, u, a);
            }
        }

        printf("}\n");

        free(fila);
    }

    free(visitado);
}