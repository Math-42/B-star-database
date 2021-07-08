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

/**
 * Aloca uma struct do tipo arvore e inicializa seus valores
 * @param nomeArquivoIndice nome do arquivo onde os indices serão salvos
 * @return ponteiro para a struct do tipo arvore
 */
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

/**
 * Libera a struct alocada, salva as modificações e fecha o arquivo
 * @param currArvore arvore a ser liberada
 * 
 */
void finalizaArvore(arvore* currArvore) {
    currArvore->header.status = '1';
    salvaHeaderArvore(currArvore);

    fclose(currArvore->arquivoIndice);
    free(currArvore);
}

/**
 * Cria um novo nó de arbore b
 * @param isFolha define se o nó criado será folha ou não
 * @param RRndoNo RRN do nó que será criado
 * @return nó criado e inicializado
 */
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

/**
 * Salva o header da árvore no seu respectivo arquivo
 * @param currArvore arvore que terá seu header salvo
 */
void salvaHeaderArvore(arvore* currArvore) {
    fseek(currArvore->arquivoIndice, 0, 0);

    fwrite(&currArvore->header.status, sizeof(char), 1, currArvore->arquivoIndice);
    fwrite(&currArvore->header.noRaiz, sizeof(int), 1, currArvore->arquivoIndice);
    fwrite(&currArvore->header.RRNproxNo, sizeof(int), 1, currArvore->arquivoIndice);
    fwrite(&currArvore->header.lixo, sizeof(char), 68, currArvore->arquivoIndice);
}

/**
 * Le o header de uma arvore a partir de um arquivo
 * @param arquivoIndice arquivo de onde o header será lido
 * @param header variável onde o header será salvo
 */
void leHeaderArvore(FILE* arquivoIndice, arvoreHeader* header) {
    fseek(arquivoIndice, 0, 0);

    fread(&header->status, sizeof(char), 1, arquivoIndice);
    fread(&header->noRaiz, sizeof(int), 1, arquivoIndice);
    fread(&header->RRNproxNo, sizeof(int), 1, arquivoIndice);
    fread(&header->lixo, sizeof(char), 68, arquivoIndice);
}

/**
 * Aloca uma struct do tipo arvore e inicializa seus valores a partir de um arquivo já existente
 * @param nomeArquivoIndice nome do arquivo de onde os dados serão carregados
 * @return ponteiro para a struct do tipo arvore
 */
arvore* carregaArvore(char nomeArquivoIndice[]) {
    arvore* novaArvore = (arvore*)malloc(sizeof(arvore));

    novaArvore->arquivoIndice = fopen(nomeArquivoIndice, "r+");
    if (novaArvore->arquivoIndice == NULL) printf("Arquivo nulo \n");
    leHeaderArvore(novaArvore->arquivoIndice, &novaArvore->header);

    lerNoArvore(novaArvore, &novaArvore->raiz, novaArvore->header.noRaiz);

    return novaArvore;
}

/**
 * Lê um nó especifico de uma arvore
 * @param currArvore arvore de onde o nó será lido
 * @param novoNo variável onde o nó lido será salvo
 * @param RRN RRN do nó a ser lido
 */
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

/**
 * Salva um nó especifico de uma arvore
 * @param currArvore arvore de onde o nó será salvo
 * @param novoNo nó que será salvo
 * @param RRN RRN que indica onde o nó será salvo
 */
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

/**
 * Insere um registro um um array de registros de forma ordenada e corrigindo seus ponteiros
 * @param registros array de registros
 * @param novoRegistro registro que será inserido
 * @param tamanho tamanho do array (0,N-1)
 */
void insereRegistroOrdenado(registro registros[], registro novoRegistro, int tamanho) {
    int i;

    //percorre o vetor fazendo um shift em todos os registros posteriores a posição correta do novo registro
    for (i = tamanho; i > 0 && novoRegistro.C < registros[i - 1].C; i--) {
        registros[i] = registros[i - 1];
    }

    registros[i] = novoRegistro;
    if (i + 1 < ORDEM_ARVORE) registros[i + 1].P_ant = novoRegistro.P_prox;
}

/**
 * Busca um registro em um array de forma binária
 * @param registros array de registros
 * @param chave chave buscada
 * @param tamanho tamanho do array (0,N-1)
 * @return retorna o registro ou, caso não exista, o registro mais próximo do requisitado
 */
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

/**
 * Divide um nó em dois e elege um registro
 * @param currArvore arvore que contem o nó
 * @param currNo nó que será dividido
 * @param novoRegistro registro que está sendo inserido durante a divisão
 * @return ponteiro para o registro que foi eleito
 */
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

/**
 * Insere um novo registro no nó, caso possivel, e, se não, faz um "split"
 * @param currArvore arvore que contem o nó que receberá o novo registro
 * @param currNo nó onde o registro será inserido (ou que será dividido)
 * @param novoRegistro registro que será inserido
 * @return ponteiro para o registro que foi eleito ou NULL caso nenhum seja eleito
 */
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

/**
 * Busca recursivamente o local onde o novo registro deve ser inserido
 * @param currArvore arvore que contem o nó que receberá o novo registro
 * @param currNo nó atual da busca
 * @param novoRegistro registro que será inserido
 * @return ponteiro para o registro que foi eleito ou NULL caso nenhum seja eleito
 */
registro* buscaInsersaoRecursao(arvore* currArvore, arvoreNo* currNo, registro novoRegistro) {
    if (currNo->folha == '1' || currArvore->header.noRaiz == 0) {//testa se é folha ou se raiz = folha
        return insereNovoRegistro(currArvore, currNo, novoRegistro);
    }

    //busca  o próximo nó por onde se deve continuar a busca
    registro registroPai = buscaBinariaRegistro(currNo->registros, novoRegistro.C, currNo->nroChavesIndexadas);
    arvoreNo proxNo;
    int RRNproxReg = registroPai.C > novoRegistro.C ? registroPai.P_ant : registroPai.P_prox;
    lerNoArvore(currArvore, &proxNo, RRNproxReg);

    registro* registroEleito = buscaInsersaoRecursao(currArvore, &proxNo, novoRegistro);

    //testa se algum registro foi eleito no processo
    if (registroEleito != NULL) {
        registro* novoRegistroEleito = insereNovoRegistro(currArvore, currNo, *registroEleito);
        free(registroEleito);
        return novoRegistroEleito;
    }
}

/**
 * Insere um novo registro na árvore
 * @param currArvore arvore que contem o nó que receberá o novo registro
 * @param novoRegistro registro que será inserido
 */
void insereRegistro(arvore* currArvore, registro novoRegistro) {
    if (currArvore->header.noRaiz == -1) {// testa se já existe uma raiz, caso não exista ela será criada
        currArvore->raiz = criarNovoNo('0', currArvore->header.RRNproxNo);
        currArvore->header.noRaiz = currArvore->raiz.RRNdoNo;
        salvaNoArvore(currArvore, &currArvore->raiz, currArvore->header.RRNproxNo);
        currArvore->header.RRNproxNo++;
    }


    registro* registroEleitoParaRaiz = buscaInsersaoRecursao(currArvore, &currArvore->raiz, novoRegistro);

    //testa se um registro foi eleito, nesse caso a raiz deve ser atualizada
    if (registroEleitoParaRaiz != NULL) {
        currArvore->raiz = criarNovoNo('0', currArvore->header.RRNproxNo);  //cria uma nova raiz
        currArvore->header.noRaiz = currArvore->raiz.RRNdoNo;
        currArvore->header.RRNproxNo++;

        insereNovoRegistro(currArvore, &currArvore->raiz, *registroEleitoParaRaiz);
        free(registroEleitoParaRaiz);
    }
}

/**
 * Busca recursivamente um registro com base na chave passada
 * @param currArvore arvore onde a busca ocorrerá
 * @param currNo nó atual da busca
 * @param chaveRegistro chave da busca
 * @return byteOffset do registro no arquivo original ou -1 caso não encontre
 */
long int buscaRegistroRecursao(arvore* currArvore, arvoreNo* currNo, int chaveRegistro) {
    //procura o registro
    registro registroPai = buscaBinariaRegistro(currNo->registros, chaveRegistro, currNo->nroChavesIndexadas);
    if (registroPai.C == chaveRegistro) return registroPai.Pr;

    //se nãso achou decide o próximo nó
    int RRNproxReg = registroPai.C > chaveRegistro ? registroPai.P_ant : registroPai.P_prox;

    if (RRNproxReg == -1) {//testa se existe o próximo nó
        return -1;
    } else {//continua a busca
        arvoreNo proxNo;
        lerNoArvore(currArvore, &proxNo, RRNproxReg);
        return buscaRegistroRecursao(currArvore, &proxNo, chaveRegistro);
    }
}

/**
 * Busca um registro com base na chave passada
 * @param currArvore arvore onde a busca ocorrerá
 * @param chaveRegistro chave da busca
 * @return byteOffset do registro no arquivo original ou -1 caso não encontre
 */
long int buscaRegistro(arvore* currArvore, int chave) {
    if (currArvore->header.noRaiz == -1) {
        return -1;
    }
    return buscaRegistroRecursao(currArvore, &currArvore->raiz, chave);
}