#include "fsaut.h"


FSAUT *initFSAut(void *alphabet[], size_t alphaSize, size_t nbStates, size_t *transitions, BOOL *isStateFinal)
{
	FSAUT *out = malloc(sizeof(FSAUT));
	if(!out) errx(EXIT_FAILURE, "malloc failed @ initFSAut");

	out->alphabet = alphabet;
	out->alphabetSize = alphaSize;
	out->nbStates = nbStates;
	out->transitions = transitions;
	out->isStateFinal = isStateFinal;

	return out;
}

size_t getIndexOfLetter(void* letter, void** alphabet, size_t alphaSize, BOOL (*isEq)(void*, void*))
{
	//TODO: replace this so it works with a hashmap, moving from O(n) to O(1)
	size_t out = 0;
	for (; !isEq(*(alphabet + out), letter) && out < alphaSize; out++);
	return out;
}

BOOL isInLanguage(void* word[], size_t wLen, FSAUT* language, BOOL (*isEq)(void*, void*))
{
	size_t currState = 0;
	for (size_t i = 0; i < wLen; i++)
	{
		size_t letterIndex = getIndexOfLetter(word[i], language->alphabet, language->alphabetSize, isEq);
		currState = *(language->transitions + computePos(language->alphabetSize, currState, letterIndex));
		if (currState == language->nbStates) return FALSE; //sinking state
	}

	return *(language->isStateFinal + currState);
}

void destroyFSAut(FSAUT *aut)
{
	for (size_t i = 0; i < aut->alphabetSize; ++i)
	{
		free(*(aut->alphabet + i));
	}

	free(aut->alphabet);
	free(aut->transitions);
	free(aut->isStateFinal);
	
	free(aut);
}