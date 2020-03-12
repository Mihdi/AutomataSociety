#include "spellbook.h"

BOOL naive(struct CellularAutomaton *conway, struct AutomatonCell *cell) { return TRUE; }
BOOL negationist(struct CellularAutomaton *conway, struct AutomatonCell *cell) { return FALSE; }

BOOL(*getSpell(size_t id))(struct CellularAutomaton*, struct AutomatonCell *)
{
	if (id >= GRIMMORY_SIZE) return NULL;

	//more elegant than a global, less bothering than a switch
	const BOOL(*spellbook[GRIMMORY_SIZE])(struct CellularAutomaton*, struct AutomatonCell*) =
	{
		naive,
		negationist
	};

	return spellbook[id];
}