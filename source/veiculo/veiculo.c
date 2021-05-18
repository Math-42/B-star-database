#include "veiculo.h"
#include <stdio.h>
#include <stdlib.h>

#include "../utils/utils.h"
                    "setembro", "outubro",   "novembro", "dezembro"};

void imprimeHeader_Veiculo(veiculoHeader header) {
    printf("Imprimindo header veiculo: \n");
    printf("status: %c \n", header.status);
    printf("byteProxReg: %ld\n", header.byteProxReg);
    printf("nroRegistros: %d\n", header.nroRegistros);
    printf("nroRegRemovidos: %d\n", header.nroRegRemovidos);
    printf("descrevePrefixo: %s\n", header.descrevePrefixo);
    printf("descreveData: %s\n", header.descreveData);
    printf("descreveLugares: %s\n", header.descreveLugares);
    printf("descreveLinha: %s\n", header.descreveLinha);
    printf("descreveModelo: %s\n", header.descreveModelo);
    printf("descreveCategoria: %s\n", header.descreveCategoria);
    printf("====================\n");
}

void CreateTable_Veiculo(char nomeArquivoCSV[100], char nomeArquivoBin[100]) {
    FILE* arquivoBin = abrirBinario(nomeArquivoBin);
    FILE* arquivoCSV = abrirCSV(nomeArquivoCSV);

    if (arquivoCSV == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    veiculoHeader novoHeader;
    veiculo novoVeiculo;
    int finalDoArquivo = 0;

    novoHeader.status = '0';
    novoHeader.byteProxReg = 175;
    novoHeader.nroRegistros = 0;
    novoHeader.nroRegRemovidos = 0;

    lerHeaderCSV_Veiculo(arquivoCSV, &novoHeader);
    salvaHeader_Veiculo(arquivoBin, &novoHeader);

    while (!finalDoArquivo) {
        finalDoArquivo = lerVeiculo_CSV(arquivoCSV, &novoVeiculo);
        salvaVeiculo(arquivoBin, &novoVeiculo, &novoHeader);
    }

    novoHeader.status = '1';

    salvaHeader_Veiculo(arquivoBin, &novoHeader);

    fclose(arquivoBin);
    fclose(arquivoCSV);

    binarioNaTela(nomeArquivoBin);
}

int lerVeiculo(FILE* arquivoCSV, veiculo *novoVeiculo) {

    int tamanhoRegistro = 0;
    novoVeiculo->removido = foiRemovido(arquivoCSV);

    lerStringFixa(arquivoCSV, novoVeiculo->prefixo,5);
    lerStringFixa(arquivoCSV, novoVeiculo->data,10);

    novoVeiculo->quantidadeLugares = lerInteiro(arquivoCSV);
    novoVeiculo->codLinha = lerInteiro(arquivoCSV);

    int tamanhoModelo = lerString(arquivoCSV, novoVeiculo->modelo);
    int tamanhoCategoria = lerString(arquivoCSV, novoVeiculo->categoria);

    tamanhoRegistro += tamanhoModelo + tamanhoCategoria;
    tamanhoRegistro += 31;//tamanho da parte fixa da struct
	
    novoVeiculo->tamanhoCategoria = tamanhoCategoria;
    novoVeiculo->tamanhoModelo = tamanhoModelo;
    novoVeiculo->tamanhoRegistro = tamanhoRegistro;

    imprimeVeiculo(*novoVeiculo);
    return finalDoArquivo(arquivoCSV);
}

void imprimeVeiculo(veiculo currVeiculo) {
    printf("Imprimindo veiculo: \n");
    printf("removido: %c \n", currVeiculo.removido);
    printf("tamanhoRegistro: %d\n", currVeiculo.tamanhoRegistro);
    printf("prefixo: %s\n", currVeiculo.prefixo);
    printf("data: %s\n", currVeiculo.data);
    printf("quantidadeLugares: %d\n", currVeiculo.quantidadeLugares);
    printf("codLinha: %d\n", currVeiculo.codLinha);
    printf("tamanhoModelo: %d\n", currVeiculo.tamanhoModelo);
    printf("modelo: %s\n", currVeiculo.modelo);
    printf("tamanhoCategoria: %d\n", currVeiculo.tamanhoCategoria);
    printf("categoria: %s\n", currVeiculo.categoria);
    printf("====================\n");
}

void salvaVeiculo(FILE* arquivoBin, veiculo* currV,veiculoHeader* header) {
    fseek(arquivoBin,header->byteProxReg,SEEK_SET);

    fwrite(&currV->removido, sizeof(char), 1, arquivoBin);
    fwrite(&currV->tamanhoRegistro, sizeof(int), 1, arquivoBin);
    fwrite(&currV->prefixo, sizeof(char), 5, arquivoBin);
    fwrite(&currV->data, sizeof(char), 10, arquivoBin);
    fwrite(&currV->quantidadeLugares, sizeof(int), 1, arquivoBin);
    fwrite(&currV->codLinha, sizeof(int), 1, arquivoBin);

    fwrite(&currV->tamanhoModelo, sizeof(int), 1, arquivoBin);
    fwrite(&currV->modelo, sizeof(char), currV->tamanhoModelo,arquivoBin);

    fwrite(&currV->tamanhoCategoria, sizeof(int), 1, arquivoBin);
    fwrite(&currV->categoria, sizeof(char), currV->tamanhoCategoria,arquivoBin);

	header->byteProxReg = ftell(arquivoBin);
	header->nroRegRemovidos += (currV->removido == '0')? 1:0;
	header->nroRegistros += (currV->removido == '0')? 0:1;
}

void lerHeaderCSV_Veiculo(FILE* arquivoCSV, veiculoHeader* header) {
    lerString(arquivoCSV, header->descrevePrefixo);
    lerString(arquivoCSV, header->descreveData);
    lerString(arquivoCSV, header->descreveLugares);
    lerString(arquivoCSV, header->descreveLinha);
    lerString(arquivoCSV, header->descreveModelo);
    lerString(arquivoCSV, header->descreveCategoria);
}

void lerHeaderBin_Veiculo(FILE* arquivoBin, veiculoHeader* header) {
    long int currPos = ftell(arquivoBin);
    fseek(arquivoBin, 0, 0);

    fread(&header->status, sizeof(char), 1, arquivoBin);
    fread(&header->byteProxReg, sizeof(long int), 1, arquivoBin);
    fread(&header->nroRegistros, sizeof(int), 1, arquivoBin);
    fread(&header->nroRegRemovidos, sizeof(int), 1, arquivoBin);
    fread(&header->descrevePrefixo, 18, 1, arquivoBin);
    fread(&header->descreveData, 35, 1, arquivoBin);
    fread(&header->descreveLugares, 42, 1, arquivoBin);
    fread(&header->descreveLinha, 26, 1, arquivoBin);
    fread(&header->descreveModelo, 17, 1, arquivoBin);
    fread(&header->descreveCategoria, 20, 1, arquivoBin);

    fseek(arquivoBin, currPos, 0);
}

void salvaHeaderCSV_Veiculo(FILE* arquivoBin, veiculoHeader* header) {
    fseek(arquivoBin, 0, 0);
	imprimeHeader_Veiculo(*header);
    fwrite(&header->status, sizeof(char), 1, arquivoBin);
    fwrite(&(header->byteProxReg), sizeof(long int), 1, arquivoBin);
    fwrite(&(header->nroRegistros), sizeof(int), 1, arquivoBin);
    fwrite(&(header->nroRegRemovidos), sizeof(int), 1, arquivoBin);
    fwrite(&(header->descrevePrefixo), sizeof(char), 18, arquivoBin);
    fwrite(&(header->descreveData), sizeof(char), 35, arquivoBin);
    fwrite(&(header->descreveLugares), sizeof(char), 42, arquivoBin);
    fwrite(&(header->descreveLinha), sizeof(char), 26, arquivoBin);
    fwrite(&(header->descreveModelo), sizeof(char), 17, arquivoBin);
    fwrite(&(header->descreveCategoria), sizeof(char), 20, arquivoBin);
}