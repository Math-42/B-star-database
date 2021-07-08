#include "linha.h"
#include <stdio.h>
#include <stdlib.h>
#include "../binario/binario.h"
#include "../csv/csv.h"
#include "../utils/utils.h"
#include <string.h>

/**
 *  Valida o header de um arquivo
 * @param arquivo arquivo de onde o header se origina
 * @param header header a ser verificado
 * @param verificaConsistencia flag que indica para testar a consistencia do arquivo
 * @param verificaRegistros flag que indica para verificar se existem registros
 * @return retorna 1 caso o arquivo passe nos testes exigidos
 */
int validaHeader_linha(FILE** arquivo, linhaHeader header, int verificaConsistencia, int verificaRegistros){
    
    int correto = 1;

    if (verificaConsistencia>0 && header.status == '0') {
        printf("Falha no processamento do arquivo.");
        correto = 0;
    }

    if (verificaRegistros>0 && header.nroRegistros == 0) {
        printf("Registro inexistente.");
        correto = 0;;
    }
    if(!correto)fclose(*arquivo);
    return correto;
}


/**
 * Imprime o tipo de pagamento no cartão no formato solicitado
 * @param descricao descricao do campo
 * @param cartao string original no formato salvo
 */
void imprimirCartao(char* descricao, char* cartao){
    printf("%s: ", descricao);
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
 * Lê um registro de uma linha do arquivo CSV lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param arquivoCSV nome do arquivo csv fonte dos dados
 * @param novaLinha variavel para salvar os dados
 * @return retorna 1 caso for o ultimo registro e 0 caso contrário
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
 * @return retorna 1 caso for o ultimo registro e 0 caso contrário
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
 * Lê um registro de linha do terminal lidando com campos nulos e os tamanhos
 * de registro total e dos campos variaveis
 * @param currL variavel para salvar os dados lidos
 */
void lerLinha_Terminal(linha* currL) {

    currL->tamanhoRegistro = 0;
    currL->removido = '1';

    scanf("%d", &currL->codLinha);

    lerStringTerminalFixa(currL->aceitaCartao,1);

    currL->tamanhoNome = lerStringTerminal(currL->nomeLinha);
    currL->tamanhoCor = lerStringTerminal(currL->corLinha);

    currL->tamanhoRegistro += currL->tamanhoNome + currL->tamanhoCor;
    currL->tamanhoRegistro += 13;  // tamanho da parte fixa da struct
}

/**
 * Imprime os campos de uma linha conforme a formatação requisitada
 * @param currLinha linha a ser impressa
 * @param header header do arquivo
 */
void imprimeLinha(linha currLinha, linhaHeader header) {

    imprimirCampo(header.descreveCodigo,&currLinha.codLinha,1);
    imprimirCampo(header.descreveNome,currLinha.nomeLinha,0);
    imprimirCampo(header.descreveLinha,currLinha.corLinha,0);
    imprimirCartao(header.descreveCartao, currLinha.aceitaCartao);//o cartão recebe um tratamento diferente devido as excessões

    printf("\n");

}

/**
 * Salva uma nova linha em um arquivo binário na posição indicada pelo header
 * e atualiza quantidade de registros e próxima prosição de salvamento
 * @param arquivoBin arquivo onde a linha será lida
 * @param currL variavel onde a linha será salva
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
    lerStringBin(arquivoBin,(header->descreveCodigo), 15);
    lerStringBin(arquivoBin,(header->descreveCartao), 13);
    lerStringBin(arquivoBin,(header->descreveNome), 13);
    lerStringBin(arquivoBin,(header->descreveLinha), 24);

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

/**
 * Cria um arquivo binário na estrutura solicitada de header e campos a partir de um arquivo CSV
 * @param nomeArquivoCSV nome do arquivo csv fonte dos dados
 * @param nomeArquivoBin nome do arquivo binário onde os dados serão salvos
 */
void CreateTable_Linha(char nomeArquivoCSV[100], char nomeArquivoBin[100]) {
    FILE* arquivoBin;
    FILE* arquivoCSV;

    if(!abrirArquivo(&arquivoCSV,nomeArquivoCSV,"r",1))return;
    
    abrirArquivo(&arquivoBin,nomeArquivoBin,"wb",0);

    linhaHeader novoHeader;
    linha novaLinha;

    //valores iniciais do header
    novoHeader.status = '0';
    novoHeader.byteProxReg = 82;
    novoHeader.nroRegistros = 0;
    novoHeader.nroRegRemovidos = 0;
    
    lerHeaderCSV_Linha(arquivoCSV, &novoHeader);
    salvaHeader_Linha(arquivoBin, &novoHeader);

    int isFinalDoArquivo = finalDoArquivo(arquivoCSV);

    while (!isFinalDoArquivo) {
        isFinalDoArquivo = lerLinha_CSV(arquivoCSV, &novaLinha);
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

    FILE* arquivoBin;
    if(!abrirArquivo(&arquivoBin,nomeArquivoBin,"rb",1))return;

    linhaHeader novoHeader;
    linha novaLinha;
    
    lerHeaderBin_Linha(arquivoBin, &novoHeader);
    if(!validaHeader_linha(&arquivoBin,novoHeader,1,1))return;

    int isFinalDoArquivo = finalDoArquivo(arquivoBin);

    while (!isFinalDoArquivo) {
        isFinalDoArquivo = lerLinha_Bin(arquivoBin, &novaLinha);
        if (novaLinha.removido == '1') imprimeLinha(novaLinha, novoHeader);
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
void SelectFromWhere_Linha(char nomeArquivoBin[100], char* campo, char*valor){
    FILE* arquivoBin;
    if(!abrirArquivo(&arquivoBin,nomeArquivoBin,"rb",1))return;

    linhaHeader header;

    lerHeaderBin_Linha(arquivoBin, &header);
    if(!validaHeader_linha(&arquivoBin,header,1,1))return;    

    int headerPos;                      // posição do campo no cabeçalho
    if (strcmp(campo, "codLinha") == 0)  // codLinha (int)
        headerPos = 0;
    else if (strcmp(campo, "aceitaCartao") == 0)  // aceitaCartao (string)
        headerPos = 1;
    else if (strcmp(campo, "nomeLinha") == 0)  // nomeLinha (string)
        headerPos = 2;
    else if (strcmp(campo, "corLinha") == 0)  // corLinha (string)
        headerPos = 3;

    int total = header.nroRegistros + header.nroRegRemovidos;  // numero total de registros de dados
    int existePeloMenosUm = 0;
    
    fseek(arquivoBin, 82, 0);  // posiciono para o primeiro registro de dados do binario

    linha linhaTemp; // crio a cada iteração uma linha atribuindo a ela os
                    // valores lido em cada registro do binario

    while (total--) {  // percorro todos registros de dados
        lerLinha_Bin(arquivoBin, &linhaTemp);
        int existe = 0;
        if (linhaTemp.removido == '0') continue;  // linha ja removida
        
        switch (headerPos) {
            case 0:
                if (linhaTemp.codLinha == stringToInt(valor, (int)strlen(valor))) {
                    imprimeLinha(linhaTemp, header);
                    fclose(arquivoBin);
                    return;  
                    //como o codLinha é unico pode interromper assim que encontrar o primeiro
                }
                break;
            case 1: 
                if (strcmp(valor, linhaTemp.aceitaCartao) == 0) existe = 1;
                break;
            case 2:
                if (strcmp(valor, linhaTemp.nomeLinha) == 0) existe = 1;
                break;
            case 3:
                if (strcmp(valor, linhaTemp.corLinha) == 0) existe = 1;
                break;
            default:
                break;
        }

        if (existe) {   // dado encontrado 
            imprimeLinha(linhaTemp, header);
            existePeloMenosUm = 1;
        }
    }

    if (!existePeloMenosUm)printf("Registro inexistente.\n");  // nenhum registro encontrado

    fclose(arquivoBin);
}

/**
 *  Efetua as leituras correspondentes usando o string_quote
 *  trata os espaços com lixo nas string fixas e salva os dados do novo veículo
 *  no fim do binário
 * @param nomeArquivoBIn nome do arquivo binário onde os valores serão salvos
 */
void InsertInto_Linha(char nomeArquivoBin[100], int numeroDeEntradas){
    FILE* arquivoBin;
    if(!abrirArquivo(&arquivoBin,nomeArquivoBin,"rb+",1))return;
    linha novaLinha;
    linhaHeader header;

    if (arquivoBin == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }
    
    lerHeaderBin_Linha(arquivoBin, &header);
    if(!validaHeader_linha(&arquivoBin,header,1,0))return;

    header.status = '0';
    salvaHeader_Linha(arquivoBin, &header);

    while (numeroDeEntradas--){  
        lerLinha_Terminal(&novaLinha);
        salvaLinha(arquivoBin, &novaLinha, &header);  // salvo o novo veículo no fim do binário
    }

    header.status = '1';
    salvaHeader_Linha(arquivoBin, &header);
    fclose(arquivoBin);
    binarioNaTela(nomeArquivoBin);
}