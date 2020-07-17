#include "spellbook.h"

BOOL candide(CELL *cell __attribute__((unused)), CELLAUT *ca __attribute__((unused)))
{
	return TRUE;
}

BOOL agrippa(CELL *cell __attribute__((unused)), CELLAUT *ca __attribute__((unused)))
{
	return FALSE;
}

/*
 * I know I don't have to do this and I could decl & assign spellbook as a glob const, 
 * but I feel like this is cleaner, and at least this way I can read the indexes of each
 * zeta-function in a glance. It's all O(1) anyway, and this only matters for the init phase
 */
BOOL (*drawSpell(size_t i))(CELL *, CELLAUT *)
{
	BOOL (*spellbook[NB_SPELLS])(CELL *, CELLAUT *);

	spellbook[0] = candide; 
	spellbook[1] = agrippa;

	return *(spellbook + i);
}