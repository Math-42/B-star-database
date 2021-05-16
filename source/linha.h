#ifndef _LINHA_H_
#define _LINHA_H_

typedef struct {
    char status;
    long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[18];
	char descreveData [35];
	char descreveLugares[42];
	char descreveLinha [26];
	char descreveModelo [17];
	char descreveCategoria [20];

} linhaHeader;
typedef struct{
	char removido;
	int tamanhoRegistro;
	int codLinha;
	char aceitaCartao;
	int tamanhoNome;
	char nomeLinha[100];
	int tamanhoCor;
	char corLinha[100];
}linha;

#endif