#include "veiculo.h"

#include <stdio.h>
#include <stdlib.h>

#include "../binario/binario.h"
#include "../csv/csv.h"
#include "../utils/utils.h"

char MESES[][12] = {"janeiro",  "fevereiro", "março",    "abril",
                    "maio",     "junho",     "julho",    "agosto",
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

void imprimeData(char* stringData) {
    printf("Data de entrada do veiculo na frota: ");

    if (stringData[0] != '\0') {
        int indiceDoMes = (stringData[5] - '0') * 10 + (stringData[6] - '0') -1;  // calcula o indice do mes e translada para entre 0-11
        printf("%.2s de %s de %.4s\n", stringData + 8,MESES[indiceDoMes],stringData);
    } else {
        printf("campo com valor nulo\n");
    }
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

void SelectFrom_Veiculo(char nomeArquivoBin[100]) {
    FILE* arquivoBin = abrirBinario(nomeArquivoBin);

    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    veiculoHeader novoHeader;
    veiculo novoVeiculo;

    lerHeaderBin_Veiculo(arquivoBin, &novoHeader);
    novoHeader.status = '0';
    // imprimeHeader_Veiculo(novoHeader);
    salvaHeader_Veiculo(arquivoBin, &novoHeader);
    int finalDoArquivo = 0;

    while (!finalDoArquivo) {
        finalDoArquivo = lerVeiculo_Bin(arquivoBin, &novoVeiculo);
        if (novoVeiculo.removido == '1') imprimeVeiculo(novoVeiculo);
    }
    novoHeader.status = '1';
    salvaHeader_Veiculo(arquivoBin, &novoHeader);

    fclose(arquivoBin);
}

int lerVeiculo_CSV(FILE* arquivoCSV, veiculo* novoVeiculo) {
    int tamanhoRegistro = 0;
    novoVeiculo->removido = foiRemovido(arquivoCSV);

    lerStringFixa(arquivoCSV, novoVeiculo->prefixo, 5);
    lerStringFixa(arquivoCSV, novoVeiculo->data, 10);

    novoVeiculo->quantidadeLugares = lerInteiro(arquivoCSV);
    novoVeiculo->codLinha = lerInteiro(arquivoCSV);

    int tamanhoModelo = lerString(arquivoCSV, novoVeiculo->modelo);
    int tamanhoCategoria = lerString(arquivoCSV, novoVeiculo->categoria);

    tamanhoRegistro += tamanhoModelo + tamanhoCategoria;
    tamanhoRegistro += 31;  // tamanho da parte fixa da struct

    novoVeiculo->tamanhoCategoria = tamanhoCategoria;
    novoVeiculo->tamanhoModelo = tamanhoModelo;
    novoVeiculo->tamanhoRegistro = tamanhoRegistro;

    return finalDoArquivo(arquivoCSV);
}

int lerVeiculo_Bin(FILE* arquivoBin, veiculo* currV) {
    lerStringBin(arquivoBin, &currV->removido, 1);
    currV->tamanhoRegistro = lerInteiroBin(arquivoBin);

    lerStringBin(arquivoBin, currV->prefixo, 5);
    lerStringBin(arquivoBin, currV->data, 10);

    currV->quantidadeLugares = lerInteiroBin(arquivoBin);
    currV->codLinha = lerInteiroBin(arquivoBin);

    currV->tamanhoModelo = lerInteiroBin(arquivoBin);
    lerStringBin(arquivoBin, currV->modelo, currV->tamanhoModelo);

    currV->tamanhoCategoria = lerInteiroBin(arquivoBin);
    lerStringBin(arquivoBin, currV->categoria, currV->tamanhoCategoria);

    return finalDoArquivo(arquivoBin);
}

void imprimeVeiculo(veiculo currVeiculo) {

    imprimirCampo("Prefixo do veiculo",currVeiculo.prefixo,0);
    imprimirCampo("Modelo do veiculo",currVeiculo.modelo,0);
    imprimirCampo("Categoria do veiculo",currVeiculo.categoria,0);
    imprimeData(currVeiculo.data);
    imprimirCampo("Quantidade de lugares sentados disponiveis",&currVeiculo.quantidadeLugares,1);
    printf("\n");

}

void salvaVeiculo(FILE* arquivoBin, veiculo* currV, veiculoHeader* header) {
    fseek(arquivoBin, header->byteProxReg, SEEK_SET);

    fwrite(&currV->removido, sizeof(char), 1, arquivoBin);
    fwrite(&currV->tamanhoRegistro, sizeof(int), 1, arquivoBin);
    fwrite(&currV->prefixo, sizeof(char), 5, arquivoBin);
    fwrite(&currV->data, sizeof(char), 10, arquivoBin);
    fwrite(&currV->quantidadeLugares, sizeof(int), 1, arquivoBin);
    fwrite(&currV->codLinha, sizeof(int), 1, arquivoBin);

    fwrite(&currV->tamanhoModelo, sizeof(int), 1, arquivoBin);
    fwrite(&currV->modelo, sizeof(char), currV->tamanhoModelo, arquivoBin);

    fwrite(&currV->tamanhoCategoria, sizeof(int), 1, arquivoBin);
    fwrite(&currV->categoria, sizeof(char), currV->tamanhoCategoria,
           arquivoBin);

    header->byteProxReg = ftell(arquivoBin);
    header->nroRegRemovidos += (currV->removido == '0') ? 1 : 0;
    header->nroRegistros += (currV->removido == '0') ? 0 : 1;
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
    fseek(arquivoBin, 0, SEEK_SET);

    fread(&header->status, sizeof(char), 1, arquivoBin);
    fread(&header->byteProxReg, sizeof(long int), 1, arquivoBin);
    fread(&header->nroRegistros, sizeof(int), 1, arquivoBin);
    fread(&header->nroRegRemovidos, sizeof(int), 1, arquivoBin);
    fread(&header->descrevePrefixo, sizeof(char), 18, arquivoBin);
    fread(&header->descreveData, sizeof(char), 35, arquivoBin);
    fread(&header->descreveLugares, sizeof(char), 42, arquivoBin);
    fread(&header->descreveLinha, sizeof(char), 26, arquivoBin);
    fread(&header->descreveModelo, sizeof(char), 17, arquivoBin);
    fread(&header->descreveCategoria, sizeof(char), 20, arquivoBin);
}

void salvaHeader_Veiculo(FILE* arquivoBin, veiculoHeader* header) {
    fseek(arquivoBin, 0, 0);

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