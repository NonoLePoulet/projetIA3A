#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#include "list.h"
#include "board.h"


list_t openList_p;
list_t closedList_p;


void showSolution( Item *goal )
{
  int i = 0;

  printf("\nSolution:");

  while (goal) {
    printBoard( goal );
    goal = goal->parent;
    i++;
  }

  printf("\nLength of the solution = %d\n", i-1);
  printf("Size of open list = %d\n", openList_p.numElements);
  printf("Size of closed list = %d\n", closedList_p.numElements);
  return;
}


void bfs( void )
{
  Item *cur_node, *child_p;
  int cur_pos = 0;
  int i;

  while ( listCount(&openList_p) != 0 ) { /* While items are on the open list */
   	
    /* Get the first item on the open list */
    cur_node = popFirst(&openList_p);
		
    printf("%d  %f\n", listCount(&openList_p), evaluateBoardKnights( cur_node ));

    /* Add it to the "visited" list */

    /* Do we have a solution? */
    if ( evaluateBoardKnights(cur_node) == 0.0 ) {
      showSolution(cur_node);
      return;

    } 

    else {
      /* Enumerate adjacent states */
        for (i = 0; i < MAX_BOARD; i++) {
            child_p = getChildBoardKnights( cur_node, i, cur_pos);
   			
            if (child_p != NULL) { // it's a valid child!

                  cur_pos = i;
	  			        addLast( &openList_p, child_p );
                
            }
        }
    }
  }

  return;
}

void dfs()
{
  Item *cur_node, *child_p, *temp;
  int cur_pos = 0;
  int i;

  while ( listCount(&openList_p) != 0 ) { /* While items are on the open list */
   	
    /* Get the first item on the open list */
    cur_node = popFirst(&openList_p);
		
    printf("%d  %f\n", listCount(&openList_p), evaluateBoardKnights( cur_node ));


    /* Do we have a solution? */
    if ( evaluateBoardKnights(cur_node) == 0.0 ) {
      showSolution(cur_node);
      return;

    } 

    else {
      /* Enumerate adjacent states */
        for (int i = 0; i < MAX_BOARD; i++) {
            child_p = getChildBoardKnights( cur_node, i, cur_pos);
   			
            if (child_p != NULL) { // it's a valid child!
              
                cur_pos = i;
	  			      addFirst( &openList_p, child_p );
            }
        }
    }
  }

  return;
}

int main()
{
	/* init lists */
  initList(&openList_p);
  initList(&closedList_p);

	
  printf("\nInitial:");
  Item *initial_state = initGameKnights();
  printBoard( initial_state );

  printf("\nSearching ...\n");
  
  addLast( &openList_p, initial_state );

  bfs();
	printf("Finished!\n");
  
	/* clean lists */
	cleanupList( &openList_p );
  cleanupList( &closedList_p );
	
  return 0;
}
