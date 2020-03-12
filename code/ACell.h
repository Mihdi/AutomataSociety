#pragma once

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#include "misc.h"
#include "CA.h"
#include "spellbook.h"

typedef struct AutomatonCell {
	
	UINT *pos;
	UINT nbNeighbours;
	void* mem;
	BOOL(*zeta)(struct AutomatonCell *, struct CellularAutomaton *);
	UINT priority;

} ACell;

#define ACP ACell *

ACP acp(UINT* pos, UINT neighbours, UINT memSize, size_t zeta, UINT priority);