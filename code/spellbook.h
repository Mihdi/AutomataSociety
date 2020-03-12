#pragma once

#include "misc.h"
#include "ACell.h"
#include "CA.h"

#define GRIMMORY_SIZE 2

BOOL naive(struct CellularAutomaton *conway, struct AutomatonCell *cell);
BOOL negationist(struct CellularAutomaton *conway, struct AutomatonCell *cell);

/*
 * Hey guys! I know that we shouldn't comment what the code does, but I guess it may be needed this time as this is pitch dark magic
 * getSpell takes a size_t and returns a pointer to a function that takes a CAP and a ACP as args and returns a BOOL.
 *
 */
 BOOL(*getSpell(size_t id))(struct CellularAutomaton *, struct AutomatonCell *);