#include "list.h"

LIST *initList()
{
	LIST *out = calloc(1, sizeof(LIST));
	if (!out) errx(EX_OSERR, NULL);

	out->next = out;
	out->prev = out;

	return out;
}

size_t listLength(LIST *li)
{
	size_t out = 0;
	for (LIST *p = li->next; p != li; ++out, p = p->next);
	return out;
}

void add2list(void *value, LIST *li)
{
	LIST *new = initList();

	new->value = value;
	new->prev = li->prev;
	li->prev = new;

	new->next = li;
	new->prev->next = new;
}

void *removeElt(LIST *elt)
{
	elt->next->prev = elt->prev;
	elt->prev->next = elt->next;

	void *out = elt->value;
	free(elt);
	return out;
}

void *dequeue(LIST *li) //FIFO
{
	LIST *dequeued = li->next;
	void *out = dequeued->value;

	li->next = dequeued->next;

	li->next->prev = li;

	free(dequeued);
	return out;
}

void *pop(LIST *li) //FILO
{
	LIST *popped = li->prev;
	void *out = popped->value;

	li->prev = popped->prev;

	li->prev->next = li;

	free(popped);
	return out;
}

LIST *find(LIST *li, void *value, BOOL (*cmp) (void*, void *))
{
	for(LIST *out = li->next; out != li; out = out->next)
	{
		switch(cmp(value, out->value)) {
			case 1 :
				return out;
			case -1 : //force end the search to save time
				return NULL;
		}
	}
	return NULL;
}

void insertSorted(void *value, LIST *li, BOOL (*cmp) (void*, void *))
{
	LIST *next = find(li, value, cmp);
	add2list(value, next ? next : li);
}

void printList(LIST *li, void (*printer)(void *))
{
	printf("[S]->");
	for (LIST *p = li->next; p != li; p = p->next)
	{
		printer(p->value);
	}
	printf("[S]\n");
}

void **listToArray(LIST *li, size_t *len)
{
	*len = listLength(li);
	void * *out = malloc(*len * sizeof(void *));

	LIST *p = li->next;
	for (size_t i = 0; i < *len; ++i, p = p->next)
	{
		out[i] = p->value;
	}

	return out;
}

//free the list without freeing its values
void freeList(LIST *li)
{
	LIST *temp;
	for (LIST *p = li->next; p != li; p = temp)
	{
		temp = p->next;
		free(p);
	}
	free(li);
}
void destroyList(LIST *li)
{
	LIST *temp;
	for (LIST *p = li->next; p != li; p = temp)
	{
		temp = p->next;
		free(p->value);
		free(p);
	}
	free(li);
}