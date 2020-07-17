#ifndef CELL_H
#define CELL_H

#include "misc.h"
#include "sort.h"
#include "cellaut.h"
#include <stdlib.h>
#include <err.h>

//forward declaration, this is defined in cellaut.h
typedef struct cellAut CELLAUT;

typedef struct cell
{
	size_t *pos;
	UINT nbNeigh;
	void *mem;
	UINT priority;
	BOOL (*zeta)(struct cell *cell, struct cellAut *ca);
} CELL;

BOOL compareCellsByPriority(void *a, void *b);

CELL *initCell(size_t *pos, UINT nbNeigh, size_t memSize, 
	UINT priority, BOOL (*zeta)(CELL *cell, CELLAUT *ca));

void printCell(CELL *cell);
void destroyCell(CELL *cell);
void destroyCellList(LIST *cells);
#endif // CELL_H