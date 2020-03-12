#pragma once

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#include "ACell.h"
#include "misc.h"
#include "LIST.h"


typedef struct CellularAutomaton {
	UINT dimension;
	UINT *dimWidth;
	UINT memSize;
	LISTP cellsByPriority; //List of Lists of cells
	BOOL values[];
} CA;
#define CAP struct CellularAutomaton *

CAP cap(UINT nbDim, UINT* dimVec, UINT memSize, LISTP cellsByPriority);
int computeIndex(UINT* pos, UINT* dimVec, UINT dimNB);
void CA_iterate(CAP conway);
