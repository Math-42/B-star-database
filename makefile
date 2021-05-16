all: 
	gcc source/utils.c main.c -o exec
leak:
	gcc -g main.c -o exec && valgrind --leak-check=full -s ./exec
run:
	./exec