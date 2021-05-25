#include "linha.h"
#include <stdio.h>
#include <stdlib.h>
#include "../binario/binario.h"
#include "../csv/csv.h"
#include "../utils/utils.h"

void imprimeHeader_Linha(linhaHeader header) {
    printf("Imprimindo header linha: \n");
    printf("status: %c \n", header.status);
    printf("byteProxReg: %ld\n", header.byteProxReg);
    printf("nroRegistros: %d\n", header.nroRegistros);
    printf("nroRegRemovidos: %d\n", header.nroRegRemovidos);
    printf("descreveCodigo: %s\n", header.descreveCodigo);
    printf("descreveCartao: %s\n", header.descreveCartao);
    printf("descreveNome: %s\n", header.descreveNome);
    printf("descreveLinha: %s\n", header.descreveLinha);
    printf("====================\n");
}

void imprimirCartao(char* cartao){
    printf("Aceita cartao: ");
    switch (cartao[0]){
    case 'S':
        printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR");
        break;
    case 'N':
        printf("PAGAMENTO EM CARTAO E DINHEIRO");
        break;
    case 'F':
        printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA");
        break;
    default:
        printf("campo com valor nulo");
        break;
    }
    printf("\n");
}

void CreateTable_Linha(char nomeArquivoCSV[100], char nomeArquivoBin[100]) {
    FILE* arquivoBin = fopen(nomeArquivoBin,"wb");
    FILE* arquivoCSV = fopen(nomeArquivoCSV,"r");

    if (arquivoCSV == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    linhaHeader novoHeader;
    linha novoLinha;
    int finalDoArquivo = 0;

    novoHeader.status = '0';
    novoHeader.byteProxReg = 82;
    novoHeader.nroRegistros = 0;
    novoHeader.nroRegRemovidos = 0;
    
    lerHeaderCSV_Linha(arquivoCSV, &novoHeader);
    salvaHeader_Linha(arquivoBin, &novoHeader);

    while (!finalDoArquivo) {
        finalDoArquivo = lerLinha_CSV(arquivoCSV, &novoLinha);
        salvaLinha(arquivoBin, &novoLinha, &novoHeader);
    }
    

    novoHeader.status = '1';

    salvaHeader_Linha(arquivoBin, &novoHeader);

    fclose(arquivoBin);
    fclose(arquivoCSV);

    binarioNaTela(nomeArquivoBin);
}

void SelectFrom_Linha(char nomeArquivoBin[100]) {
    FILE* arquivoBin = fopen(nomeArquivoBin,"rb");
    linhaHeader novoHeader;
    linha novoLinha;

    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }
    
    lerHeaderBin_Linha(arquivoBin, &novoHeader);

    if (novoHeader.nroRegistros == 0) {
        printf("Registro inexistente.");
        return;
    }

    novoHeader.status = '0';
    salvaHeader_Linha(arquivoBin, &novoHeader);
    int finalDoArquivo = 0;

    while (!finalDoArquivo) {
        finalDoArquivo = lerLinha_Bin(arquivoBin, &novoLinha);
        if (novoLinha.removido == '1') imprimeLinha(novoLinha);
    }
    novoHeader.status = '1';
    salvaHeader_Linha(arquivoBin, &novoHeader);

    fclose(arquivoBin);
}

int lerLinha_CSV(FILE* arquivoCSV, linha* novoLinha) {
    int tamanhoRegistro = 0;

    novoLinha->removido = foiRemovido(arquivoCSV);

    novoLinha->codLinha = lerInteiro(arquivoCSV);

    lerStringFixa(arquivoCSV, novoLinha->aceitaCartao, 1);

    int tamanhoNome = lerString(arquivoCSV, novoLinha->nomeLinha);
    int tamanhoCor = lerString(arquivoCSV, novoLinha->corLinha);


    tamanhoRegistro += tamanhoNome + tamanhoCor;
    tamanhoRegistro += 13;  // tamanho da parte fixa da struct

    novoLinha->tamanhoNome = tamanhoNome;
    novoLinha->tamanhoCor = tamanhoCor;
    novoLinha->tamanhoRegistro = tamanhoRegistro;

    return finalDoArquivo(arquivoCSV);
}

int lerLinha_Bin(FILE* arquivoBin, linha* currV) {

    lerStringBin(arquivoBin, &currV->removido, 1);

    currV->tamanhoRegistro = lerInteiroBin(arquivoBin);
    currV->codLinha = lerInteiroBin(arquivoBin);

    lerStringBin(arquivoBin, currV->aceitaCartao, 1);

    currV->tamanhoNome = lerInteiroBin(arquivoBin);
    lerStringBin(arquivoBin, currV->nomeLinha, currV->tamanhoNome);

    currV->tamanhoCor = lerInteiroBin(arquivoBin);
    lerStringBin(arquivoBin, currV->corLinha, currV->tamanhoCor);

    return finalDoArquivo(arquivoBin);
}

void imprimeLinha(linha currLinha) {

    imprimirCampo("Codigo da linha",&currLinha.codLinha,1);
    imprimirCampo("Nome da linha",currLinha.nomeLinha,0);
    imprimirCampo("Cor que descreve a linha",currLinha.corLinha,0);
    imprimirCartao(currLinha.aceitaCartao);

    printf("\n");

}

void salvaLinha(FILE* arquivoBin, linha* currV, linhaHeader* header) {
    fseek(arquivoBin, header->byteProxReg, SEEK_SET);

    fwrite(&currV->removido, sizeof(char), 1, arquivoBin);
    fwrite(&currV->tamanhoRegistro, sizeof(int), 1, arquivoBin);
    fwrite(&currV->codLinha, sizeof(int), 1, arquivoBin);
    fwrite(&currV->aceitaCartao, sizeof(char), 1, arquivoBin);

    fwrite(&currV->tamanhoNome, sizeof(int), 1, arquivoBin);
    fwrite(&currV->nomeLinha, sizeof(char), currV->tamanhoNome, arquivoBin);

    fwrite(&currV->tamanhoCor, sizeof(int), 1, arquivoBin);
    fwrite(&currV->corLinha, sizeof(char), currV->tamanhoCor,arquivoBin);

    header->byteProxReg = ftell(arquivoBin);
    header->nroRegRemovidos += (currV->removido == '0') ? 1 : 0;
    header->nroRegistros += (currV->removido == '0') ? 0 : 1;
}

void lerHeaderCSV_Linha(FILE* arquivoCSV, linhaHeader* header) {
    lerString(arquivoCSV, header->descreveCodigo);
    lerString(arquivoCSV, header->descreveCartao);
    lerString(arquivoCSV, header->descreveNome);
    lerString(arquivoCSV, header->descreveLinha);
}

void lerHeaderBin_Linha(FILE* arquivoBin, linhaHeader* header) {
    fseek(arquivoBin, 0, SEEK_SET);

    fread(&header->status, sizeof(char), 1, arquivoBin);
    fread(&(header->byteProxReg), sizeof(long int), 1, arquivoBin);
    fread(&(header->nroRegistros), sizeof(int), 1, arquivoBin);
    fread(&(header->nroRegRemovidos), sizeof(int), 1, arquivoBin);
    fread(&(header->descreveCodigo), sizeof(char), 15, arquivoBin);
    fread(&(header->descreveCartao), sizeof(char), 13, arquivoBin);
    fread(&(header->descreveNome), sizeof(char), 13, arquivoBin);
    fread(&(header->descreveLinha), sizeof(char), 24, arquivoBin);
}

void salvaHeader_Linha(FILE* arquivoBin, linhaHeader* header) {
    fseek(arquivoBin, 0, 0);

    fwrite(&header->status, sizeof(char), 1, arquivoBin);
    fwrite(&(header->byteProxReg), sizeof(long int), 1, arquivoBin);
    fwrite(&(header->nroRegistros), sizeof(int), 1, arquivoBin);
    fwrite(&(header->nroRegRemovidos), sizeof(int), 1, arquivoBin);
    fwrite(&(header->descreveCodigo), sizeof(char), 15, arquivoBin);
    fwrite(&(header->descreveCartao), sizeof(char), 13, arquivoBin);
    fwrite(&(header->descreveNome), sizeof(char), 13, arquivoBin);
    fwrite(&(header->descreveLinha), sizeof(char), 24, arquivoBin);
}