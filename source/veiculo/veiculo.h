#ifndef _VEICULO_H_
#define _VEICULO_H_
#include <stdio.h>

typedef struct veiculoHeader{
    char status;
    long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[19];
    char descreveData[36];
    char descreveLugares[43];
    char descreveLinha[27];
    char descreveModelo[18];
    char descreveCategoria[21];

} veiculoHeader;

typedef struct veiculo{
    char removido;
    int tamanhoRegistro;
    char prefixo[6];
    char data[11];
    int quantidadeLugares;
    int codLinha;
    int tamanhoModelo;
    char modelo[100];
    int tamanhoCategoria;
    char categoria[100];
} veiculo;

void CreateTable_Veiculo(char nomeArquivoCSV[100], char nomeArquivoBin[100]);
int lerVeiculo(FILE* arquivoCSV, veiculo *novoVeiculo);
void imprimeVeiculo(veiculo currVeiculo);
void salvaVeiculo(FILE* arquivoBin, veiculo* currVeiculo,veiculoHeader* header);
void lerHeaderCSV_Veiculo(FILE* arquivoCSV, veiculoHeader *header);
void lerHeaderBin_Veiculo(FILE* arquivoBin, veiculoHeader *header);
void salvaHeader_Veiculo(FILE* arquivoBin, veiculoHeader *header);



#endif