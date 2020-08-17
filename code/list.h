#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "misc.h"


#ifndef LIST_H
#define LIST_H

#ifndef EX_OSERR
#define EX_OSERR 1
#endif

typedef struct list
{
	void *value;
	struct list *next;
	struct list *prev;
} LIST;

LIST *initList();

//returns size of list not including sentinel
size_t listLength(LIST *li);

void add2list( void *value, LIST *li);

//add value before the smallest greater elt to keep list sorted
void insertSorted(void *value, LIST *li, BOOL (*cmp) (void*, void *));
//remove a given element from its list
void *removeElt(LIST* elt);

void *dequeue(LIST *li); //FIFO
void *pop(LIST *li); //FILO

//find first val in list that matches cmp predicate
LIST *find(LIST *li, void *val, BOOL (*cmp) (void*, void *));

void printList(LIST *li, void (*printer)(void *));

//returns an array containing the elements of li, and fills len with the appropriate length;
void **listToArray(LIST *li, size_t *len);

//free the list without freeing its values
void freeList(LIST *li);

//free everything in the list, including its contained values
void destroyList(LIST *li);
#endif // LIST_H