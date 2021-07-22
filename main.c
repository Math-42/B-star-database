/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * Trabalho 1 da disciplina de organização de arquivos                     *
 * Autores:                                                                    *
 * Israel Felipe da Silva   - 11796531                                         *
 * Matheus Vieira Gonçalves - 11200397                                         *
 *                     														   *
 * Maio/2021                                                                   *    
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>

#include "source/linha/linha.h"
#include "source/utils/utils.h"
#include "source/veiculo/veiculo.h"

void menu() {
    int funcionalidade, numeroDeEntradas;
    char nomeArquivoCSV[100];
    char nomeArquivoBin[100];
    char nomeArquivoBin2[100];
    char nomeArquivoBinOrdenado[100];
    char nomeArquivoBinIndice[100];
    char nomeDoCampoBuscado[100];
    char valorBuscado[100];
    int valorBuscadoVeiculoArvore;
    scanf("%d", &funcionalidade);  //le o modo que o programa deve operar

    switch (funcionalidade) {
        case 1:
            scanf("%s %s", nomeArquivoCSV, nomeArquivoBin);
            CreateTable_Veiculo(nomeArquivoCSV, nomeArquivoBin);
            break;
        case 2:
            scanf("%s %s", nomeArquivoCSV, nomeArquivoBin);
            CreateTable_Linha(nomeArquivoCSV, nomeArquivoBin);
            break;
        case 3:
            scanf("%s", nomeArquivoBin);
            SelectFrom_Veiculo(nomeArquivoBin);
            break;
        case 4:
            scanf("%s", nomeArquivoBin);
            SelectFrom_Linha(nomeArquivoBin);
            break;
        case 5:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeDoCampoBuscado);
            scan_quote_string(valorBuscado);
            SelectFromWhere_Veiculo(nomeArquivoBin, nomeDoCampoBuscado, valorBuscado);
            break;
        case 6:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeDoCampoBuscado);
            scan_quote_string(valorBuscado);
            SelectFromWhere_Linha(nomeArquivoBin, nomeDoCampoBuscado, valorBuscado);
            break;
        case 7:
            scanf("%s", nomeArquivoBin);
            scanf("%d", &numeroDeEntradas);
            InsertInto_Veiculo(nomeArquivoBin, numeroDeEntradas);
            break;
        case 8:
            scanf("%s", nomeArquivoBin);
            scanf("%d", &numeroDeEntradas);
            InsertInto_Linha(nomeArquivoBin, numeroDeEntradas);
            break;
        case 9:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeArquivoBinIndice);
            CreateIndex_Veiculo(nomeArquivoBin, nomeArquivoBinIndice);
            break;
        case 10:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeArquivoBinIndice);
            CreateIndex_Linha(nomeArquivoBin, nomeArquivoBinIndice);
            break;
        case 11:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeDoCampoBuscado);
            scanf("%*s");
            scan_quote_string(valorBuscado);
            SelectFromWithIndex_Veiculo(nomeArquivoBin, nomeDoCampoBuscado, valorBuscado);
            break;
        case 12:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeDoCampoBuscado);
            scanf("%*s");
            scanf("%d", &valorBuscadoVeiculoArvore);
            SelectFromWithIndex_Linha(nomeArquivoBin, nomeDoCampoBuscado, valorBuscadoVeiculoArvore);
            break;
        case 13:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeArquivoBinIndice);
            scanf("%d", &numeroDeEntradas);
            InsertIntoWithIndex_Veiculo(nomeArquivoBin, nomeArquivoBinIndice, numeroDeEntradas);
            break;
        case 14:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeArquivoBinIndice);
            scanf("%d", &numeroDeEntradas);
            InsertIntoWithIndex_Linha(nomeArquivoBin, nomeArquivoBinIndice, numeroDeEntradas);
            break;
        case 17:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeArquivoBinOrdenado);
            scanf("%*s");
            if (SortReg_Veiculo(nomeArquivoBin, nomeArquivoBinOrdenado)) binarioNaTela(nomeArquivoBinOrdenado);
            break;
        case 18:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeArquivoBinOrdenado);
            scanf("%*s");
            if (SortReg_Linha(nomeArquivoBin, nomeArquivoBinOrdenado)) binarioNaTela(nomeArquivoBinOrdenado);
            break;
        case 19:
            scanf("%s", nomeArquivoBin);
            scanf("%s", nomeArquivoBin2);
            scanf("%*s %*s");
            if (!SortReg_Veiculo(nomeArquivoBin, "veiculo-ordenado.bin")) return;
            if (!SortReg_Linha(nomeArquivoBin2, "linha-ordenado.bin")) return;
            Search_LinhaVeiculo("veiculo-ordenado.bin", "linha-ordenado.bin");
            break;
        default:
            break;
    }
}

int main() {
    menu();
    return 0;
}