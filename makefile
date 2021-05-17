all: 
	gcc source/csv/csv.c source/utils/utils.c source/veiculo/veiculo.c source/linha/linha.c main.c -o exec
leak:
	gcc -g source/csv/csv.c source/utils/utils.c source/veiculo/veiculo.c source/linha/linha.c main.c -o exec && valgrind --leak-check=full -s ./exec
run:
	./exec