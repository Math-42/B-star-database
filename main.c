#include <stdio.h>
#include <stdlib.h>
#include "source/veiculo/veiculo.h"

void menu(int funcionalidade) {
	switch (funcionalidade){
	case 1:
		CreateTable_Veiculo("casosDeTeste/csv/veiculo.csv","casosDeTeste/out/veiculo.bin");
		return;
		break;
	case 2:
		CreateTable_Veiculo("casosDeTeste/csv/linha.csv","casosDeTeste/out/veiculo.bin");
		return;
		break;
	case 3:
		SelectFrom_Veiculo("casosDeTeste/teste/veiculo5.bin");
		return;
		break;
	
	default:
		break;
	}
}

int main() {
	menu(3);
	return 0;
}