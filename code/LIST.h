#pragma once
#include "misc.h"

typedef struct List {
	void* itmPtr;
	struct List* next;
} LIST;

#define LISTP LIST *