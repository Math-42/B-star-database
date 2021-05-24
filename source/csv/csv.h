#ifndef _CSV_H_
#define _CSV_H_
#include <stdio.h>

int lerString(FILE* arquivo, char* string);
int lerInteiro(FILE* arquivo);
int lerStringFixa(FILE* arquivo, char* string,int tamanho);
char foiRemovido(FILE* arquivo);

#endif