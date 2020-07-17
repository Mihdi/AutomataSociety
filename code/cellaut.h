#ifndef CELLAUT_H
#define CELLAUT_H

#include <stdlib.h>
#include <err.h>
#include <pthread.h>

#include "misc.h"
#include "list.h"
#include "sort.h"
#include "cell.h"

//foward declaration, this is defined in cell.h
typedef struct cell CELL;

typedef struct cellAut
{
	size_t dim; //number of dimension, so this is the length of width[]
	size_t *width; //the ith element is the size of ith dimension
	BOOL *results; //array that stores the answers of the cell for a given iteration
	size_t memSize; //defines the size of allocated memory per cell
	LIST *cells; //the cells are stored in lists of equal priority, so ->cells is a list of lists
} CELLAUT;

struct pthread_iOLArgs
{
	LIST *currPrio;
	CELLAUT *cellaut;
};

/*
 * free rawCells after use if you don't need it anymore. Be careful not to free the cells though.
 * Is this an impractical design? TODO: think of dis
 */
CELLAUT *initCellAut(size_t dim, size_t *width, LIST *rawCells);

void iterateCellAut(CELLAUT *cellaut);
void pthreads_iterateCellAut(CELLAUT *cellaut, const size_t nbThreads);

size_t computeIndex(size_t n, size_t *dim, size_t *pos);
void destroyCellAut(CELLAUT *cellaut);

#endif // CELLAUT_H