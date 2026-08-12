main : compiler.c processor.c memory.c main.c
	gcc -c main.c -o main.o
	gcc -c compiler.c -o compiler.o
	gcc -c processor.c -o processor.o
	gcc -c memory.c -o memory.o

	gcc main.o compiler.o processor.o memory.o -o simulator
	./simulator

	rm -f compiler.o processor.o memory.o main.o simulator