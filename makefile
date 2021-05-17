all: 
	gcc source/utils.c source/veiculo.c source/linha.c main.c -o exec
leak:
	gcc -g source/utils.c source/veiculo.c source/linha.c main.c -o exec && valgrind --leak-check=full -s ./exec
run:
	./exec