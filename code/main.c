#include "list.h"
#include "sort.h"
#include "spellbook.h"
#include "misc.h"
#include "networking.h"
#include "fsaut.h"
#include "rbtree.h"

#define MAX_NB_THREADS 4
#define ALPHASIZE 2
#define NBSTATE 3

int *putIntOnHeap(int x)
{
	int *out = malloc(sizeof(int));
	if (!out) errx(EX_OSERR, NULL);

	*out = x;
	return out;
}

void printInt(void *integer)
{
	int *intPtr = integer;
	printf("[%d]->", *intPtr);
}

BOOL compareInt(void *a, void *b)
{
	int *pa = a;
	int *pb = b;

	return *pa <= *pb;
}

char *putCharOnHeap(char c)
{
	char *out = malloc(sizeof(char));
	*out = c;
	return out;
}

BOOL cmpChar(void *a, void *b)
{
	char *pa = a;
	char *pb = b;

	return *pa <= *pb;
}

int isEqChar(void *a, void *b)
{
	char *pa = a;
	char *pb = b;

	return *pa == *pb;
}

void printChar(void *c)
{
	char *pc = c;
	printf("%c",*pc);
}

void printArray(void **arr, size_t len, void (*printer)(void *))
{
	for (size_t i = 0; i < len; ++i)
	{
		printer(*(arr + i));
	}
	printf("\n");
}


void testListNIntroSort()
{
	int a[10]= {0,1,2,3,4,5,6,7,8,9};
	LIST *li = initList();
	printf("list size: %zu\n", listLength(li) );
	for (int i = 0; i < 10; ++i)
	{
		add2list(putIntOnHeap(a[10 - i - 1]), li);
	}
	printList(li, printInt);
	printf("list size: %zu\n", listLength(li) );
	

	size_t len; 
	void **arr = listToArray(li, &len);

	printArray(arr, len, printInt);
	introSort(arr, len, compareInt);
	printArray(arr, len, printInt);

	destroyList(li);
	free(arr);
}

void testComputeIndex()
{
	size_t n = 4;
	size_t dim[4] = {5, 4, 3, 2}; //hyperdepth, depth, row, col

	size_t m[120];
	for (size_t i = 0; i < 120; ++i)
	{
		m[i] = i;
	}
	
	printf("\n");

	for (size_t i = 0; i < dim[0]; ++i)
	{
		for (size_t j = 0; j < dim[1]; ++j)
		{
			for (size_t k = 0; k < dim[2]; ++k)
			{
				for (size_t l = 0; l < dim[3]; ++l)	
				{

					size_t pos[4];
					
					pos[0] = i;
					pos[1] = j;
					pos[2] = k;
					pos[3] = l;

					printf("%zu ", m[computeIndex(n, dim, pos)] );
				}
				printf("\n");
			}
			printf("\n");
		}
		printf("\n");
	}
	printf("\n");
}

void theBigTest(BOOL isMultiThreaded)
{
	size_t *pos = malloc(sizeof(size_t));
	UINT nbNeigh = 1;
	size_t memSize = 1;
	UINT priority = 0;

	CELL *c = initCell(pos, nbNeigh, memSize, priority, drawSpell(0));
	printCell(c);
	destroyCell(c);

	LIST *li = initList();
	CELL *c2;
	for (int i = 0; i < 10; ++i)
	{
		pos = malloc(sizeof(size_t));

		c2 = initCell(pos, nbNeigh, memSize, priority, drawSpell(0));
		add2list(c2, li);
	}

	for (LIST *p = li->next; li != p; p = p->next)
	{
		CELL *cell = p->value;
		printCell(cell);
	}

	destroyCellList(li);

	size_t dim = 2;
	size_t *width = malloc(sizeof(size_t) * 2);
	*width = 3;
	*(width + 1) = 3;

	LIST *rawCells = initList();

	for (size_t i = 0; i < width[0]; ++i)
	{
		for (size_t j = 0; j < width[1]; ++j)
		{
			pos = malloc(sizeof(size_t) * 2);
			*pos = i;
			*(pos + 1) = j;

			CELL *c3 = initCell(pos, nbNeigh, memSize, priority, drawSpell(0));
			add2list(c3, rawCells);
		}
	}

	CELLAUT *ca = initCellAut(dim, width, rawCells);
	freeList(rawCells);

	for (size_t i = 0; i < width[0]; ++i)
	{
		for (size_t j = 0; j < width[1]; ++j)
		{
			printf("%d\n", ca->results[width[0] * j + i]);
		}
	}

	if(isMultiThreaded)
	{
		pthreads_iterateCellAut(ca, MAX_NB_THREADS);
	}
	else
	{
		iterateCellAut(ca);
	}

	for (size_t i = 0; i < width[0]; ++i)
	{
		for (size_t j = 0; j < width[1]; ++j)
		{
			printf("%d\n", ca->results[width[0] * j + i]);
		}
	}

	destroyCellAut(ca);
}

void testServ(int listenSock __attribute__((unused)), int interactSock)
{
	write(interactSock, "HTTP/1.1 404 Not Found\r\n\r\n<Not all who wander are lost...>", 58);
	printf("hi scrubs\n");
}

void testFSAUT()
{

	//init the alphabet
	void **alphabet = malloc(sizeof(char *) * ALPHASIZE);
	for (char i = 0; i < ALPHASIZE; i++)
	{
		alphabet[(size_t)i] = putCharOnHeap('a' + i);
	}
	
	size_t *transitions = malloc(sizeof(size_t) * ALPHASIZE * NBSTATE);

	*(transitions + computePos(ALPHASIZE, 0, 0)) = 1;
	*(transitions + computePos(ALPHASIZE, 0, 1)) = NBSTATE;

	*(transitions + computePos(ALPHASIZE, 1, 0)) = NBSTATE;
	*(transitions + computePos(ALPHASIZE, 1, 1)) = 2;

	*(transitions + computePos(ALPHASIZE, 2, 0)) = 2;
	*(transitions + computePos(ALPHASIZE, 2, 1)) = 2;


	//init list of final states
	BOOL *isStateFinal = malloc(sizeof(int) * 3);

	*(isStateFinal) = 0;
	*(isStateFinal + 1) = 0;
	*(isStateFinal + 2) = 1;


	FSAUT *aut = initFSAut(alphabet, ALPHASIZE, NBSTATE, transitions, isStateFinal);

	void *w[3];
	
	w[0] = putCharOnHeap('b');
	w[1] = putCharOnHeap('b');
	w[2] = putCharOnHeap('a');

	int isWord = isInLanguage(w, 3, aut, isEqChar);
	printf("aba is %sword.\n", isWord ? "" : "not ");

	for (int i = 0; i < 3; ++i)
	{
		free(w[i]);
	}

	destroyFSAut(aut);
}

void treePrintCountAndValue(void *foo, void *treeVal)
{
	size_t *count = foo;
	char *tVal = treeVal;
	printf("val: %c \t count: %zu\n", *tVal, (*count)++);
}
void testRBTree()
{
	RBTREE *t = initRBTree();
	
	char *toBeInserted = "2145a9367";

	for (char *p = toBeInserted; *p; ++p)
	{
		printf("inserting %c: \t", *p );
		insertInRBTree(putCharOnHeap(*p), &t, cmpChar);
		printRBTree(t, printChar);
		printf("size of tree is %zu\n", sizeOfRBTree(t));
		printf("\n");
	}
	size_t a = 0;

	printf("preorder:\n");
	preorderDFSRBTree(t, treePrintCountAndValue ,&a);

	a = 0;
	printf("inorder:\n");
	inorderDFSRBTree(t, treePrintCountAndValue, &a);

	a = 0;
	printf("postorder:\n");
	postorderDFSRBTree(t, treePrintCountAndValue, &a);

	printf("\nlist in order:\t");
	size_t len;
	void **treeArr = RBTreeToArray(t, &len);
	for (size_t i = 0; i < len; ++i)
	{
		printChar(*(treeArr + i));
	}
	free(treeArr);
	printf("\n");

	toBeInserted = "2145a9367zc";
	for (char *p = toBeInserted; *p; ++p)
	{
		char *temp = putCharOnHeap(*p);
		RBTREE *b = searchInRBTree(temp, t, cmpChar, isEqChar);
		free(temp);
		if(!b) printf("not found\n");
		else {
			printf("found it:\t");
			char *foo = b->v;
			printChar(foo);
			printf("\n");
		}
	}

	destroyRBTree(t);
}
int main()
{
	theBigTest(TRUE);
	testFSAUT();
	//servTCPIPAgnostic(testServ);
	testRBTree();
}