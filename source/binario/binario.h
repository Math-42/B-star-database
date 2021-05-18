#ifndef _BIN_H_
#define _BIN_H_
#include <stdio.h>

FILE* abrirBinario(char nomeDoArquivo[100]);
int lerInteiroBin(FILE* arquivo);
int lerStringBin(FILE* arquivo, char* string,int tamanho);

#endif