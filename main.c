#include <stdio.h>
#include <stdlib.h>
#include "source/veiculo.h"

void menu(int funcionalidade) {
	switch (funcionalidade){
	case 1:
		CreateTable_Veiculo("casosDeTeste/csv/veiculo.csv","casosDeTeste/out/veiculo.bin");
		return;
		break;
	
	default:
		break;
	}
}

int main() {
	menu(1);
	return 0;
}