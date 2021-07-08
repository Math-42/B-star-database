#include "veiculo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../arvore/arvore.h"
#include "../binario/binario.h"
#include "../csv/csv.h"
#include "../utils/utils.h"

char MESES[][12] = {"janeiro", "fevereiro", "março", "abril",
                    "maio", "junho", "julho", "agosto",
                    "setembro", "outubro", "novembro", "dezembro"};  // todos os meses

/**
 *  Valida o header de um arquivo
 * @param arquivo arquivo de onde o header se origina
 * @param header header a ser verificado
 * @param verificaConsistencia flag que indica para testar a consistencia do arquivo
 * @param verificaRegistros flag que indica para verificar se existem registros
 * @return retorna 1 caso o arquivo passe nos testes exigidos
 */
int validaHeader_veiculo(FILE** arquivo, veiculoHeader header, int verificaConsistencia, int verificaRegistros) {
    int correto = 1;

    if (verificaConsistencia > 0 && header.status == '0') {
        printf("Falha no processamento do arquivo.");
        correto = 0;
    }

    if (verificaRegistros > 0 && header.nroRegistros == 0) {
        printf("Registro inexistente.");
        correto = 0;
        ;
    }
    if (!correto) fclose(*arquivo);
    return correto;
}

/**
 * Lê um registro de veiculo do arquivo CSV lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param arquivoCSV nome do arquivo csv fonte dos dados
 * @param novoVeiculo variavel para salvar os dados
 * @return retorna 1 caso for o ultimo registro e 0 caso contrário
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
 * @param pos indica se deve ler o próximo registro (-1) ou algum em especifico
 * @return retorna 1 caso for o ultimo registro e 0 caso contrário
 */
int lerVeiculo_Bin(FILE* arquivoBin, veiculo* currV, long int pos) {
    if (pos != -1) fseek(arquivoBin, pos, 0);
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
 * Lê uma  entrada de registro de veiculo do terminal lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param currV variavel para salvar os dados
 */
void lerVeiculo_Terminal(veiculo* currV) {
    currV->tamanhoRegistro = 0;
    currV->removido = '1';

    lerStringTerminalFixa(currV->prefixo, 5);
    lerStringTerminalFixa(currV->data, 10);

    scanf("%d", &currV->quantidadeLugares);
    scanf("%d", &currV->codLinha);

    currV->tamanhoModelo = lerStringTerminal(currV->modelo);
    currV->tamanhoCategoria = lerStringTerminal(currV->categoria);

    currV->tamanhoRegistro = currV->tamanhoModelo + currV->tamanhoCategoria;
    currV->tamanhoRegistro += 31;  // tamanho da parte fixa da struct
}

/**
 * Imprime a data no formato solicitado
 * @param descricao descricao do campo
 * @param stringData string original no formato salvo
 */
void imprimeData(char* descricao, char* stringData) {
    printf("%s: ", descricao);
    if (stringData[0] != '\0') {  //testa se a data é nula
        int indiceDoMes =
            (stringData[5] - '0') * 10 + (stringData[6] - '0') - 1;  // calcula o indice do mes e translada para entre 0-11
        printf("%.2s de %s de %.4s\n", stringData + 8, MESES[indiceDoMes], stringData);
    } else {
        printf("campo com valor nulo\n");
    }
}

/**
 * Imprime os campos de um veiculo conforme a formatação requisitada
 * @param currVeiculo veiculo a ser impresso
 * @param header header do arquivo
 */
void imprimeVeiculo(veiculo currVeiculo, veiculoHeader header) {
    imprimirCampo(header.descrevePrefixo, currVeiculo.prefixo, 0);
    imprimirCampo(header.descreveModelo, currVeiculo.modelo, 0);
    imprimirCampo(header.descreveCategoria, currVeiculo.categoria, 0);
    imprimeData(header.descreveData, currVeiculo.data);  //impressão diferente devido ao formato
    imprimirCampo(header.descreveLugares, &currVeiculo.quantidadeLugares, 1);
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
    fwrite(&currV->categoria, sizeof(char), currV->tamanhoCategoria, arquivoBin);

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
    lerStringBin(arquivoBin, header->descrevePrefixo, 18);
    lerStringBin(arquivoBin, header->descreveData, 35);
    lerStringBin(arquivoBin, header->descreveLugares, 42);
    lerStringBin(arquivoBin, header->descreveLinha, 26);
    lerStringBin(arquivoBin, header->descreveModelo, 17);
    lerStringBin(arquivoBin, header->descreveCategoria, 20);
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

/**
 * Cria um arquivo binário na estrutura solicitada de header e campos a partir de um arquivo CSV
 * @param nomeArquivoCSV nome do arquivo csv fonte dos dados
 * @param nomeArquivoBIn nome do arquivo binário onde os dados serão salvos
 */
void CreateTable_Veiculo(char nomeArquivoCSV[100], char nomeArquivoBin[100]) {
    FILE* arquivoBin;
    FILE* arquivoCSV;

    if (!abrirArquivo(&arquivoCSV, nomeArquivoCSV, "r", 1)) return;

    abrirArquivo(&arquivoBin, nomeArquivoBin, "wb", 0);

    veiculoHeader novoHeader;
    veiculo novoVeiculo;

    //definindo valores iniciais do header
    novoHeader.status = '0';
    novoHeader.byteProxReg = 175;
    novoHeader.nroRegistros = 0;
    novoHeader.nroRegRemovidos = 0;

    lerHeaderCSV_Veiculo(arquivoCSV, &novoHeader);
    salvaHeader_Veiculo(arquivoBin, &novoHeader);

    int isFinalDoArquivo = finalDoArquivo(arquivoCSV);
    //percorre o arquivo até o final
    while (!isFinalDoArquivo) {
        isFinalDoArquivo = lerVeiculo_CSV(arquivoCSV, &novoVeiculo);
        salvaVeiculo(arquivoBin, &novoVeiculo, &novoHeader);
    }

    novoHeader.status = '1';

    salvaHeader_Veiculo(arquivoBin, &novoHeader);  //finaliza e salva o header

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
    FILE* arquivoBin;
    if (!abrirArquivo(&arquivoBin, nomeArquivoBin, "rb", 1)) return;

    veiculoHeader novoHeader;
    veiculo novoVeiculo;

    lerHeaderBin_Veiculo(arquivoBin, &novoHeader);
    if (!validaHeader_veiculo(&arquivoBin, novoHeader, 1, 1)) return;

    int isFinalDoArquivo = finalDoArquivo(arquivoBin);
    //percorre todo o arquivo imprimindo apenas os registros salvos
    while (!isFinalDoArquivo) {
        isFinalDoArquivo = lerVeiculo_Bin(arquivoBin, &novoVeiculo, -1);
        if (novoVeiculo.removido == '1') imprimeVeiculo(novoVeiculo, novoHeader);
    }

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
    FILE* arquivoBin;
    if (!abrirArquivo(&arquivoBin, nomeArquivoBin, "rb", 1)) return;

    veiculoHeader header;

    lerHeaderBin_Veiculo(arquivoBin, &header);
    if (!validaHeader_veiculo(&arquivoBin, header, 1, 1)) return;

    int headerPos;  // posição do campo no cabeçalho
    if (strcmp(campo, "prefixo") == 0)
        headerPos = 0;  // prefixo (string)
    else if (strcmp(campo, "data") == 0)
        headerPos = 1;  // data (string)
    else if (strcmp(campo, "quantidadeLugares") == 0)
        headerPos = 2;  // qt_lugares (int)
    else if (strcmp(campo, "codLinha") == 0)
        headerPos = 3;  // cod_linha (int)
    else if (strcmp(campo, "modelo") == 0)
        headerPos = 4;  // modelo (string)
    else if (strcmp(campo, "categoria") == 0)
        headerPos = 5;  // categoria (string)

    int total = header.nroRegistros + header.nroRegRemovidos;  // numero total de registros de dados
    int existePeloMenosUm = 0;

    fseek(arquivoBin, 175, 0);  // posiciono para o primeiro registro de dados do binario

    veiculo veiculoTemp;  // crio a cada iteração um veiculo atribuindo a ele os
                          // valores lido em cada registro do binario

    while (total--) {  // percorro todos registros de dados
        lerVeiculo_Bin(arquivoBin, &veiculoTemp, -1);
        int existe = 0;
        if (veiculoTemp.removido == '0') continue;  // veiculo ja removido

        switch (headerPos) {
            case 0:
                if (strcmp(valor, veiculoTemp.prefixo) == 0) {
                    imprimeVeiculo(veiculoTemp, header);
                    fclose(arquivoBin);
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

        if (existe) {  // dado encontrado
            imprimeVeiculo(veiculoTemp, header);
            existePeloMenosUm = 1;
        }
    }

    if (!existePeloMenosUm) printf("Registro inexistente.\n");  // nenhum registro encontrado

    fclose(arquivoBin);
}

/**
 *  Efetua as leituras correspondentes usando o string_quote
 *  trata os espaços com lixo nas string fixas e salva os dados do novo veículo
 *  no fim do binário
 * @param nomeArquivoBIn nome do arquivo binário onde os valores serão salvos
 */
void InsertInto_Veiculo(char nomeArquivoBin[100], int numeroDeEntradas) {
    FILE* arquivoBin;
    if (!abrirArquivo(&arquivoBin, nomeArquivoBin, "rb+", 1)) return;

    veiculo novoVeiculo;
    veiculoHeader header;

    lerHeaderBin_Veiculo(arquivoBin, &header);
    if (!validaHeader_veiculo(&arquivoBin, header, 1, 0)) return;

    header.status = '0';
    salvaHeader_Veiculo(arquivoBin, &header);

    while (numeroDeEntradas--) {
        lerVeiculo_Terminal(&novoVeiculo);
        salvaVeiculo(arquivoBin, &novoVeiculo, &header);  // salvo o novo veículo no fim do binário
    }

    header.status = '1';
    salvaHeader_Veiculo(arquivoBin, &header);
    fclose(arquivoBin);
    binarioNaTela(nomeArquivoBin);
}

void CreateIndex_Veiculo(char nomeArquivoBinRegistros[100], char nomeArquivoBinIndex[100]) {
    FILE* arquivoBinRegistros;
    if (!abrirArquivo(&arquivoBinRegistros, nomeArquivoBinRegistros, "rb", 1)) return;

    veiculoHeader novoHeader;
    veiculo novoVeiculo;

    lerHeaderBin_Veiculo(arquivoBinRegistros, &novoHeader);
    if (!validaHeader_veiculo(&arquivoBinRegistros, novoHeader, 1, 1)) return;

    arvore* novaArvore = criaArvore(nomeArquivoBinIndex);

    int isFinalDoArquivo = finalDoArquivo(arquivoBinRegistros);
    //percorre todo o arquivo salvando apenas os registros salvos

    while (!isFinalDoArquivo) {
        registro novoRegistro;

        novoRegistro.P_ant = -1;
        novoRegistro.P_prox = -1;
        novoRegistro.Pr = ftell(arquivoBinRegistros);

        isFinalDoArquivo = lerVeiculo_Bin(arquivoBinRegistros, &novoVeiculo, -1);
        novoRegistro.C = convertePrefixo(novoVeiculo.prefixo);

        if (novoVeiculo.removido == '1') insereRegistro(novaArvore, novoRegistro);
    }

    fclose(arquivoBinRegistros);
    finalizaArvore(novaArvore);
    binarioNaTela(nomeArquivoBinIndex);
}

void SelectFromWithIndex_Veiculo(char nomeArquivoBinRegistros[100], char nomeArquivoBinIndex[100], char valorBuscado[6]) {
    FILE* arquivoBinRegistros;
    if (!abrirArquivo(&arquivoBinRegistros, nomeArquivoBinRegistros, "rb", 1)) return;

    veiculoHeader novoHeader;
    veiculo novoVeiculo;

    lerHeaderBin_Veiculo(arquivoBinRegistros, &novoHeader);
    if (!validaHeader_veiculo(&arquivoBinRegistros, novoHeader, 1, 1)) return;

    arvore* novaArvore = carregaArvore(nomeArquivoBinIndex);
    int isFinalDoArquivo = finalDoArquivo(arquivoBinRegistros);

    int chave = convertePrefixo(valorBuscado);
    int byteOffset = buscaRegistro(novaArvore, chave);
    if (byteOffset != -1) {
        lerVeiculo_Bin(arquivoBinRegistros, &novoVeiculo, byteOffset);
        imprimeVeiculo(novoVeiculo, novoHeader);
    } else {
        printf("Registro inexistente.");
    }

    fclose(arquivoBinRegistros);
    finalizaArvore(novaArvore);
}
