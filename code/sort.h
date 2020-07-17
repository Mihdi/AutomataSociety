#include <stdlib.h>
#include <math.h>
#include <err.h>

#include "misc.h"


#ifndef SORT_H
#define SORT_H

#ifndef K_INTROSORT_CONST
#define K_INTROSORT_CONST 2
#endif

/*
 * for all generic sorts in this file:
 *  comparison must return TRUE if the two elements are in order, false otherwise.
 */
void linearSort(void* a[], size_t len, BOOL(*compare)(void*, void*));
void quickSort(void* a[], size_t len, BOOL (*compare)(void*, void*));
void heapSort(void* a[], size_t len, BOOL(*compare)(void*, void*));
void introSort(void* a[], size_t aLen, BOOL(*compare)(void*, void*));
#endif // SORT_H