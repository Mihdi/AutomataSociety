#include "sort.h"

#define swapPtrsInArray(a, i, j) do{\
void *c = a[i];\
a[i] = a[j];\
a[j] = c;\
}while(0);

size_t partition(void *a[], size_t len, int (*compare)(void*, void*)) 
{
	void **top = calloc(len - 1, sizeof(void *));
	void **bot = calloc(len - 1, sizeof(void *));;
	if (!top || !bot) errx(EX_OSERR, "calloc failed");

	size_t ti = 0, bi = 0;

	for (size_t i = 0; i < len - 1; ++i)
	{
		if ((*compare)(a[i], a[len - 1])) 
		{
			*(top + ti++) = a[i];
		}
		else 
		{
			*(bot + bi++) = a[i];
		}
	}

	for (size_t i = 0; i < ti; ++i)
	{
		a[i] = *(top + i);
	}

	a[ti] = a[len - 1];

	for (size_t i = 0; i < bi; ++i)
	{
		a[ti + i + 1] = *(bot + i);
	}

	free(top);
	free(bot);
	
	return ti;
}

void quickSort(void *a[], size_t len, BOOL (*compare)(void*, void*)) 
{
	if (len <= 1) return;

	size_t q = partition(a, len, compare);

	quickSort(a, q, compare);
	quickSort(a + q + 1, len - q - 1, compare);
}

void heapify(void *a[], size_t len, size_t index, BOOL(*compare)(void*, void*))
{
	size_t root = index;
	size_t marx = 2 * index + 1; //This is really standard so I may as well have fun with the names. 
	size_t smith = 2 * index + 2; //It's the usual left/right, blahblah, if you can read C, you know how to heap sort

	if (marx < len && compare(a[root], a[marx]))
	{
		root = marx;
	}

	if (smith < len && compare(a[root], a[smith]))
	{
		root = smith;
	}

	if (root != index)
	{
		swapPtrsInArray(a, index, root);
		heapify(a, len, root, compare);
	}
}

void heapSort(void *a[], size_t len, BOOL(*compare)(void*, void*))
{
	for (size_t i = len / 2 - 1;/*∞*/; --i)
	{
		heapify(a, len, i, compare);
		
		if (!i) break;
	}

	for (size_t i = len - 1; i > 0 ; --i)
	{
		swapPtrsInArray(a, 0, i);
		heapify(a, i, 0, compare);
	}
}

void linearSort(void *a[], size_t len, BOOL(*compare)(void*, void*))
{
	if (len <= 1) return;

	for (size_t i = 1; i < len; ++i)
	{
		size_t j = i - 1;

		while (!(compare(*(a + j), *(a + j + 1))))
		{
			swapPtrsInArray(a, j , (j + 1));

			if(j == 0) break;
			j--;
		}
	}
}

void __introSort(void* a[], size_t aLen, size_t allowedDepth, BOOL(*compare)(void*, void*))
{
	if (aLen < 2)
	{
		return;
	}
	else if (aLen < 15)
	{
		linearSort(a, aLen, compare);
		return;
	}
	else 
	{
		if (!allowedDepth)
		{
			size_t p = partition(a, aLen, compare);
			__introSort(a, p, allowedDepth - 1, compare);
			__introSort(a + p + 1, aLen - p - 1, allowedDepth - 1, compare);
		}
		else
		{
			heapSort(a, aLen, compare);
			return;
		}
	}
}

void introSort(void* a[], size_t aLen, BOOL(*compare)(void*, void*))
{
	size_t maxDepth = log(aLen) * K_INTROSORT_CONST;
	__introSort(a, aLen, maxDepth, compare);
}