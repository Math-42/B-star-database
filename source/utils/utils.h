#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>

void lerStringFixa_CSV(FILE* arquivo, char* string, int tamanho);
int finalDoArquivo(FILE* arquivo);
void imprimirCampo(char* descricao, void* valor, int isInt);
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif