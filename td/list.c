#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* Item Functions */

Item *nodeAlloc()
{
  Item *node;

  node = (Item *) malloc( sizeof(Item) );
  assert(node);

  node->board = NULL;
  node->parent = NULL;
  node->prev = NULL;
  node->next = NULL;
  node->f = node->g = node->h = (double)0.0;

  return node;
}

void freeItem( Item *node )
{
  if (node && node->board) free(node->board);
	if (node) free(node);
}


void initList( list_t *list_p )
{
  //if (list_p == NULL) list_p = malloc(sizeof(list_t));
	assert(list_p);

  list_p->numElements = 0;
  list_p->first = NULL;
  list_p->last = NULL;
}

int listCount( list_t *list )
{
	return list->numElements;
}

// return an item with corresponding board, or null
Item* onList( list_t *list, char *board)
{
  Item* a = list->first;
  while (a != NULL){
    if (memcmp(a->board, board, a->size) == 0)
    { 
      return a;
    }
    a = a->next;
  }
  return NULL;
}

// return and remove first item
Item *popFirst( list_t *list ) //
{
  if (list->numElements==0)
  {
    return NULL;
  }
  
	Item *item;
  item = list->first;
  list->first = item->next;

  if (list->first !=NULL)
  {
    list->first->prev == NULL;
  }
  else
  {
    list->last == NULL;
  }
  

  list->numElements--;
	return item;
}

// return and remove last item
Item *popLast( list_t *list ) //
{
  if (list->numElements==0)
  {
    return NULL;
  }
	Item *item;
  item = list->last;
  list->last = item->prev;

  if (list->last != NULL)
  {
    list->last->next = NULL;
  }
  else
  {
    list->first = NULL;
  }

  list->numElements--;
  return item;
}

// remove a node from list
void delList( list_t *list, Item *node) {
  if (list == NULL || node == NULL)
  {
    return;
  }
  
  if (node->prev != NULL) {
    node->prev->next = node->next;
  } else {
    list->first = node->next;
  }
  
  if (node->next != NULL) {
    node->next->prev = node->prev;
  } else {
    list->last = node->prev; 
  }

  list->numElements--;
  freeItem(node);
}

// return and remove best item with minimal f value
Item *popBest( list_t *list ) // and remove the best board from the list.
{
  if (list == NULL)
  {
    return NULL;
  }

  Item *best = list->first;
  Item *actuel = list->first;

  while (actuel != NULL)
  {
    if (best->f > actuel->f)
    {
      best = actuel;
    }
    
    actuel = actuel->next;
    
  }
  
  if (best == list->first)
  {
    return popFirst(list);
  }
  else if (best == list->last)
  {
    return popLast(list);
  }
  
  best->prev->next = best->next;
  best->next->prev = best->prev;

  best->prev = NULL;
  best->next = NULL;
  list->numElements --;

  return best;
}

 // add item in top
void addFirst( list_t *list, Item *node ) 
{
  node->prev = NULL; 
  node->next = list->first;

  if (list->numElements == 0) {
    list->last = node; // Si la liste est vide, le noeud devient le dernier
  } else {
    list->first->prev = node; // Sinon, on l'attache à l'ancien dernier
  }
  
  list->first = node;
  list->numElements++;
}

 // add item in queue
void addLast( list_t *list, Item *node ) 
{
  node->next = NULL; 
  node->prev = list->last;

  if (list->numElements == 0) {
    list->first = node; // Si la liste est vide, le noeud devient le premier
  } else {
    list->last->next = node; // Sinon, on l'attache à l'ancien dernier
  }
  
  list->last = node;
  list->numElements++;
}

void cleanupList( list_t *list )
{
  int N = list->numElements;
  for(int i=0;i<N;i++){
    freeItem(popFirst(list));
  }
	
}

void printList( list_t list ) {
  Item *item = list.first;
  while (item) {
    printf("%.2f [%s] - ", item->f, item->board);
    item = item->next;
  }
  printf(" (nb_items: %d)\n", list.numElements);
}



// TEST LIST

// int main()
// {
//   Item *item, *node;
//   char str[3];

//   list_t openList;
  
//   initList( &openList );

//   for (int i=0; i<10; i++) {
//     item = nodeAlloc();
//     item->f = i;
//     sprintf(str, "%2d", i);
//     item->board = strdup(str);
//     item->size = 3;
//     addLast( &openList, item );
//   }

//     printList(openList);
//     printf("\n");
    
//   for (int i=20; i<25; i++) {
//     item = nodeAlloc();
//     item->f = i;
//     item->size = 3;
//     sprintf(str, "%2d", i);
//     item->board = strdup(str);
//     addFirst( &openList, item );
//   }	

//   printList(openList);
//   printf("\n");

//     node = popFirst(&openList);
//     if (node) printf("first: %.2f\n", node->f);
//     printList(openList);
//     printf("\n");

//     node = popLast(&openList);
//     if (node) printf("last: %.2f\n", node->f);
//     printList(openList);
//     printf("\n");
    
//   node = popBest( &openList );
//   printf("best node = %.2f\n", node->f);
//   printList(openList);
//   printf("\n");

//   strcpy(str, "23");
//   node = onList(&openList, str);
//   if (node) printf("found %s: %.2f!\n", str, node->f); 
//   printList(openList);
//   printf("\n");

//   node = popFirst(&openList);
	
//   item = nodeAlloc();
//   item->f = 50;
//   item->size = 3;
//   sprintf(str, "50");
//   item->board = strdup(str);
//   addLast( &openList, item );

  

//   printf("clean\n");	
//   cleanupList(&openList);
//   printList(openList);
//   printf("\n");
  
//   return 0;
// }
