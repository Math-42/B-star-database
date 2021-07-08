#include "arvore.h"

void imprimeHeader(arvore* currArvore) {
    printf("============== HEADER ==============\n");
    printf("status: %c\n", currArvore->header.status);
    printf("noRaiz: %d\n", currArvore->header.noRaiz);
    printf("RRNproxNo: %d\n", currArvore->header.RRNproxNo);
    printf("lixo: %s\n", currArvore->header.lixo);
    printf("====================================\n");
}

void imprimeNo(arvoreNo* no) {
    printf("%d:|", no->RRNdoNo);
    printf("F: %c ", no->folha);
    printf("N: %d|", no->nroChavesIndexadas);

    for (int i = 0; i < ORDEM_ARVORE - 1; i++) {
        printf(" %d:", i);
        printf("[(%d,", no->registros[i].P_ant);
        printf("%d), ", no->registros[i].P_prox);
        printf("%d->", no->registros[i].C);
        printf("%ld]", no->registros[i].Pr);
    }
    printf("\n\n");
}

void imprimeRegistro(registro* registroEleito) {
    printf("\nREGISTRO ELEITO:");
    printf("[(%d,", registroEleito->P_ant);
    printf("%d), ", registroEleito->P_prox);
    printf("%c->", registroEleito->C);
    printf("%ld]\n", registroEleito->Pr);
}

void imprimeNoRecursivo(arvore* currArvore, arvoreNo* no, int depth) {
    if (depth != 0)
        printf("%*c└", (depth), ' ');

    printf("%4d:|", no->RRNdoNo);

    //printf("%*d:|", depth, no->RRNdoNo);
    printf("F: %c ", no->folha);
    printf("N: %d|", no->nroChavesIndexadas);

    for (int i = 0; i < ORDEM_ARVORE - 1; i++) {
        printf(" %d:", i);
        printf("[(%d,", no->registros[i].P_ant);
        printf("%d), ", no->registros[i].P_prox);
        printf("%d->", no->registros[i].C);
        printf("%ld]", no->registros[i].Pr);
    }
    printf("\n");

    for (int i = 0; i < ORDEM_ARVORE - 1; i++) {
        if (no->registros[i].C != -1 && no->registros[i].P_ant != -1) {
            arvoreNo proxNo;
            lerNoArvore(currArvore, &proxNo, no->registros[i].P_ant);
            imprimeNoRecursivo(currArvore, &proxNo, depth + 6);
        }
    }
    if (no->registros[no->nroChavesIndexadas - 1].P_prox != -1) {
        arvoreNo proxNo;
        lerNoArvore(currArvore, &proxNo, no->registros[no->nroChavesIndexadas - 1].P_prox);
        imprimeNoRecursivo(currArvore, &proxNo, depth + 6);
    }
}

void imprimeArvore(arvore* currArvore) {
    printf("raiz: %d\n", currArvore->header.noRaiz);
    imprimeHeader(currArvore);
    imprimeNoRecursivo(currArvore, &currArvore->raiz, 0);
}

arvore* criaArvore(char nomeArquivoIndice[]) {
    arvore* novaArvore = (arvore*)malloc(sizeof(arvore));

    novaArvore->header.status = '1';
    novaArvore->header.noRaiz = -1;
    novaArvore->header.RRNproxNo = 0;

    for (int i = 0; i < 68; i++) novaArvore->header.lixo[i] = '@';

    novaArvore->arquivoIndice = fopen(nomeArquivoIndice, "w+");
    salvaHeaderArvore(novaArvore);

    return novaArvore;
}

void finalizaArvore(arvore* currArvore) {
    currArvore->header.status = '1';
    salvaHeaderArvore(currArvore);

    fclose(currArvore->arquivoIndice);
    free(currArvore);
}

arvoreNo criarNovoNo(char isFolha, int RRndoNo) {
    arvoreNo noCriado;

    noCriado.folha = isFolha;
    noCriado.nroChavesIndexadas = 0;
    noCriado.RRNdoNo = RRndoNo;

    for (int i = 0; i < ORDEM_ARVORE - 1; i++) {
        noCriado.registros[i].P_ant = -1;
        noCriado.registros[i].P_prox = -1;
        noCriado.registros[i].C = -1;
        noCriado.registros[i].Pr = -1;
    }

    return noCriado;
}

void salvaHeaderArvore(arvore* currArvore) {
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

arvore* carregaArvore(char nomeArquivoIndice[]) {
    arvore* novaArvore = (arvore*)malloc(sizeof(arvore));

    novaArvore->arquivoIndice = fopen(nomeArquivoIndice, "r+");
    if (novaArvore->arquivoIndice == NULL) printf("Arquivo nulo \n");
    leHeaderArvore(novaArvore->arquivoIndice, &novaArvore->header);

    lerNoArvore(novaArvore, &novaArvore->raiz, novaArvore->header.noRaiz);

    imprimeArvore(novaArvore);

    return novaArvore;
}

void lerNoArvore(arvore* currArvore, arvoreNo* novoNo, int RRN) {
    fseek(currArvore->arquivoIndice, (RRN + 1) * 77, 0);

    fread(&novoNo->folha, sizeof(char), 1, currArvore->arquivoIndice);
    fread(&novoNo->nroChavesIndexadas, sizeof(int), 1, currArvore->arquivoIndice);
    fread(&novoNo->RRNdoNo, sizeof(int), 1, currArvore->arquivoIndice);

    fread(&novoNo->registros[0].P_ant, sizeof(int), 1, currArvore->arquivoIndice);

    for (int i = 0; i < ORDEM_ARVORE - 1; i++) {
        fread(&novoNo->registros[i].C, sizeof(int), 1, currArvore->arquivoIndice);
        fread(&novoNo->registros[i].Pr, sizeof(long int), 1, currArvore->arquivoIndice);
        fread(&novoNo->registros[i].P_prox, sizeof(int), 1, currArvore->arquivoIndice);
    }

    for (int i = 0; i < ORDEM_ARVORE - 2; i++) {
        novoNo->registros[i + 1].P_ant = novoNo->registros[i].P_prox;
    }
}

void salvaNoArvore(arvore* currArvore, arvoreNo* novoNo, int RRN) {
    fseek(currArvore->arquivoIndice, (RRN + 1) * 77, 0);

    fwrite(&novoNo->folha, sizeof(char), 1, currArvore->arquivoIndice);
    fwrite(&novoNo->nroChavesIndexadas, sizeof(int), 1, currArvore->arquivoIndice);
    fwrite(&novoNo->RRNdoNo, sizeof(int), 1, currArvore->arquivoIndice);

    fwrite(&novoNo->registros[0].P_ant, sizeof(int), 1, currArvore->arquivoIndice);

    for (int i = 0; i < ORDEM_ARVORE - 1; i++) {
        fwrite(&novoNo->registros[i].C, sizeof(int), 1, currArvore->arquivoIndice);
        fwrite(&novoNo->registros[i].Pr, sizeof(long int), 1, currArvore->arquivoIndice);
        fwrite(&novoNo->registros[i].P_prox, sizeof(int), 1, currArvore->arquivoIndice);
    }
}

void insereRegistroOrdenado(registro registros[], registro novoRegistro, int tamanho) {
    int i;

    //percorre o vetor fazendo um shift em todos os registros posteriores a posição correta do novo registro
    for (i = tamanho; i > 0 && novoRegistro.C < registros[i - 1].C; i--) {
        registros[i] = registros[i - 1];
    }

    registros[i] = novoRegistro;
    if (i + 1 < ORDEM_ARVORE) registros[i + 1].P_ant = novoRegistro.P_prox;
}

registro buscaBinariaRegistro(registro registros[], int chave, int tamanho) {
    int inicio = 0;
    int fim = tamanho - 1;
    int indice;

    while (inicio <= fim) {
        indice = (inicio + fim) / 2;
        if (registros[indice].C == chave)
            break;
        else if (registros[indice].C > chave)
            fim = indice - 1;
        else
            inicio = indice + 1;
    }

    return registros[indice];
}

registro* splitNo(arvore* currArvore, arvoreNo* currNo, registro novoRegistro) {
    registro* registroEleito = (registro*)malloc(sizeof(registro));

    //cria um novo nó a direita
    //no caso no raiz=folha ele deixa de ser raiz e passa a ser folha
    currNo->folha = (currArvore->header.noRaiz == 0) ? '1' : currNo->folha;
    arvoreNo novoNoEsquerda = criarNovoNo(currNo->folha, currNo->RRNdoNo);
    arvoreNo novoNoDireita = criarNovoNo(currNo->folha, currArvore->header.RRNproxNo);

    //copia os antigos e insere o novo registro no array temporário, na ordem correta
    registro* tempSplitArray = (registro*)malloc(ORDEM_ARVORE * sizeof(registro));
    for (int i = 0; i < ORDEM_ARVORE - 1; i++) tempSplitArray[i] = currNo->registros[i];
    insereRegistroOrdenado(tempSplitArray, novoRegistro, ORDEM_ARVORE - 1);

    // distribui os registros,já ordenados, entre os dois nós
    for (int i = 0; i < ORDEM_ARVORE / 2; i++) {
        novoNoEsquerda.registros[i] = tempSplitArray[i];
        novoNoEsquerda.nroChavesIndexadas++;
    }

    for (int i = ORDEM_ARVORE / 2 + 1, j = 0; i < ORDEM_ARVORE; i++, j++) {
        novoNoDireita.registros[j] = tempSplitArray[i];
        novoNoDireita.nroChavesIndexadas++;
    }

    salvaNoArvore(currArvore, &novoNoEsquerda, novoNoEsquerda.RRNdoNo);
    salvaNoArvore(currArvore, &novoNoDireita, novoNoDireita.RRNdoNo);

    // elege um registro
    registroEleito->C = tempSplitArray[ORDEM_ARVORE / 2].C;
    registroEleito->Pr = tempSplitArray[ORDEM_ARVORE / 2].Pr;
    registroEleito->P_ant = novoNoEsquerda.RRNdoNo;
    registroEleito->P_prox = novoNoDireita.RRNdoNo;

    free(tempSplitArray);
    currArvore->header.RRNproxNo++;

    return registroEleito;
}

registro* insereNovoRegistro(arvore* currArvore, arvoreNo* currNo, registro novoRegistro) {
    if (currNo->nroChavesIndexadas < ORDEM_ARVORE - 1) {
        insereRegistroOrdenado(currNo->registros, novoRegistro, currNo->nroChavesIndexadas);
        currNo->nroChavesIndexadas++;
        salvaNoArvore(currArvore, currNo, currNo->RRNdoNo);
        return NULL;
    } else {
        return splitNo(currArvore, currNo, novoRegistro);
    }
}

registro* buscaInsersaoRecursao(arvore* currArvore, arvoreNo* currNo, registro novoRegistro) {
    if (currNo->folha == '1' || currArvore->header.noRaiz == 0) {
        return insereNovoRegistro(currArvore, currNo, novoRegistro);
    }

    arvoreNo proxNo;
    registro registroPai = buscaBinariaRegistro(currNo->registros, novoRegistro.C, currNo->nroChavesIndexadas);
    int RRNproxReg = registroPai.C > novoRegistro.C ? registroPai.P_ant : registroPai.P_prox;
    lerNoArvore(currArvore, &proxNo, RRNproxReg);

    registro* registroEleito = buscaInsersaoRecursao(currArvore, &proxNo, novoRegistro);
    
    if (registroEleito != NULL) {
        registro* novoRegistroEleito = insereNovoRegistro(currArvore, currNo, *registroEleito);
        free(registroEleito);
        return novoRegistroEleito;
    }
}

void insereRegistro(arvore* currArvore, registro novoRegistro) {
    if (currArvore->header.noRaiz == -1) {
        currArvore->raiz = criarNovoNo('0', currArvore->header.RRNproxNo);
        currArvore->header.noRaiz = currArvore->raiz.RRNdoNo;
        salvaNoArvore(currArvore, &currArvore->raiz, currArvore->header.RRNproxNo);
        currArvore->header.RRNproxNo++;
    }

    registro* registroEleitoParaRaiz = buscaInsersaoRecursao(currArvore, &currArvore->raiz, novoRegistro);

    if (registroEleitoParaRaiz != NULL) {
        currArvore->raiz = criarNovoNo('0', currArvore->header.RRNproxNo);  //cria uma nova raiz
        currArvore->header.noRaiz = currArvore->raiz.RRNdoNo;
        currArvore->header.RRNproxNo++;

        insereNovoRegistro(currArvore, &currArvore->raiz, *registroEleitoParaRaiz);
        free(registroEleitoParaRaiz);
    }
}