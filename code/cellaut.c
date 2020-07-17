#include "cellaut.h"


LIST *sortCells(LIST *rawCells)
{
	size_t len;
	void * *arr = listToArray(rawCells, &len);
	introSort(arr, len, compareCellsByPriority);

	LIST *out = initList();

	//empty list edge case
	if (len < 1) return out;

	//init for the loop
	CELL *currCell = *(arr);
	UINT currPriority = currCell->priority;

	LIST *currPrioList = initList();
	add2list(currPrioList, out);

	for (size_t i = 0; i < len; ++i)
	{
		currCell = *(arr + i);

		//adding new block the out list
		if (currCell->priority != currPriority)
		{
			currPriority = currCell->priority;
			currPrioList = initList();
			add2list(currPrioList, out);
		}

		add2list(currCell, currPrioList);
	}

	free(arr);
	
	return out;
}

CELLAUT *initCellAut(size_t dim, size_t *width, LIST *rawCells)
{
	//allocating mem for CELLAUT
	CELLAUT *out = malloc(sizeof(CELLAUT));
	if (!out) errx(EX_OSERR, NULL);

	//init CELLAUT properties
	out->dim = dim;
	out->width = width;

	//computing memory size of CELLAUT's array
	size_t resultsLen = 1;
	for (size_t i = 0; i < dim; ++i)
	{
		resultsLen *= *(width + i);
	}

	//allocating CELLAUT's array & setting it to UNKNOW val
	out->results = malloc(resultsLen * sizeof(BOOL));
	for (size_t i = 0; i < resultsLen; ++i)
	{
		*(out->results + i) = UNKNOWN;
	}

	//init list of cells, reminder this is truly a list of lists
	out->cells = sortCells(rawCells);

	return out;
}

size_t computeIndex(size_t n, size_t *dim, size_t *pos)
{
	if (n < 2) return 0;
	
	size_t out = 0;
	size_t multiplier = 1;

	size_t i = n - 1;

	while (1)
	{
		out += *(pos + i) * multiplier;
		multiplier *= *(dim + i);

		if (i-- == 0) break;
	}

	return out;
}

void iterateCellAut(CELLAUT *cellaut)
{
	for (LIST *p = cellaut->cells->next; p != cellaut->cells; p = p->next)
	{
		LIST *currPrio = p->value;
		for (LIST *subP = currPrio->next; subP != currPrio; subP = subP->next)
		{
			CELL *currCell = subP->value;
			size_t pos = computeIndex(cellaut->dim, cellaut->width, currCell->pos);
			
			*(cellaut->results + pos)= currCell->zeta(currCell, cellaut);
		}
	}
}

void *pthread_iterateOverList(void *arg)
{
	struct pthread_iOLArgs *a = arg;

	LIST *currPrio = a->currPrio;
	CELLAUT *cellaut = a->cellaut;

	for (LIST *subP = currPrio->next; subP != currPrio; subP = subP->next)
	{
		CELL *currCell = subP->value;
		size_t pos = computeIndex(cellaut->dim, cellaut->width, currCell->pos);
		
		*(cellaut->results + pos)= currCell->zeta(currCell, cellaut);
	}

	freeList(a->currPrio);

	pthread_exit(NULL);
}


void pthreads_iterateCellAut(CELLAUT *cellaut, const size_t nbThreads)
{
	for (LIST *p = cellaut->cells->next; p != cellaut->cells; p = p->next)
	{
		//init the taskLists
		struct pthread_iOLArgs taskList[nbThreads];
		for (size_t i = 0; i < nbThreads; ++i)
		{
			taskList[i].currPrio = initList();
			taskList[i].cellaut = cellaut;
		}

		//dividing the tasks among the threads
		size_t counter = 0;
		LIST *currPrio = p->value;
		for (LIST *subP = currPrio->next; subP != currPrio; counter++, subP = subP->next)
		{
			add2list(subP->value, taskList[counter % nbThreads].currPrio);
		}

		//executing the threads
		pthread_t ptid[nbThreads];
		for (size_t i = 0; i < nbThreads; ++i)
		{
			int e = pthread_create((ptid + i), NULL, pthread_iterateOverList, (taskList + i));
			if(e) errx(EXIT_FAILURE, "couldn't create pthread");	
		}

		//wait for the threads
		for (size_t i = 0; i < nbThreads; ++i)
		{
			pthread_join(ptid[i], NULL);
		}
	}
}

void destroyCellAut(CELLAUT *ca)
{
	
	LIST *temp;
	for (LIST *p = ca->cells->next; p != ca->cells; p = temp)
	{
		temp = p->next;
		LIST *subP = p->value;
		destroyCellList(subP);
		free(p);
	}
	free(ca->cells);

	free(ca->width);
	free(ca->results);
	free(ca);
}