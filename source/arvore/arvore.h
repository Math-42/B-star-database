#ifndef _ARVORE_H_
#define _ARVORE_H_

#include <stdio.h>
#include <stdlib.h>

#define ORDEM_ARVORE 5  // ordem da arvore b

typedef struct arvoreHeader {
    char status;
    int noRaiz;
    int RRNproxNo;
    char lixo[69];  //levando em consideração o /0

} arvoreHeader;

typedef struct registro {
    int P_ant;  // P[n]
    int C;
    long int Pr;
    int P_prox;  // P[n+1]
} registro;

/**
 * Para facilitar a implementação os registros foram separados em structs próprias, sendo assim
 * P_ant e P_prox são equivalentes aos P's originais
 * 
 * P1 = registros[0].P_ant
 * P2 = registros[1].P_ant = registros[0].P_prox
 * P3 = registros[2].P_ant = registros[1].P_prox
 * P4 = registros[3].P_ant = registros[2].P_prox
 * P5 = registros[3].P_prox
 */

typedef struct arvoreNo {
    char folha;
    int nroChavesIndexadas;
    int RRNdoNo;
    registro registros[ORDEM_ARVORE - 1];
} arvoreNo;

typedef struct arvore {
    FILE* arquivoIndice;
    arvoreHeader header;
    arvoreNo raiz;
} arvore;

arvore* criaArvore(char* nomeArquivoIndice);
arvore* carregaArvore(char* nomeArquivoIndice);

void alteraStatusArvore(arvore* currArvore);
void lerHeaderArvore(FILE* arquivoIndice, arvoreHeader* header);
void salvaHeaderArvore(arvore* currArvore);
void imprimeArvore(arvore* currArvore);

void lerNoArvore(arvore* currArvore, arvoreNo* novoNo, int RRN);
void insereRegistro(arvore* currArvore, registro novoRegistro);
long int buscaRegistro(arvore* currArvore, int chave);
void leRegistro(FILE* arquivoIndice, arvoreNo* registro);
void finalizaArvore(arvore* currArvore);

#endif