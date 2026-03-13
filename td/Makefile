
all: nqueens.o board.o list.o
	gcc - c -Wall -g nqueens.o board.o list.o

list.o: list.c list.h
	gcc -c -Wall -g list.c

board.o: board.c board.h
	gcc -c -Wall -g board.c

nqueens.o: nqueens.c
	gcc -c -Wall -g nqueens.c

clean:
	rm *.o


