#ifndef _LINHA_H_
#define _LINHA_H_

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