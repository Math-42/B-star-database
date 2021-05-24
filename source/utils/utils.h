#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>

int finalDoArquivo(FILE* arquivo);
void imprimirCampo(char* descricao, void* valor, int isInt);
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif