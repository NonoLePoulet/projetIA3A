#include "list.h"

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

#define MAX_BOARD 64 //25
#define WH_BOARD 8 //5



Item *initGame();

Item *initGameKnights();

void initBoard(Item *node, char *board);

Item *getChildBoard( Item *node, int pos );

int isValidPosition( Item *node, int pos);

int isValidPositionKnights( Item *node, int pos, int cur_pos);

double evaluateBoard( Item *node );

double evaluateBoardKnights(Item *node);
	
void printBoard( Item *board );
