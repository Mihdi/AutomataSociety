#include "cell.h"


CELL *initCell(size_t *pos, UINT nbNeigh, size_t memSize, UINT priority, BOOL (*zeta)(CELL *cell, CELLAUT *ca))
{
	CELL *out = malloc(sizeof(CELL));
	if (!out) errx(EX_OSERR, NULL);

	out->pos = pos;
	out->nbNeigh = nbNeigh;

	void *mem = malloc(sizeof(memSize));
	if (!mem) errx(EX_OSERR, NULL);
	out->mem = mem;
	
	out->priority = priority;
	out->zeta = zeta;

	return out;
}


BOOL compareCellsByPriority(void *a, void *b)
{
	CELL *ap = a;
	CELL *bp = b;

	return ap->priority <= bp->priority;
}

void printCell(CELL *cell)
{
	printf("cell: %p\n", cell);
	printf("pos: %p\n", cell->pos);
	printf("neigh: %d\n", cell->nbNeigh);
	printf("mem: %p\n", cell->mem);
	printf("priority: %d\n", cell->priority);
	printf("zeta: %p\n\n", cell->zeta);
}

void destroyCell(CELL *cell)
{
	free(cell->pos);
	free(cell->mem);
	free(cell);
}

//destroys a list of cells
void destroyCellList(LIST *cells)
{
	LIST *temp;
	for (LIST *p = cells->next; p != cells; p = temp)
	{
		temp = p->next;
		CELL *toBeRekt = p->value;

		destroyCell(toBeRekt);
		free(p);
	}
	free(cells);
}