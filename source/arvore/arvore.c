arvore* criaArvore(char nomeArquivoIndice[]) {
    arvore* novaArvore = (arvore*)malloc(sizeof(arvore));

    novaArvore->header.status = '1';
    novaArvore->header.noRaiz = -1;
    novaArvore->header.RRNproxNo = 0;
    novaArvore->driver = 1;

    for (int i = 0; i < 68; i++) novaArvore->header.lixo[i] = '@';

    novaArvore->arquivoIndice = fopen(nomeArquivoIndice, "w+");
    if (novaArvore->arquivoIndice == NULL) printf("Arquivo nulo \n");
    salvaHeaderArvore(novaArvore);

    return novaArvore;
}

void finalizaArvore(arvore* currArvore) {
    currArvore->header.status = '1';
    salvaHeaderArvore(currArvore);

    fclose(currArvore->arquivoIndice);
    free(currArvore);
}

void salvaHeaderArvore(arvore* currArvore) {
    if (currArvore->arquivoIndice == NULL) printf("null");
    fseek(currArvore->arquivoIndice, 0, 0);

    fwrite(&currArvore->header.status, sizeof(char), 1, currArvore->arquivoIndice);
    fwrite(&currArvore->header.noRaiz, sizeof(int), 1, currArvore->arquivoIndice);
    fwrite(&currArvore->header.RRNproxNo, sizeof(int), 1, currArvore->arquivoIndice);
    fwrite(&currArvore->header.lixo, sizeof(char), 68, currArvore->arquivoIndice);
}

void leHeaderArvore(FILE* arquivoIndice, arvoreHeader* header) {
    fseek(arquivoIndice, 0, 0);

    fread(&header->status, sizeof(char), 1, arquivoIndice);
    fread(&header->noRaiz, sizeof(int), 1, arquivoIndice);
    fread(&header->RRNproxNo, sizeof(int), 1, arquivoIndice);
    fread(&header->lixo, sizeof(char), 68, arquivoIndice);
}
