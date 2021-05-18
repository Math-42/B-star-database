#include "binario.h"
#include <stdio.h>

FILE* abrirBinario(char nomeDoArquivo[100]) {
    FILE* arquivo = fopen(nomeDoArquivo, "rb+");
    return arquivo;
}

int lerStringBin(FILE* arquivo, char* string,int tamanho) {
	fread(string,sizeof(char),tamanho,arquivo);
	string[tamanho] = '\0';
    return tamanho;
}

int lerInteiroBin(FILE* arquivo) {
    int inteiroLido;  // tamanho do maior inteiro possivel
	fread(&inteiroLido,sizeof(int),1,arquivo);
    return inteiroLido;
}
