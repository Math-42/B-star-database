/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * Trabalho 1 da disciplina de organização de arquivos * Autores: Israel
 * Felipe da Silva   - 11796531
 * * Matheus Vieira Gonçalves - 11200397                                    *
 *                     															   *
 * Maio/2021 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * */

#include <stdio.h>
#include <stdlib.h>

#include "source/linha/linha.h"
#include "source/utils/utils.h"
#include "source/veiculo/veiculo.h"

void menu() {

    int funcionalidade, numeroDeEntradas;
    char nomeArquivoCSV[100];
    char nomeArquivoBin[100];
    char nomeDoCampoBuscado[100];
    char valorBuscado[100];

    scanf("%d", &funcionalidade);//le o modo que o programa deve operar

    switch (funcionalidade) {
        case 1:
            scanf("%s %s", nomeArquivoCSV, nomeArquivoBin);
            CreateTable_Veiculo(nomeArquivoCSV, nomeArquivoBin);
            return;
            break;
        case 2:
            scanf("%s %s", nomeArquivoCSV, nomeArquivoBin);
            CreateTable_Linha(nomeArquivoCSV, nomeArquivoBin);
            return;
            break;
        case 3:
            scanf("%s", nomeArquivoBin);
            SelectFrom_Veiculo(nomeArquivoBin);
            return;
            break;
        case 4:
            scanf("%s", nomeArquivoBin);
            SelectFrom_Linha(nomeArquivoBin);
            return;
            break;
        case 5:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeDoCampoBuscado);
            scan_quote_string(valorBuscado);
            SelectFromWhere_Veiculo(nomeArquivoBin, nomeDoCampoBuscado, valorBuscado);
            return;
            break;
        case 6:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeDoCampoBuscado);
            scan_quote_string(valorBuscado);
            SelectFromWhere_Linha(nomeArquivoBin, nomeDoCampoBuscado, valorBuscado);
            return;
            break;
        case 7:
            scanf("%s", nomeArquivoBin);
            scanf("%d", &numeroDeEntradas);
            InsertInto_Veiculo(nomeArquivoBin, numeroDeEntradas);
            return;
            break;
        case 8:
            scanf("%s", nomeArquivoBin);
            scanf("%d", &numeroDeEntradas);
            InsertInto_Linha(nomeArquivoBin, 4);
            return;
            break;
        default:
            break;
    }
}

int main() {
    menu();
    return 0;
}