#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Abre um arquivo e testa se ele já existe ou não
 * @param arquivo ponteiro do tipo FILE*
 * @param nomeDoArquivo nome do arquivo que será aberto
 * @param tipoDeAbertura a forma que ele deve ser aberto, rb, wb+ e etc
 * @param verificaExistencia flag que indica se deve ou não testar a existencia, 0 para falso, 1 ou mais para verdade
 * @return retorna 1 caso o arquivo exista (ou caso não for exigida a verificação) e 0 caso não exista
 */
int abrirArquivo(FILE** arquivo, char* nomeDoArquivo, char* tipoDeAbertura, int verificaExistencia) {
    int existe = 1;
    *arquivo = fopen(nomeDoArquivo, tipoDeAbertura);
    if (verificaExistencia > 0 && *arquivo == NULL) {
        existe = 0;
        printf("Falha no processamento do arquivo.");
    }
    return existe;
}

/**
 * Transforma um inteiro no formato string em um inteiro
 * @param string string inicial
 * @param tamanho quantidade de digitos do número
 * @return inteiro convertido
 */
int stringToInt(char string[11], int tamanho) {
    int valorLido = 0;
    for (int i = 0; i < tamanho; i++) {  //percorre cada digito transformando em int
        valorLido = valorLido * 10 + (string[i] - '0');
    }
    return valorLido;
}

/**
 * Determina se é o final do arquivo sem alterar a posição do ponteiro
 * caso não seja
 * @param arquivo arquivo a ser lido
 * @return retorna 1 caso seja o final e 0 caso não seja
 */
int finalDoArquivo(FILE* arquivo) {
    int isFinal = 0;
    char finalByte;
    isFinal = (fread(&finalByte, sizeof(char), 1, arquivo) == 0) ? 1 : 0;
    if (!isFinal) fseek(arquivo, -1, SEEK_CUR);  //volta 1 byte caso não esteja no final
    return isFinal;
}

/**
 * Determina se uma string é nula, ou seja, se já começa com \0
 * @param string string a ser analisada
 * @return retorna a própria string ou uma string padrão caso seja nula
 */
char* ajustaString(char* string) {
    return (string[0] == '\0') ? "campo com valor nulo" : string;
}

/**
 * Imprime um inteiro ou campo com valor nulo caso ele seja -1
 * @param inteiro inteiro a ser analisada
 */
void imprimeInteiro(int inteiro) {
    if (inteiro == -1) {
        printf("campo com valor nulo\n");
    } else {
        printf("%d\n", inteiro);
    }
}

/**
 * Imprime um campo de um registro
 * @param descricao descrição do campo
 * @param valor ponteiro para o valor que será impresso
 * @param isInt determina se o valor a ser impresso é inteiro ou string, sendo > 0 para
 * @return retorna a própria string ou uma string padrão caso seja nula
 */
void imprimirCampo(char* descricao, void* valor, int isInt) {
    if (isInt > 0) {  //testa se o valor recebido é inteiro ou não
        int* valorInteiro = (int*)valor;
        printf("%s: ", descricao);
        imprimeInteiro(*valorInteiro);
    } else {
        char* valorString = (char*)valor;
        printf("%s: %s\n", descricao, ajustaString(valorString));
    }
}

/**
 * lê uma string do terminal considerando campos nulos
 * @param string string onde o dado será salvo
 * @return retorna o tamanho da string lida
 */
int lerStringTerminal(char string[100]) {
    scan_quote_string(string);
    int tamanho = (int)strlen(string);
    string[tamanho] = '\0';
    return tamanho;
}

/**
 * Lê um campo de string do terminal de tamanho fixo considerando campos nulos
 * @param string local onde o conteudo será salvo
 * @param tamanhoMaximo tamanho máximo aceito
 * @return retorna o tamanho real da string lida
 */
int lerStringTerminalFixa(char* string, int tamanhoMaximo) {
    int tamanhoReal = lerStringTerminal(string);
    for (int i = tamanhoReal + 1; i < tamanhoMaximo; i++) string[i] = '@';  //completa de @ após o \0 caso necessário
    return tamanhoReal;
}

/**
 * Lê um campo de inteiro do terminal
 * @return retorna o inteiro lido ou -1 caso seja nulo
 */
int lerInteiroTerminal() {
    char numero[5];
    scanf("%s", numero);
    return (numero[0] == 'N') ? -1 : atoi(numero);
}

// FUNCOES FORNECIDAS PELOS MONITORES
void binarioNaTela(char* nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

    /* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

    unsigned long i, cs;
    unsigned char* mb;
    size_t fl;
    FILE* fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char*)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

void scan_quote_string(char* str) {
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

    while ((R = getchar()) != EOF && isspace(R))
        ;  // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n') {  // campo NULO
        getchar();
        getchar();
        getchar();        // ignorar o "ULO" de NULO.
        strcpy(str, "");  // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) {  // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();          // ignorar aspas fechando
    } else if (R != EOF) {  // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else {  // EOF
        strcpy(str, "");
    }
}

int convertePrefixo(char* str) {
    /* O registro que tem essa string como chave foi removido */
    if (str[0] == '*')
        return -1;

    /* Começamos com o primeiro digito na ordem de 36^0 = 1 */
    int power = 1;

    /* Faz a conversão char por char para chegar ao resultado */
    int result = 0;
    for (int i = 0; i < 5; i++) {
        /* 
            Interpreta o char atual como se fosse um digito
            em base 36. Os digitos da base 36 são:
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
            E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
            S, T, U, V, W, X, Y, Z
        */
        int cur_digit;
        /* Checa pelos digitos normais e os converte para números */
        if (str[i] >= '0' && str[i] <= '9')
            cur_digit = str[i] - '0';
        /* Checa pelas letras e as converte para números */
        else if (str[i] >= 'A' && str[i] <= 'Z')
            cur_digit = 10 + str[i] - 'A';

        /*
            Multiplica o digito atual pelo ordem da posição atual
            e adiciona no resultado
            Primeira posição:   36^0 = 1
            Segunda posição:    36^1 = 36
            Terceira posição:   36^2 = 1.296
            Quarta posição:     36^3 = 46.656
            Quinta posição:     36^4 = 1.679.616
        */
        result += cur_digit * power;

        /* Aumenta a ordem atual */
        power *= 36;
    }

    return result;
}