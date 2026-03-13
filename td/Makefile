
all: nqueens.o board.o list.o
	gcc -g -Wall nqueens.o board.o list.o -o nqueens

list.o: list.c 
	gcc -c -g -Wall list.c 

board.o: board.c
	gcc -c  -g -Wall board.c 

nqueens.o: nqueens.c
	gcc -c -g -Wall nqueens.c

clean:
	rm *.o


