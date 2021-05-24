#include <stdio.h>
#include <stdlib.h>
#include "source/veiculo/veiculo.h"
#include "source/linha/linha.h"

void menu(int funcionalidade) {
	switch (funcionalidade){
	case 1:
		CreateTable_Veiculo("casosDeTeste/csv/veiculo.csv","casosDeTeste/out/veiculo.bin");
		return;
		break;
	case 2:
		CreateTable_Linha("casosDeTeste/teste/linha3.csv","casosDeTeste/out/linha3.bin");
		return;
		break;
	case 3:
		SelectFrom_Veiculo("casosDeTeste/teste/veiculo5.bin");
		return;
		break;
	case 4:
		SelectFrom_Linha("casosDeTeste/teste/linha7.bin");
		return;
		break;
	
	default:
		break;
	}
}

int main() {
	menu(4);
	return 0;
}