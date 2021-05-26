#include "binario.h"
#include <stdio.h>

/**
 * Lê uma string de um arquivo binário
 * @param arquivo arquivo de onde será lida
 * @param string variável onde a string será salva
 * @param tamanho tamanho da string a ser lida
 * @return retorna o tamanho da string
 */
int lerStringBin(FILE* arquivo, char* string,int tamanho) {
	fread(string,sizeof(char),tamanho,arquivo);
	string[tamanho] = '\0';
    return tamanho;
}

/**
 * Lê um inteiro de um arquivo binário
 * @param arquivo arquivo de onde será lido
 * @return retorna o inteiro lido
 */
int lerInteiroBin(FILE* arquivo) {
    int inteiroLido;
	fread(&inteiroLido,sizeof(int),1,arquivo);
    return inteiroLido;
}
