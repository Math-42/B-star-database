#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>

int abrirArquivo(FILE** arquivo,char* nomeDoArquivo, char* tipoDeAbertura, int verificaExistencia);
int finalDoArquivo(FILE* arquivo);
void imprimirCampo(char* descricao, void* valor, int isInt);
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
int stringToInt(char string[11], int tamanho);
int lerStringTerminalFixa(char* string,int tamanhoMaximo);
int lerStringTerminal(char string[100]);

#endif