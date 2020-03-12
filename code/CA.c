#include "CA.h"



CAP cap(UINT nbDim, UINT *dimVec, UINT memSize, LISTP cellsByPriority)
{
	CAP output = malloc(sizeof(CA));
	
	if (!output) return NULL;

	output->dimension = nbDim;
	output->dimWidth = dimVec;
	output->memSize = memSize;
	output->cellsByPriority = cellsByPriority;

	return output;
}

int computeIndex(UINT *pos, UINT *dimVec, UINT dimNB) 
{
	if (!dimNB) return -1;

	int output = 0;
	int stride = 1;

	for (int i = 0; i < dimNB; ++i)
	{
		output += *(pos + i) * stride;
		stride *= *(dimVec + i);
	}
	return output;
}

void CA_iterate(CAP conway) 
{
	for (LISTP i = conway->cellsByPriority; i; i = i->next) 
	{
		for (LISTP j = i->itmPtr; j; j = j->next) 
		{
			ACP cell = j->itmPtr;

			int index = computeIndex(cell->pos, conway->dimWidth, conway->dimension);
			conway->values[index] = cell->zeta(conway, cell);
		}
	}
}