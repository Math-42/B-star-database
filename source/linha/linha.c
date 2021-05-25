#include "linha.h"
#include <stdio.h>
#include <stdlib.h>
#include "../binario/binario.h"
#include "../csv/csv.h"
#include "../utils/utils.h"

/**
 * Imprime o tipo de pagamento no cartão no formato solicitado
 * @param stringData string original no formato salvo
 */
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

/**
 * Cria um arquivo binário na estrutura solicitada de header e campos a partir de um arquivo CSV
 * @param nomeArquivoCSV nome do arquivo csv fonte dos dados
 * @param nomeArquivoBin nome do arquivo binário onde os dados serão salvos
 */
void CreateTable_Linha(char nomeArquivoCSV[100], char nomeArquivoBin[100]) {
    FILE* arquivoBin = fopen(nomeArquivoBin,"wb");
    FILE* arquivoCSV = fopen(nomeArquivoCSV,"r");

    if (arquivoCSV == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    linhaHeader novoHeader;
    linha novaLinha;
    int finalDoArquivo = 0;

    //valores iniciais do header
    novoHeader.status = '0';
    novoHeader.byteProxReg = 82;
    novoHeader.nroRegistros = 0;
    novoHeader.nroRegRemovidos = 0;
    
    lerHeaderCSV_Linha(arquivoCSV, &novoHeader);
    salvaHeader_Linha(arquivoBin, &novoHeader);

    while (!finalDoArquivo) {
        finalDoArquivo = lerLinha_CSV(arquivoCSV, &novaLinha);
        salvaLinha(arquivoBin, &novaLinha, &novoHeader);
    }
    

    novoHeader.status = '1';

    salvaHeader_Linha(arquivoBin, &novoHeader);

    fclose(arquivoBin);
    fclose(arquivoCSV);

    binarioNaTela(nomeArquivoBin);
}

/**
 * Imprime todos os valores de um binário
 * @param nomeArquivoBin nome do arquivo binário de onde os dados serão lidos
 */
void SelectFrom_Linha(char nomeArquivoBin[100]) {
    FILE* arquivoBin = fopen(nomeArquivoBin,"rb");
    linhaHeader novoHeader;
    linha novaLinha;

    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }
    
    lerHeaderBin_Linha(arquivoBin, &novoHeader);

    if (novoHeader.status == 0) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    if (novoHeader.nroRegistros == 0) {
        printf("Registro inexistente.");
        return;
    }

    novoHeader.status = '0';
    salvaHeader_Linha(arquivoBin, &novoHeader);
    int finalDoArquivo = 0;

    while (!finalDoArquivo) {
        finalDoArquivo = lerLinha_Bin(arquivoBin, &novaLinha);
        if (novaLinha.removido == '1') imprimeLinha(novaLinha);
    }
    novoHeader.status = '1';
    salvaHeader_Linha(arquivoBin, &novoHeader);

    fclose(arquivoBin);
}

/**
 * Lê um registro de uma linha do arquivo CSV lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param arquivoCSV nome do arquivo csv fonte dos dados
 * @param novaLinha variavel para salvar os dados
 * @return retorna 1 caso for o utimo registro e 0 caso contrário
 */
int lerLinha_CSV(FILE* arquivoCSV, linha* novaLinha) {
    int tamanhoRegistro = 0;

    novaLinha->removido = foiRemovido(arquivoCSV);

    novaLinha->codLinha = lerInteiro(arquivoCSV);

    lerStringFixa(arquivoCSV, novaLinha->aceitaCartao, 1);

    int tamanhoNome = lerString(arquivoCSV, novaLinha->nomeLinha);
    int tamanhoCor = lerString(arquivoCSV, novaLinha->corLinha);


    tamanhoRegistro += tamanhoNome + tamanhoCor;
    tamanhoRegistro += 13;  // tamanho da parte fixa da struct

    novaLinha->tamanhoNome = tamanhoNome;
    novaLinha->tamanhoCor = tamanhoCor;
    novaLinha->tamanhoRegistro = tamanhoRegistro;

    return finalDoArquivo(arquivoCSV);
}

/**
 * Lê um registro de linha do arquivo binário lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param arquivoBin nome do arquivo binário fonte dos dados
 * @param currL variavel para salvar os dados
 * @return retorna 1 caso for o utimo registro e 0 caso contrário
 */
int lerLinha_Bin(FILE* arquivoBin, linha* currL) {

    lerStringBin(arquivoBin, &currL->removido, 1);

    currL->tamanhoRegistro = lerInteiroBin(arquivoBin);
    currL->codLinha = lerInteiroBin(arquivoBin);

    lerStringBin(arquivoBin, currL->aceitaCartao, 1);

    currL->tamanhoNome = lerInteiroBin(arquivoBin);
    lerStringBin(arquivoBin, currL->nomeLinha, currL->tamanhoNome);

    currL->tamanhoCor = lerInteiroBin(arquivoBin);
    lerStringBin(arquivoBin, currL->corLinha, currL->tamanhoCor);

    return finalDoArquivo(arquivoBin);
}

/**
 * Imprime os campos de uma linha conforme a formatação requisitada
 * @param currLinha linha a ser impressa
 */
void imprimeLinha(linha currLinha) {

    imprimirCampo("Codigo da linha",&currLinha.codLinha,1);
    imprimirCampo("Nome da linha",currLinha.nomeLinha,0);
    imprimirCampo("Cor que descreve a linha",currLinha.corLinha,0);
    imprimirCartao(currLinha.aceitaCartao);//o cartão recebe um tratamento diferente devido as excessões

    printf("\n");

}

/**
 * Salva uma nova linha em um arquivo binário na posição indicada pelo header
 * e atualiza quantidade de registros e próxima prosição de salvamento
 * @param arquivoBin arquivo onde o veiculo será salvo
 * @param currL veicuo que será salvo
 * @param header header do arquivo binário
 */
void salvaLinha(FILE* arquivoBin, linha* currL, linhaHeader* header) {
    fseek(arquivoBin, header->byteProxReg, SEEK_SET);

    fwrite(&currL->removido, sizeof(char), 1, arquivoBin);
    fwrite(&currL->tamanhoRegistro, sizeof(int), 1, arquivoBin);
    fwrite(&currL->codLinha, sizeof(int), 1, arquivoBin);
    fwrite(&currL->aceitaCartao, sizeof(char), 1, arquivoBin);

    fwrite(&currL->tamanhoNome, sizeof(int), 1, arquivoBin);
    fwrite(&currL->nomeLinha, sizeof(char), currL->tamanhoNome, arquivoBin);

    fwrite(&currL->tamanhoCor, sizeof(int), 1, arquivoBin);
    fwrite(&currL->corLinha, sizeof(char), currL->tamanhoCor,arquivoBin);

    header->byteProxReg = ftell(arquivoBin);
    header->nroRegRemovidos += (currL->removido == '0') ? 1 : 0;
    header->nroRegistros += (currL->removido == '0') ? 0 : 1;
}

/**
 * Lê o header do CSV e salva em uma variável
 * @param arquivoCSV arquivo de onde o header será lido
 * @param header header onde será salvo
 */
void lerHeaderCSV_Linha(FILE* arquivoCSV, linhaHeader* header) {
    lerString(arquivoCSV, header->descreveCodigo);
    lerString(arquivoCSV, header->descreveCartao);
    lerString(arquivoCSV, header->descreveNome);
    lerString(arquivoCSV, header->descreveLinha);
}

/**
 * Lê o header de um arquivo binário
 * @param arquivoBin arquivo de onde será lido
 * @param header variável onde o header será salvo
 */
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

/**
 * Salva o header em um arquivo binário
 * @param arquivoBin arquivo de onde será salvo
 * @param header header que será salvo
 */
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