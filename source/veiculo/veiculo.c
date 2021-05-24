#include "veiculo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../binario/binario.h"
#include "../csv/csv.h"
#include "../utils/utils.h"

char MESES[][12] = {"janeiro",  "fevereiro", "março",    "abril",
                    "maio",     "junho",     "julho",    "agosto",
                    "setembro", "outubro",   "novembro", "dezembro"}; // todos os meses


/**
 * Imprime a data no formato solicitado
 * @param stringData string original no formato salvo
 */
void imprimeData(char* stringData) {
    printf("Data de entrada do veiculo na frota: ");
    if (stringData[0] != '\0') {//testa se a data é nula
        int indiceDoMes =
            (stringData[5] - '0') * 10 + (stringData[6] - '0') -1;  // calcula o indice do mes e translada para entre 0-11
        printf("%.2s de %s de %.4s\n", stringData + 8, MESES[indiceDoMes],stringData);
    } else {
        printf("campo com valor nulo\n");
    }
}

/**
 * Cria um arquivo binário na estrutura solicitada de header e campos a partir de um arquivo CSV
 * @param nomeArquivoCSV nome do arquivo csv fonte dos dados
 * @param nomeArquivoBIn nome do arquivo binário onde os dados serão salvos
 */
void CreateTable_Veiculo(char nomeArquivoCSV[100], char nomeArquivoBin[100]) {
    FILE* arquivoBin = fopen(nomeArquivoBin, "wb");
    FILE* arquivoCSV = fopen(nomeArquivoCSV, "r");

    if (arquivoCSV == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    veiculoHeader novoHeader;
    veiculo novoVeiculo;
    int finalDoArquivo = 0;

    //definindo valores iniciais do header
    novoHeader.status = '0';
    novoHeader.byteProxReg = 175;
    novoHeader.nroRegistros = 0;
    novoHeader.nroRegRemovidos = 0;

    lerHeaderCSV_Veiculo(arquivoCSV, &novoHeader);
    salvaHeader_Veiculo(arquivoBin, &novoHeader);

    //percorre o arquivo até o final
    while (!finalDoArquivo) {
        finalDoArquivo = lerVeiculo_CSV(arquivoCSV, &novoVeiculo);
        salvaVeiculo(arquivoBin, &novoVeiculo, &novoHeader);
    }

    novoHeader.status = '1';

    salvaHeader_Veiculo(arquivoBin, &novoHeader);//finaliza e salva o header

    //fecha todos arquivos abertos
    fclose(arquivoBin);
    fclose(arquivoCSV);

    binarioNaTela(nomeArquivoBin);
}

/**
 * Imprime todos os valores de um binário
 * @param nomeArquivoBIn nome do arquivo binário de onde os dados serão lidos
 */
void SelectFrom_Veiculo(char nomeArquivoBin[100]) {
    FILE* arquivoBin = fopen(nomeArquivoBin, "rb");
    veiculoHeader novoHeader;
    veiculo novoVeiculo;

    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    lerHeaderBin_Veiculo(arquivoBin, &novoHeader);

    if (novoHeader.status == 0) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    if (novoHeader.nroRegistros == 0) {
        printf("Registro inexistente.");
        return;
    }

    novoHeader.status = '0';
    salvaHeader_Veiculo(arquivoBin, &novoHeader);
    int finalDoArquivo = 0;
    //percorre todo o arquivo imprimindo apenas os registros salvos
    while (!finalDoArquivo) {
        finalDoArquivo = lerVeiculo_Bin(arquivoBin, &novoVeiculo);
        if (novoVeiculo.removido == '1') imprimeVeiculo(novoVeiculo);
    }
    novoHeader.status = '1';
    salvaHeader_Veiculo(arquivoBin, &novoHeader);

    fclose(arquivoBin);
}

/**
 * A busca por um campo em específico é feita pela a
 * varredura da posição do campo correspondente a ser buscado no cabeçalho, após
 * isso percorre os registros todos os dados nessa respectiva posição e então
 * compara com o valor procurado
 * @param nomeArquivoBIn nome do arquivo binário de onde os dados serão lidos
 * @param campo nome do campo onde fara a busca
 * @param valor valor que está sendo buscado
 */
void SelectFromWhere_Veiculo(char nomeArquivoBin[100], char* campo, char* valor) {
    FILE* arquivoBin = fopen(nomeArquivoBin, "rb");
    veiculoHeader header;
    veiculo novoVeiculo;

    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    lerHeaderBin_Veiculo(arquivoBin, &header);

    if (header.status == 0) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    if (header.nroRegistros == 0) {
        printf("Registro inexistente.");
        return;
    }

    long int currPos = ftell(arquivoBin);

    int headerPos;                      // posição do campo no cabeçalho
    if (strcmp(campo, "prefixo") == 0)  // prefixo (string)
        headerPos = 0;
    else if (strcmp(campo, "data") == 0)  // data (string)
        headerPos = 1;
    else if (strcmp(campo, "quantidadeLugares") == 0)  // qt_lugares (int)
        headerPos = 2;
    else if (strcmp(campo, "codLinha") == 0)  // cod_linha (int)
        headerPos = 3;
    else if (strcmp(campo, "modelo") == 0)  // modelo (string)
        headerPos = 4;
    else if (strcmp(campo, "categoria") == 0)  // categoria (string)
        headerPos = 5;

    int total = header.nroRegistros;  // numero total de registros de dados
    int existePeloMenosUm = 0;

    fseek(arquivoBin, 175,
          0);  // posiciono para o primeiro registro de dados do binario

    veiculo veiculoTemp;  // crio a cada iteração um veiculo atribuindo a ele os
                          // valores lido em cada registro do binario

    while (total--) {  // percorro todos registros de dados
        lerVeiculo_Bin(arquivoBin, &veiculoTemp);
        int existe = 0;
        if (veiculoTemp.removido == '0')  // veiculo ja removido
            continue;

        switch (headerPos) {
            case 0:
                if (strcmp(valor, veiculoTemp.prefixo) == 0) {
                    fseek(arquivoBin, currPos, 0);
                    imprimeVeiculo(veiculoTemp);
                    return;  
                    //como o prefixo é unico pode interromper assim que encontrar o primeiro
                }
                break;
            case 1: 
                if (strcmp(valor, veiculoTemp.data) == 0) existe = 1;
                break;
            case 2:
                if (veiculoTemp.quantidadeLugares == stringToInt(valor, (int)strlen(valor))) existe = 1;
                break;
            case 3:
                if (veiculoTemp.codLinha == stringToInt(valor, (int)strlen(valor))) existe = 1;
                break;
            case 4:
                if (strcmp(valor, veiculoTemp.modelo) == 0) existe = 1;
                break;
            case 5:
                if (strcmp(valor, veiculoTemp.categoria) == 0) existe = 1;
                break;
            default:
                break;
        }

        if (existe) {
            imprimeVeiculo(veiculoTemp);
            existePeloMenosUm = 1;
        }
    }

    if (!existePeloMenosUm)printf("Registro inexistente.\n");  // nenhum registro encontrado

    fseek(arquivoBin, currPos, 0);
    fclose(arquivoBin);
}

/**
 *  Efetua as leituras correspondentes usando o string_quote
 *  trata os espaços com lixo nas string fixas e salva os dados do novo veículo
 *  no fim do binário
 * @param nomeArquivoBIn nome do arquivo binário onde os valores serão salvos
 * @param quantidadeInputs quantidade de valores que serão salvos
 */
void InsertInto_Veiculo(char nomeArquivoBin[100], int quantidadeInputs) {
    FILE* arquivoBin = fopen(nomeArquivoBin, "wb");
    veiculo novoVeiculo;
    veiculoHeader header;

    char string[100];
    int tmp;

    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }
    
    lerHeaderBin_Veiculo(arquivoBin, &header);

    if (header.status == 0) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    if (header.nroRegistros == 0) {
        printf("Registro inexistente.");
        return;
    }

    novoVeiculo.tamanhoRegistro = 0;
    novoVeiculo.removido = '1';

    for (int i = 0; i < 6; i++)  // trata o lixo na string de tam fixo
        string[i] = '@';
    scan_quote_string(string);
    string[(int)strlen(string)] = '\0';
    novoVeiculo.tamanhoRegistro += (int)strlen(string);
    strcpy(novoVeiculo.prefixo,
           string);  // copio a string obtida em seu campo respectivo do veiculo

    for (int i = 0; i < 11; i++) string[i] = '@';
    scan_quote_string(string);
    string[(int)strlen(string)] = '\0';
    novoVeiculo.tamanhoRegistro += (int)strlen(string);
    strcpy(novoVeiculo.data, string);

    scanf("%d", &tmp);
    novoVeiculo.quantidadeLugares = tmp;
    novoVeiculo.tamanhoRegistro += 4;

    scanf("%d", &tmp);
    novoVeiculo.codLinha = tmp;
    novoVeiculo.tamanhoRegistro += 4;

    scan_quote_string(string);
    string[(int)strlen(string)] = '\0';
    novoVeiculo.tamanhoModelo += (int)strlen(string);
    novoVeiculo.tamanhoRegistro += 4 + (int)strlen(string);
    strcpy(novoVeiculo.modelo, string);

    scan_quote_string(string);
    string[(int)strlen(string)] = '\0';
    novoVeiculo.tamanhoCategoria += (int)strlen(string);
    novoVeiculo.tamanhoRegistro += 4 + (int)strlen(string);
    strcpy(novoVeiculo.categoria, string);

    salvaVeiculo(arquivoBin, &novoVeiculo,&header);  // salvo o novo veículo no fim do binário
}

/**
 * Lê um registro de veiculo do arquivo CSV lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param arquivoCSV nome do arquivo csv fonte dos dados
 * @param novoVeiculo variavel para salvar os dados
 * @return retorna 1 caso for o utimo registro e 0 caso contrário
 */
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

/**
 * Lê um registro de veiculo do arquivo binário lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param arquivoBin nome do arquivo binário fonte dos dados
 * @param currV variavel para salvar os dados
 * @return retorna 1 caso for o utimo registro e 0 caso contrário
 */
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

/**
 * Imprime os campos de um veiculo conforme a formatação requisitada
 * @param currV veiculo a ser impresso
 */
void imprimeVeiculo(veiculo currVeiculo) {
    imprimirCampo("Prefixo do veiculo", currVeiculo.prefixo, 0);
    imprimirCampo("Modelo do veiculo", currVeiculo.modelo, 0);
    imprimirCampo("Categoria do veiculo", currVeiculo.categoria, 0);
    imprimeData(currVeiculo.data);//impressão diferente devido ao formato
    imprimirCampo("Quantidade de lugares sentados disponiveis",&currVeiculo.quantidadeLugares, 1);
    printf("\n");
}

/**
 * Salva um novo veiculo em um arquivo binário na posição indicada pelo header
 * e atualiza quantidade de registros e próxima prosição de salvamento
 * @param arquivoBin arquivo onde o veiculo será salvo
 * @param currV veicuo que será salvo
 * @param header header do arquivo binário
 */
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
    fwrite(&currV->categoria, sizeof(char), currV->tamanhoCategoria,arquivoBin);

    header->byteProxReg = ftell(arquivoBin);
    header->nroRegRemovidos += (currV->removido == '0') ? 1 : 0;
    header->nroRegistros += (currV->removido == '0') ? 0 : 1;
}

/**
 * Lê o header do CSV e salva no header do binário
 * @param arquivoCSV arquivo de onde o header será lido
 * @param header header onde será salvo
 */
void lerHeaderCSV_Veiculo(FILE* arquivoCSV, veiculoHeader* header) {
    lerString(arquivoCSV, header->descrevePrefixo);
    lerString(arquivoCSV, header->descreveData);
    lerString(arquivoCSV, header->descreveLugares);
    lerString(arquivoCSV, header->descreveLinha);
    lerString(arquivoCSV, header->descreveModelo);
    lerString(arquivoCSV, header->descreveCategoria);
}

/**
 * Lê o header de um arquivo binário
 * @param arquivoBin arquivo de onde será lido
 * @param header variável onde o header será salvo
 */
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

/**
 * Salva o header em um arquivo binário
 * @param arquivoBin arquivo de onde será salvo
 * @param header header que será salvo
 */
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
