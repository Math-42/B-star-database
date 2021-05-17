#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>

FILE* abrirCSV(char nomeDoArquivo[100]);
FILE* abrirBinario(char nomeDoArquivo[100]);
int lerString(FILE* arquivo, char* string);
void lerStringFixa_CSV(FILE* arquivo, char* string, int tamanho);
int lerInteiro(FILE* arquivo);

#endif