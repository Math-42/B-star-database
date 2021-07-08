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
