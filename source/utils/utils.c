#include "utils.h"

#include <stdio.h>

FILE* abrirBinario(char nomeDoArquivo[100]) {
    FILE* arquivo = fopen(nomeDoArquivo, "wb+");
    return arquivo;
}

void lerStringFixa_CSV(FILE* arquivo, char* string, int tamanho) {
    fread(string, sizeof(char), tamanho, arquivo);
    fseek(arquivo, 1, SEEK_CUR);  // Pula a virgula
}
