all: 
	gcc source/arvore/arvore.c source/binario/binario.c source/csv/csv.c source/utils/utils.c source/veiculo/veiculo.c source/linha/linha.c main.c -o exec
leak:
	gcc -g source/arvore/arvore.c source/binario/binario.c source/csv/csv.c source/utils/utils.c source/veiculo/veiculo.c source/linha/linha.c main.c -o exec && valgrind --leak-check=full -s ./exec
teste_1: all
	./testes.sh 24 casosDeTeste_1/ambienteTeste/
teste_2: all
	./testes.sh 18 casosDeTeste_2/ambienteTeste/
teste_3: all
	./testes.sh 17 casosDeTeste_3/ambienteTeste/
teste: teste_1 teste_2 teste_3
zip:
	zip -r entrega source/ main.c makefile 
run:
	./exec
