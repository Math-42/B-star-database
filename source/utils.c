FILE* abrirCSV(char nomeDoArquivo[100]) {
    FILE* arquivo = fopen(nomeDoArquivo, "r");
    return arquivo;
}

FILE* abrirBinario(char nomeDoArquivo[100]) {
    FILE* arquivo = fopen(nomeDoArquivo, "wb+");
    return arquivo;
}
int validarInteiroLido(char string[11]) {
    if (string[0] == 'N' && string[1] == 'U' && string[2] == 'L' &&
        string[3] == 'O') {
        return 0;
    }
    return 1;
}

int lerInteiro(FILE* arquivo) {
    int posicao = -1;
    char string[11];  // maior inteiro possivel
	int valorLido = 0;

    do {
        posicao++;
        fread(&string[posicao], sizeof(char), 1, arquivo);
    } while (string[posicao] != '\n' && string[posicao] != ',');

    if (validarInteiroLido(string)==0) return -1;

    for (int i = 0; i < posicao; i++) {
		valorLido = valorLido * 10 + (string[i]-'0');
    }

    return valorLido;
}
