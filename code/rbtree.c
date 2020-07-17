#include "rbtree.h"

RBTREE *initRBTree()
{
	RBTREE *out = calloc(1, sizeof(RBTREE));
	if (!out) errx(EXIT_FAILURE, "initRBTree: calloc failed");

	out->c = BLACK; //according to RBTree's properties

	return out;
}

RBTREE *BSTInsert(void *elem, RBTREE *tree, BOOL (*compare)(void *, void *))
{
	RBTREE *parent = tree->p;

	while (tree->v)
	{
		parent = tree;

		if (compare(elem, tree->v))
		{
			if (!tree->l) tree->l = initRBTree();
			tree = tree->l;
		}
		else
		{
			if (!tree->r) tree->r = initRBTree();
			tree = tree->r;
		}
	}

	tree->v = elem;
	tree->c = RED;
	tree->p = parent;

	return tree;
}

void pivotAsTreeParent(RBTREE *pivot, RBTREE *tree)
{
	//pivot is tree's parent now
	pivot->p = tree->p;
	if (tree->p) 
	{
		if (tree->p->l == tree)
		{
			tree->p->l = pivot;
		}
		else
		{
			tree->p->r = pivot;
		}
	}

	tree->p = pivot;
}

void leftRotate(RBTREE *tree)
{	
	RBTREE *pivot = tree->r;

	//pivot's left sub-tree becomes the tree's right sub-tree
	tree->r = pivot->l;
	if (pivot->l) pivot->l->p = tree;

	pivotAsTreeParent(pivot, tree);

	pivot->l = tree;
	
}

void rightRotate(RBTREE *tree)
{
	RBTREE *pivot = tree->l;

	//pivot's right sub-tree becomes the tree's left sub-tree
	tree->l = pivot->r;
	if (pivot->r)
	{
		pivot->r->p = tree;
	}

	pivotAsTreeParent(pivot, tree);

	pivot->r = tree;
	
}

void swapColorNodes(RBTREE *a, RBTREE *b)
{
	COLOR temp = a->c;
	a->c = b->c;
	b->c = temp;
}

RBTREE *insertInRBTree(void *elem, RBTREE **tree, BOOL (*compare)(void *, void *))
{
	RBTREE *out = BSTInsert(elem, *tree, compare);
	RBTREE *insrtd = out;

	while (TRUE)
	{
		if (insrtd == *tree)
		{
			insrtd->c = BLACK;
			break;
		}
		else if(insrtd->p->c == RED)
		{
			RBTREE *uncle = uncleOf(insrtd);

			if (uncle && uncle->c == RED) //if !uncle, uncle is black
			{
				uncle->c = BLACK;
				insrtd->p->c = BLACK;

				insrtd->p->p->c = RED;

				insrtd = insrtd->p->p;
			}
			else //uncle == BLACK
			{
				RBTREE *gp = insrtd->p->p;
				RBTREE *p = insrtd->p;

				if (gp->l == p) //Left Case
				{
					if (p->r == insrtd) //Left Right Case
					{
						leftRotate(p);
					}

					//the following is done for both the Left Right case and the Left Left case
					rightRotate(gp);
					swapColorNodes(p, gp);
					break;
				}
				else //Right Case
				{
					if (p->l == insrtd) //Right Left Case
					{
						rightRotate(p);
					}

					//the following is done for both the Right Right case and the Right Left case
					leftRotate(gp);
					swapColorNodes(p, gp);
					break;
				}
			}
		}
		else
		{
			break;
		}
	}

	while ((*tree)->p) (*tree) = (*tree)->p;

	return out;
}

// void removeFromRBTree(void *elem, RBTREE *tree, BOOL (*compare)(void *, void *))
// {
	
// }

RBTREE *searchInRBTree(void *value, RBTREE *tree, BOOL (*compare)(void *, void *), BOOL (*isEq)(void *, void *))
{
	RBTREE *out = tree;
	while (out && !isEq(value, out->v))
	{
		out = compare(value, out->v) ? out->l : out->r;
	}
	return out;
}

//this could be done easily with recursion, but I come from PHP so I really don't like stacking too much
// size_t sizeOfRBTree(RBTREE *tree)
// {
// 	if(!tree) return 0;

// 	size_t out = 1;
	
// 	out += sizeOfRBTree(tree->l);
// 	out += sizeOfRBTree(tree->r);

// 	return out;
// }

//anyway, here's an iterative version
size_t sizeOfRBTree(RBTREE *tree)
{
	size_t out = 0;

	LIST *filo = initList();
	if(tree) add2list(tree, filo);

	while (filo->next != filo)
	{
		out++;
		
		RBTREE *curr = pop(filo);

		if(curr->r) add2list(curr->r, filo);
		if(curr->l) add2list(curr->l, filo);
	}

	freeList(filo);

	return out;
}

void preorderDFSRBTree(RBTREE *tree, void (*lambda)(void *, void *), void *args)
{
	LIST *filo = initList();
	if(tree) add2list(tree, filo);

	while (filo->next != filo)
	{
		RBTREE *curr = pop(filo);
		
		lambda(args, curr->v);

		if(curr->r) add2list(curr->r, filo);
		if(curr->l) add2list(curr->l, filo);
	}

	freeList(filo);
}

void inorderDFSRBTree(RBTREE *tree, void (*lambda)(void *, void *), void *args)
{
	LIST *filo = initList();
	RBTREE *curr = tree;

	while (TRUE)
	{
		while (curr)
		{
			add2list(curr, filo);
			curr = curr->l;
		}

		if(filo->next == filo) break;

		curr = pop(filo);
		lambda(args, curr->v);
		curr = curr->r;
	}
	
	freeList(filo);
}

void postorderDFSRBTree(RBTREE *tree, void (*lambda)(void *, void *), void *args)
{
	LIST *masterFilo = initList();
	LIST *filo = initList();

	if(tree) add2list(tree, filo);

	while (filo->next != filo)
	{
		RBTREE *curr = pop(filo);
		
		add2list(curr, masterFilo);

		if(curr->l) add2list(curr->l, filo);
		if(curr->r) add2list(curr->r, filo);
	}

	while (masterFilo->next != masterFilo)
	{
		RBTREE *curr = pop(masterFilo);
		lambda(args, curr->v);
	}

	freeList(filo);
	freeList(masterFilo);
}

struct argsAETA
{
	size_t count;
	void **arr;
};
void addElemToArray(void *args, void *elem)
{
	struct argsAETA *a = args;
	*(a->arr + (a->count)++) = elem;
}
void **RBTreeToArray(RBTREE *tree, size_t *len)
{
	*len = sizeOfRBTree(tree);
	void **out = malloc((*len) * sizeof(void *));

	struct argsAETA args;
	args.count = 0;
	args.arr = out;

	inorderDFSRBTree(tree, addElemToArray, &args);

	return out;
}

void printRBTree(RBTREE *tree, void (*printer)(void *))
{
	if (tree)
	{
		printf("[");

		//printing root
		if(tree->c == RED) printf("\033[0;31m");
		printer(tree->v);
		if(tree->c == RED) printf("\033[0m"); 

		//printing left child
		printf(", ");
		printRBTree(tree->l, printer);

		//printing right child
		printf(", ");
		printRBTree(tree->r, printer);

		printf("]");
	}
	else printf("NULL");
}

void freeRBTree(RBTREE *tree)
{
	RBTREE *tl = tree->l;
	RBTREE *tr = tree->r;

	free(tree);

	if(tl) free(tl);
	if(tr) free(tr);
}
void destroyRBTree(RBTREE *tree)
{
	if (tree->l) destroyRBTree(tree->l);
	if (tree->r) destroyRBTree(tree->r);

	free(tree->v);
	free(tree);
}