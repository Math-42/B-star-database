#include "csv.h"
#include <stdio.h>

FILE* abrirCSV(char nomeDoArquivo[100]) {
    FILE* arquivo = fopen(nomeDoArquivo, "r");
    return arquivo;
}

int stringToInt(char string[11], int tamanho){
	int valorLido = 0;
	for (int i = 0; i < tamanho; i++) {
        valorLido = valorLido * 10 + (string[i] - '0');
    }
	return valorLido;
}

int lerByteAByte(FILE* arquivo, char* string){
	int posicao = -1;
    
	do {
        posicao++;
        fread(&string[posicao], sizeof(char), 1, arquivo);
    } while (string[posicao] != '\n' && string[posicao] != ',');
    
    return posicao;
}

int validarValorLido(char string[11]) {
    if (string[0] == 'N' && string[1] == 'U' && string[2] == 'L' &&
        string[3] == 'O') {
        return 0;
    }
    return 1;
}

int lerString(FILE* arquivo, char* string) {
	int tamanho = lerByteAByte(arquivo,string);
    if(!validarValorLido(string))tamanho=0;
    string[tamanho] = '\0';
    return tamanho;
}

int lerStringFixa(FILE* arquivo, char* string,int tamanhoMaximo) {
    int tamanhoReal = lerString(arquivo,string);
    for(int i=tamanhoReal+1;i<tamanhoMaximo;i++)string[i]= '@';//completa de @ após o \0
    return tamanhoReal;
}

char foiRemovido(FILE* arquivo){
    char primeiroChar;
    fread(&primeiroChar, sizeof(char), 1, arquivo);
    primeiroChar = (primeiroChar == '*')? '0':'1';
    if(primeiroChar == '1')fseek(arquivo,-1,SEEK_CUR);
    return primeiroChar;
}

int lerInteiro(FILE* arquivo) {
    char string[11];  // tamanho do maior inteiro possivel

	int tamanho = lerByteAByte(arquivo,string);

    if (validarValorLido(string) == 0) return -1;

    return stringToInt(string,tamanho);
}
