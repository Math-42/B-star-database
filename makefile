all: 
	gcc source/arvore/arvore.c source/binario/binario.c source/csv/csv.c source/utils/utils.c source/veiculo/veiculo.c source/linha/linha.c main.c -o exec
leak:
	gcc -g source/arvore/arvore.c source/binario/binario.c source/csv/csv.c source/utils/utils.c source/veiculo/veiculo.c source/linha/linha.c main.c -o exec && valgrind --leak-check=full -s ./exec
teste_1: all
	gcc testes.c -o execTestes && (./execTestes 24 casosDeTeste_1/ambienteTeste/) && rm execTestes
teste_2: all
	gcc testes.c -o execTestes && (./execTestes 12 casosDeTeste_2/ambienteTeste/) && rm execTestes
teste_3: all
	gcc testes.c -o execTestes && (./execTestes 17 casosDeTeste_3/ambienteTeste/) && rm execTestes
teste: teste_1 teste_2 teste_3
zip:
	zip -r entrega source/ main.c makefile 
run:
	./exec