#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include "misc.h"
#include "cell.h"
#include "cellaut.h"

#define NB_SPELLS 2

BOOL (*drawSpell(size_t i))(CELL *, CELLAUT *);

#endif //SPELLBOOK_H