#ifndef _ARVORE_H_
#define _ARVORE_H_

#include <stdlib.h>
#include <stdio.h>

#define ORDEM_ARVORE 5 // ordem da arvore b

typedef struct arvoreHeader{
    char status;
	int noRaiz;
	int RRNproxNo;
    char lixo[69];

} arvoreHeader;

typedef struct registro{
	int P_ant;
	int C;
	long int Pr;
	int P_prox;
} registro;

typedef struct arvoreNo{
    char folha;
    int nroChavesIndexadas;
	int RRNdoNo;
	registro registros[ORDEM_ARVORE - 1];
} arvoreNo;

typedef struct arvore{
	FILE* arquivoIndice;
	arvoreHeader header;
	arvoreNo raiz;
	int driver;
} arvore;

arvore* criaArvore(char* nomeArquivoIndice);

void lerHeaderArvore(FILE* arquivoIndice, arvoreHeader* header);
void salvaHeaderArvore(arvore *currArvore);
void imprimeArvore(arvore *currArvore);

void lerNoArvore(arvore* currArvore, arvoreNo* novoNo, int RRN);
void insereRegistro(arvore* currArvore, registro novoRegistro);
void leRegistro(FILE* arquivoIndice, arvoreNo* registro);
void finalizaArvore(arvore *currArvore);

#endif