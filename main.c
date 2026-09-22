#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

void exibirMenu(Grafo *g);
int existe;
int v1, v2;
int a1, a2;

int main(void) {
    Grafo g = NULL;

    printf("======================================================\n");
    printf("   BEM-VINDO AO SISTEMA DE GERENCIAMENTO DE GRAFOS   \n");
    printf("======================================================\n");

    exibirMenu(&g);

    if (g != NULL) {
        GGdestroiGrafo(g);
        printf(">> Memoria do grafo liberada com sucesso.\n");
    }

    printf(">> Programa finalizado.\n");
    return 0;
}

void exibirMenu(Grafo *g) {
    int opcao = -1;
    int id;
    int maxV, maxA;

    while (opcao != 0) {
        printf("\n+----------------------------------- MENU GRAFO -----------------------------------+\n");
        printf("| [ESTRUTURA]                         | [ITERACAO & PROPRIEDADES]                  |\n");
        printf("|  01-Criar novo grafo                |  11-Pegar primeiro vertice                 |\n");
        printf("|  02-Destruir grafo                  |  12-Pegar proximo vertice                  |\n");
        printf("|  03-Criar vertice                   |  13-Pegar primeira aresta                  |\n");
        printf("|  04-Criar aresta                    |  14-Pegar proxima aresta                   |\n");
        printf("|  32-Remover vertice                 |  15-Numero de vertices                     |\n");
        printf("|  33-Remover aresta                  |  16-Numero maximo de vertices              |\n");
        printf("|                                     |  17-Numero de arestas                      |\n");
        printf("| [CONSULTAS]                         |  18-Numero maximo de arestas               |\n");
        printf("|  05-Verificar se existe vertice     |  21-Pegar grau do vertice                  |\n");
        printf("|  06-Verificar se existe aresta      |  22-Primeira aresta do vertice             |\n");
        printf("|  07-Verificar aresta dirigida       |  23-Proxima aresta do vertice              |\n");
        printf("|  08-Verificar aresta nao dirigida   |  24-Primeira aresta de entrada             |\n");
        printf("|  09-Pegar aresta dirigida           |  25-Proxima aresta de entrada              |\n");
        printf("|  10-Pegar aresta nao dirigida       |  26-Primeira aresta de salida              |\n");
        printf("|                                     |  27-Proxima aresta de salida               |\n");
        printf("| [ARQUIVOS]                          |  28-Verificar se e laco                    |\n");
        printf("|  19-Carregar grafo de arquivo       |  20-Salvar grafo em arquivo                |\n");
        printf("| [CAMINHO CURTO]                     |  31-Pegar vertice vizinho                  |\n");
        printf("|  34-Encontrar menor caminho         |                                            |\n");
        printf("+-------------------------------------+--------------------------------------------+\n");
        printf("|                                 0-Sair                                           |\n");
        printf("+----------------------------------------------------------------------------------+\n");

        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            /* Limpa o buffer de entrada caso o usuario digite algo invalido */
            while (getchar() != '\n');
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1:
                printf("\nDigite o numero maximo de vertices (v): ");
                scanf("%d", &maxV);
                printf("Digite o numero maximo de arestas (a): ");
                scanf("%d", &maxA);
                
                if (*g != NULL) {
                    *g = GGdestroiGrafo(*g);
                }
                
                *g = GGcriaGrafo(maxV, maxA);
                if (*g != NULL) {
                    printf(">> Grafo vazio criado com sucesso! (Capacidade: %d vertices, %d arestas)\n", maxV, maxA);
                } else {
                    printf(">> Falha ao criar o grafo. Verifique os limites inseridos.\n");
                }
                break;

            case 2:
                if (*g == NULL) {
                    printf(">> Nao existe grafo alocado para destruir.\n");
                } else {
                    *g = GGdestroiGrafo(*g);
                    printf(">> Grafo destruido e memoria liberada com sucesso!\n");
                }
                break;

            case 3:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }
                id = GVcriaVertice(*g);
                if (id > 0) {
                    printf(">> Vertice criado com sucesso! ID do Vertice: %d\n", id);
                } else {
                    printf(">> Nao foi possivel criar o vertice (limite maximo atingido).\n");
                }
                break;
            case 4:  

                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }

                int alfa, omega;
                printf("Digite o vértice de origem(alfa): ");
                scanf("%d", &alfa);

                printf("Digite o vértice de destino(omega): ");
                scanf("%d", &omega);

                int aresta = GAcriaAresta(*g, alfa, omega);
                if (aresta == 0) {
                    printf("Não foi possivel criar a aresta.\n");
                } else {
                    printf("Aresta criada com sucesso! ID: %d\n", aresta);
                }
                break;

            case 5:
                int identificadorVertice;

                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }

                printf("Digite o id do vértice:");
                scanf("%d", &identificadorVertice);

                existe = GBexisteIdVertice(*g, identificadorVertice);

                if(existe == 0){
                    printf(">> O vértice não existe.\n");
                }else{
                    printf(">> O vértice existe.");
                }
                break;

            case 6:

                int identificadorAresta;

                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }

                printf("Digite o id da aresta:");
                scanf("%d", &identificadorAresta);

                existe = GBexisteIdAresta(*g, identificadorAresta);

                if(existe == 0){
                    printf(">> A aresta não existe.\n");
                }else{
                    printf(">> A aresta existe.");
                }
                break;

            case 7:
                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }

                printf("Digite o id do vértice de partida:");
                scanf("%d", &v1);

                printf("Digite o id do vértice de chegada:");
                scanf("%d", &v2);

                existe = GBexisteArestaDir(*g, v1, v2);

                if(existe == 0){
                    printf(">> A aresta dirigida não existe.\n");
                }else{
                    printf(">> A aresta dirigida existe.");
                }
                
                break;
            case 8:
                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }

                printf("Digite o id do vértice 1:");
                scanf("%d", &v1);

                printf("Digite o id do vértice 2:");
                scanf("%d", &v2);

                existe = GBexisteAresta(*g, v1, v2);

                if(existe == 0){
                    printf(">> A aresta não dirigida não existe.\n");
                }else{
                    printf(">> A aresta não dirigida existe.");
                }
                
                break;
            case 9:
                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }

                printf("Digite o id do vértice de partida:");
                scanf("%d", &v1);

                printf("Digite o id do vértice de chegada:");
                scanf("%d", &v2);

                existe = GApegaArestaDir(*g, v1, v2);

                if(existe == 0){
                    printf(">> A aresta dirigida não existe.\n");
                }else{
                    printf(">> A aresta dirigida existe. Id: %d\n", existe);
                }
                
                break;
            case 10:
                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }

                printf("Digite o id do vértice 1:");
                scanf("%d", &v1);

                printf("Digite o id do vértice 2:");
                scanf("%d", &v2);

                existe = GApegaAresta(*g, v1, v2);

                if(existe == 0){
                    printf(">> A aresta não dirigida não existe.\n");
                }else{
                   printf(">> A aresta não dirigida existe. Id: %d\n", existe);
                }
                
                break;

            case 11:
                if (*g == NULL) {
                    printf(">> Crie ou carregue um grafo primeiro.\n");
                    break;
                }
                existe = GVprimeiroVertice(*g);
                if(existe == 0){
                    printf(">> Não existe vértice.\n");
                }else{
                   printf(">> Id do primeiro vértice: %d\n", existe);
                }
                
                break;

                case 12:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }
                printf("Digite o ID do vertice atual (v1): ");
                scanf("%d", &v1);

                v2 = GVproximoVertice(*g, v1);

                if (v2 != 0) {
                    printf(">> O proximo vertice existente apos %d e: %d\n", v1, v2);
                } else {
                    printf(">> Nao existe proximo vertice apos %d.\n", v1);
                }
                break;

                case 13:
                    if (*g == NULL) { 
                        printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                        break; 
                    }

                    a1 = GAprimeiraAresta(*g);

                    if (a1 != 0) {
                        printf(">> A primeira aresta do grafo e: %d\n", a1);
                    } else {
                        printf(">> O grafo nao possui arestas criadas.\n");
                    }
                    break;

                case 14:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }
                printf("Digite o ID da aresta atual (a1): ");
                scanf("%d", &a1);

                a2 = GAproximaAresta(*g, a1);

                if (a2 != 0) {
                    printf(">> A proxima aresta existente apos %d e: %d\n", a1, a2);
                } else {
                    printf(">> Nao existe proxima aresta apos %d.\n", a1);
                }
                break;

                case 15:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }

                int nv = GInumeroVertices(*g);
                printf(">> Numero total de vertices (|V|): %d\n", nv);
                break;

                
                case 16:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }

                int maxV = GInumeroVerticesMax(*g);
                printf(">> Numero maximo de vertices previsto: %d\n", maxV);
                break;
                
                case 17:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }

                int na = GInumeroArestas(*g);
                printf(">> Numero total de arestas (|A|): %d\n", na);
                break;

                 case 18:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }
                
                int maxA = GInumeroArestasMax(*g);
                printf(">> Numero maximo de arestas previsto: %d\n", maxA);
                break;

                case 19:
                {
                    char nomeArquivo[100];
                    printf("Digite o nome do arquivo a ser carregado (ex: grafo.txt): ");
                    scanf("%99s", nomeArquivo);

                    /* Se ja houver um grafo alocado, destroi antes de carregar o novo */
                    if (*g != NULL) {
                        GGdestroiGrafo(*g);
                        *g = NULL;
                    }

                    *g = GGcarregaGrafo(nomeArquivo);

                    if (*g != NULL) {
                        printf(">> Grafo carregado com sucesso a partir de '%s'!\n", nomeArquivo);
                    } else {
                        printf(">> Falha ao carregar o grafo.\n");
                    }
                }
                break;
                
                case 20:
                if (*g == NULL) { 
                    printf(">> Crie ou carregue um grafo primeiro (Opcao 01).\n"); 
                    break; 
                }
                {
                    char nomeArquivo[100];
                    printf("Digite o nome do arquivo para salvar (ex: grafo_salvo.txt): ");
                    scanf("%99s", nomeArquivo);

                    if (GBsalvaGrafo(*g, nomeArquivo)) {
                        printf(">> Grafo salvo com sucesso em '%s'!\n", nomeArquivo);
                    } else {
                        printf(">> Falha ao salvar o grafo em arquivo.\n");
                    }
                }
                break;
                
                case 0:
                printf("\n>> Encerrando o programa...\n");
                break;

            default:
                printf("\n>> Opcao invalida! Tente novamente.\n");
                break;
        }
    }
}