#ifndef FSAUT_H
#define FSAUT_H

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "misc.h"

#define computePos(c, i, j)  (c) * (i) + (j)

typedef struct fsAut
{
	void **alphabet;
	size_t alphabetSize;
	size_t nbStates;
	size_t *transitions; //2D: states * letters
	BOOL *isStateFinal; //1D: just states

} FSAUT;

FSAUT *initFSAut(void *alphabet[], size_t alphaSize, size_t nbStates, size_t *transitions, BOOL *isStateFinal);
size_t getIndexOfLetter(void *letter, void **alphabet, size_t alphaSize, BOOL (*isEq)(void *, void *));
BOOL isInLanguage(void *word[], size_t wLen, FSAUT* language, BOOL (*isEq)(void *, void *));
void destroyFSAut(FSAUT *aut);
#endif