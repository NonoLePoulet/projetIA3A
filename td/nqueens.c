//c) On peut voir que si on lance le programme avec bfs (donc le parcours en largeur) pour seulement 6 reines on obtient:
// Length of the solution = 6
// Size of open list = 2879
// Size of closed list = 60190
// Finished!

// Alors que si on le lance avec dfs (parcours en profondeur) pour seulement 6 reines on obtient:
// Length of the solution = 6
// Size of open list = 68
// Size of closed list = 251
// Finished!

//On peut donc en conclure que pour le cas du problème des 8 reines, l'écart sera encore plus énorme entre les deux fonctions, le parcours en profondeur est beaucoup plus optimisé

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
  int i = 0, j;

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
  Item *cur_node, *child_p, *temp;
  int i;

  while ( listCount(&openList_p) != 0 ) { /* While items are on the open list */
   	
    /* Get the first item on the open list */
    cur_node = popFirst(&openList_p);
		
    printf("%d  %f\n", listCount(&openList_p), evaluateBoard( cur_node ));

    /* Add it to the "visited" list */
    addLast(&closedList_p, cur_node);

    /* Do we have a solution? */
    if ( evaluateBoard(cur_node) == 0.0 ) {
      showSolution(cur_node);
      return;

    } 

    else {
      /* Enumerate adjacent states */
        for (int i = 0; i < MAX_BOARD; i++) {
            child_p = getChildBoard( cur_node, i );
   			
            if (child_p != NULL) { // it's a valid child!
					
                /* Ignore this child if already visited */
                
                if (onList(&closedList_p, child_p->board))
                {
                  freeItem(child_p);
                }
                else
                {
                  /* Add child node to openList */
	  			        addLast( &openList_p, child_p );
                }
            }
        }
    }
  }

  return;
}

void dfs()
{
  Item *cur_node, *child_p, *temp;
  int i;

  while ( listCount(&openList_p) != 0 ) { /* While items are on the open list */
   	
    /* Get the first item on the open list */
    cur_node = popFirst(&openList_p);
		
    printf("%d  %f\n", listCount(&openList_p), evaluateBoard( cur_node ));

    /* Add it to the "visited" list */
    addLast(&closedList_p, cur_node);

    /* Do we have a solution? */
    if ( evaluateBoard(cur_node) == 0.0 ) {
      showSolution(cur_node);
      return;

    } 

    else {
      /* Enumerate adjacent states */
        for (int i = 0; i < MAX_BOARD; i++) {
            child_p = getChildBoard( cur_node, i );
   			
            if (child_p != NULL) { // it's a valid child!
					
                /* Ignore this child if already visited */
                
                if (onList(&closedList_p, child_p->board))
                {
                  freeItem(child_p);
                }
                else
                {
                  /* Add child node to openList */
	  			        addFirst( &openList_p, child_p );
                }
            }
        }
    }
  }

  return;
}

void ucs()
{
  Item *cur_node, *child_p, *temp;
  int i;

  while ( listCount(&openList_p) != 0 ) { /* While items are on the open list */
   	
    /* Get the best item on the open list */
    cur_node = popBest(&openList_p);
		
    printf("%d  %f\n", listCount(&openList_p), evaluateBoard( cur_node ));

    /* Add it to the "visited" list */
    addLast(&closedList_p, cur_node);

    /* Do we have a solution? */
    if ( evaluateBoard(cur_node) == 0.0 ) {
      showSolution(cur_node);
      return;

    } 

    else {
      /* Enumerate adjacent states */
        for (int i = 0; i < MAX_BOARD; i++) {
            child_p = getChildBoard( cur_node, i );

            if (child_p != NULL)
            {
              child_p->f = child_p->depth;
            }

            if (child_p != NULL) { // it's a valid child!
					
              temp = onList(&openList_p, child_p->board);

                /* Ignore this child if already visited */
                
                if (onList(&closedList_p, child_p->board))
                {
                  freeItem(child_p);
                }

                else if (temp == NULL)
                {
                  /* Add child node to openList */
	  			        addFirst( &openList_p, child_p );
                }

                else if (temp->f > child_p->f)
                {
                  delList(&openList_p, temp);
                  addFirst(&openList_p, child_p);
                  
                }
                
                else
                {
                  freeItem(child_p);
                }
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
  Item *initial_state = initGame();
  printBoard( initial_state );

  printf("\nSearching ...\n");
  
  addLast( &openList_p, initial_state );

  ucs();
	printf("Finished!\n");
  
	/* clean lists */
	cleanupList( &openList_p );
  cleanupList( &closedList_p );
	
  return 0;
}
