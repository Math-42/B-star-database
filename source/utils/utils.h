#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>


FILE* abrirBinario(char nomeDoArquivo[100]);
void lerStringFixa_CSV(FILE* arquivo, char* string, int tamanho);


#endif