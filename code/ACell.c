#include "ACell.h"

ACP acp(UINT* pos, UINT neighbours, UINT memSize, size_t zeta, UINT priority)
{
	ACP output = malloc(sizeof(ACell));

	if (!output) return NULL;

	void* mem = calloc(1, sizeof(BYTE) * memSize);
	if (!mem) return NULL;

	output->mem = mem;
	output->nbNeighbours = neighbours;
	output->pos = pos;
	output->priority = priority;

	//loading zetas
	output->zeta = getSpell(zeta);

	return output;
}

/*
	UINT *pos;
	UINT nbNeighbours;
	void* mem;
	BOOL(*zeta)(struct AutomatonCell *, CAP);
	UINT priority;
*/