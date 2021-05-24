#ifndef _LINHA_H_
#define _LINHA_H_
#include <stdio.h>
typedef struct linhaHeader{
    char status;
    long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[16];
    char descreveCartao[14];
    char descreveNome[14];
    char descreveLinha[25];
} linhaHeader;

typedef struct linha{
    char removido;
    int tamanhoRegistro;
    int codLinha;
    char aceitaCartao[5];//Precaução para possiveis valores nulos "NULO\0"
    int tamanhoNome;
    char nomeLinha[100];
    int tamanhoCor;
    char corLinha[100];
} linha;

int lerLinha_Bin(FILE* arquivoBin, linha *novoLinha);
int lerLinha_CSV(FILE* arquivoCSV, linha *novoLinha);
void imprimeLinha(linha currLinha);
void salvaLinha(FILE* arquivoBin, linha* currLinha,linhaHeader* header);
void lerHeaderCSV_Linha(FILE* arquivoCSV, linhaHeader *header);
void lerHeaderBin_Linha(FILE* arquivoBin, linhaHeader *header);
void salvaHeader_Linha(FILE* arquivoBin, linhaHeader *header);

void SelectFrom_Linha(char nomeArquivoBin[100]);
void CreateTable_Linha(char nomeArquivoCSV[100], char nomeArquivoBin[100]);


#endif