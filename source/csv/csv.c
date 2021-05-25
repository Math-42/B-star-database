#include "csv.h"
#include <stdio.h>
#include "../utils/utils.h"

/**
 * Lê um campo CSV byte a byte
 * @param arquivo arquivo a ser lido
 * @param string string onde o conteúdo será salvo
 * @return tamanho do campo lido
 */
int lerByteAByte(FILE* arquivo, char* string){
	int posicao = -1;
    
	do {
        posicao++;
        fread(&string[posicao], sizeof(char), 1, arquivo);
    } while (string[posicao] != '\n' && string[posicao] != ',');
    
    return posicao;
}

/**
 * Testa se o valor lido é equivalente a NULO
 * @param string string a ser testada
 * @return 1 para válido e 0 para inválido
 */
int validarValorLido(char string[11]) {
    if (string[0] == 'N' && string[1] == 'U' && string[2] == 'L' &&
        string[3] == 'O') {
        return 0;
    }
    return 1;
}

/**
 * Lê um campo de string do csv e adiciona \0 no final
 * @param arquivo arquivo a ser lido
 * @param string local onde o conteudo será salvo
 * @return retorna o tamanho da string lida
 */
int lerString(FILE* arquivo, char* string) {
	int tamanho = lerByteAByte(arquivo,string);
    if(!validarValorLido(string))tamanho=0;
    string[tamanho] = '\0';
    return tamanho;
}

/**
 * Lê um campo de string do csv de tamanho fixo
 * @param arquivo arquivo a ser lido
 * @param string local onde o conteudo será salvo
 * @param tamanhoMaximo tamanho máximo aceito
 * @return retorna o tamanho real da string lida
 */
int lerStringFixa(FILE* arquivo, char* string,int tamanhoMaximo) {
    int tamanhoReal = lerString(arquivo,string);
    for(int i=tamanhoReal+1;i<tamanhoMaximo;i++)string[i]= '@';//completa de @ após o \0 caso necessário
    return tamanhoReal;
}

/**
 * Testa se um campo do CSV foi removido ou não
 * @param arquivo arquivo a ser analisado
 * @return retorna '0' para removido e '1' para não removido
 */
char foiRemovido(FILE* arquivo){
    char primeiroChar;
    fread(&primeiroChar, sizeof(char), 1, arquivo);
    primeiroChar = (primeiroChar == '*')? '0':'1';
    if(primeiroChar == '1')fseek(arquivo,-1,SEEK_CUR);
    return primeiroChar;
}

/**
 * Lê um campo de inteiro de um CSV
 * @param arquivo arquivo a ser analisado
 * @return retorna o próprio valor lido ou -1 caso seja nulo
 */
int lerInteiro(FILE* arquivo) {
    char string[11];  // tamanho do maior inteiro possivel

	int tamanho = lerByteAByte(arquivo,string);

    if (validarValorLido(string) == 0) return -1;

    return stringToInt(string,tamanho);
}
