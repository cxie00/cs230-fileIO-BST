rule1: main.o tree.o
	gcc -std=c99 main.o tree.o -o wordfreak -lm 

main.o: main.c
	gcc -std=c99 main.c -c

tree.o: tree.c
	gcc -std=c99 tree.c -c
clean: 
	rm *.o