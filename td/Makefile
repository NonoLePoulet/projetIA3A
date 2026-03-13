
all: nqueens.o board.o list.o knights.o
	gcc -g -Wall nqueens.o board.o list.o -o nqueens
	gcc -g -Wall knights.o board.o list.o -o knights

list.o: list.c 
	gcc -c -g -Wall list.c 

board.o: board.c
	gcc -c  -g -Wall board.c 

nqueens.o: nqueens.c
	gcc -c -g -Wall nqueens.c

knights.o: knights.c
	gcc -c -g -Wall knights.c

clean:
	rm *.o


