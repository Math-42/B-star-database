#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



void lerStringFixa_CSV(FILE* arquivo, char* string, int tamanho) {
    fread(string, sizeof(char), tamanho, arquivo);
    fseek(arquivo, 1, SEEK_CUR);  // Pula a virgula
}

int finalDoArquivo(FILE* arquivo){
    int isFinal = 0;
    char finalByte;
    isFinal = (fread(&finalByte,sizeof(char),1,arquivo)==0)? 1:0;
    if(!isFinal) fseek(arquivo,-1,SEEK_CUR);
    return isFinal;
}

char* ajustaString(char* string) {
    return (string[0] == '\0') ? "campo com valor nulo" : string;
}

void imprimeInteiro(int inteiro){
    if(inteiro == -1){
        printf("campo com valor nulo\n");
    } else {
        printf("%d\n",inteiro);
    }
}

void imprimirCampo(char* descricao, void* valor, int isInt){
    if(isInt >0){
        int* valorInteiro =  (int*)valor;
        printf("%s: ",descricao);
        imprimeInteiro(*valorInteiro);
    } else {
        char* valorString =  (char*)valor;
        printf("%s: %s\n",descricao,ajustaString(valorString));
    }
}

// FUNCOES FORNECIDAS PELOS MONITORES
void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


