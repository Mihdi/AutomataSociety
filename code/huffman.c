#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "misc.h"
#include "list.h"
#include "sort.h"


const size_t CHAR_BIT_SIZE = pow(2, 7 * sizeof(char)); //hmm the 8th bit seems to be an error flag, so don't you replace the 7 by 8 as everything would crash

typedef struct coupleCharSizeT
{
	char c;
	size_t st;
} CCS;

typedef struct huffmanTree
{
	char c;
	size_t st;
	struct huffmanTree *l;
	struct huffmanTree *r;
} HUFFTREE;

typedef struct coupleHuffTreeSizeT
{
	HUFFTREE *t;
	size_t st;
} CHTST;

CCS *initCCS()
{
	CCS *out = calloc(1, sizeof(CCS));
	if(!out) errx(EXIT_FAILURE, "calloc failed @initCCS");
	return out;
}

CHTST *initCHTST()
{
	CHTST *out = calloc(1, sizeof(CHTST));
	if(!out) errx(EXIT_FAILURE, "calloc failed @initCCS");
	return out;
}

BOOL cmpCCS(void *a, void *b)
{
	CCS *pa = a;
	CCS *pb = b;

	return pa->st <= pb->st;
}

#define IS_THERE_TWO_NODES_IN_QS(f, s, o) do \
{ \
  if ((f)->next != (f)) {	\
  	if ((f)->next->next != (f)){ o = TRUE; break;}\
  	if ((s)->next != (s)){o = TRUE; break;} \
  	o = FALSE; break;\
  }\
  else {\
  	if ((s)->next != (s) && (s)->next->next != (s)){o = TRUE; break;}\
  	o = FALSE; break;\
  }\
} while (0);

LIST *sortCCSByO(char *msg, size_t msgLen, size_t *alphabetSize)
{
	if (alphabetSize) *alphabetSize = 0;

	//init histogram
	CCS *hist[CHAR_BIT_SIZE];
	for (char i = 0; (size_t)i < CHAR_BIT_SIZE; ++i)
	{
		hist[(size_t)i] = initCCS();
		hist[(size_t)i]->c = i;
	}

	//compute occurences
	for (size_t i = 0; i < msgLen; ++i)
	{
		char curr = *(msg + i);
		hist[(size_t)curr]->st += 1;
	}

	//sort by occurences
	introSort((void **) hist, CHAR_BIT_SIZE, cmpCCS);

	//get rid of chars that aren't in the og msg
	LIST *out = initList();
	for (size_t i = 0; i < CHAR_BIT_SIZE; ++i)
	{
		if(hist[i]->st)
		{
			HUFFTREE *curr = calloc(1, sizeof(HUFFTREE));
			curr->c = hist[i]->c;
			curr->st = hist[i]->st;
			add2list(curr, out);

			if(alphabetSize) *alphabetSize += 1;
		}

		free(hist[i]);
	}

	return out;
}

HUFFTREE *buildHuffTree(char *msg, size_t msgLen, size_t *alphabetSize)
{
	LIST *frstQ = sortCCSByO(msg, msgLen, alphabetSize);
	if(frstQ->next == frstQ)
	{
		destroyList(frstQ);
		return NULL;
	}
	LIST *scndQ = initList();

	BOOL pred;
	IS_THERE_TWO_NODES_IN_QS(frstQ, scndQ, pred);
	while (pred)
	{
		HUFFTREE *curr[2];
		//fill curr with the two nodes of the smallest occurences
		for (size_t i = 0; i < 2; ++i)
		{
			HUFFTREE *frstQPeek = frstQ->next != frstQ ? frstQ->next->value : NULL;
			HUFFTREE *scndQPeek = scndQ->next != scndQ ? scndQ->next->value : NULL;

			if (frstQPeek && scndQPeek)
			{
				curr[i] = frstQPeek->st < scndQPeek->st ? dequeue(frstQ) : dequeue(scndQ);
			}
			else if (frstQPeek)
			{
				curr[i] = dequeue(frstQ);
			}
			else
			{
				curr[i] = dequeue(scndQ);
			}
		}

		//create the new intermediary node
		HUFFTREE *neo = calloc(1, sizeof(HUFFTREE));
		if (!neo) errx(EXIT_FAILURE, "calloc failed in buildHuffTree");

		//init the new intermediary node
		neo->st = curr[0]->st + curr[1]->st;
		neo->l = curr[0];
		neo->r = curr[1];

		add2list(neo, scndQ);

		IS_THERE_TWO_NODES_IN_QS(frstQ, scndQ, pred);
	}

	HUFFTREE *out = frstQ->next != frstQ ? frstQ->next->value : scndQ->next->value;

	freeList(frstQ);
	freeList(scndQ);

	return out;
} 

CCS **getSymbolsByBitLength(HUFFTREE *tree, size_t alphaSize)
{
	if(!alphaSize || !tree) return NULL;

	CCS **out = malloc(sizeof(CCS) * alphaSize);
	if(!out) errx(EXIT_FAILURE, "malloc failed @ getSymbolsByBitLength");
	
	LIST *q = initList();

	CHTST *curr = initCHTST();
	curr->t = tree;

	add2list(curr, q);

	size_t counter = 0;
	while(q->next != q)
	{
		CHTST *curr = dequeue(q);
		if (curr->t->c)
		{
			CCS *temp = initCCS();
			temp->c = curr->t->c;
			temp->st = curr->st;
			*(out + counter) =  temp;

			counter++;
		}
		else
		{
			HUFFTREE *temp[2] = {curr->t->l, curr->t->r};
			for (size_t i = 0; i < 2; ++i)
			{
				if(temp[i])
				{
					CHTST *new = initCHTST();
					new->t = temp[i];
					new->st = curr->st + 1;

					add2list(new, q);
				}
			}
		}
		free(curr);
	}
	destroyList(q);

	return out;
}

char *getCanonicalHuffmanCode(char *msg, size_t msgLen)
{
	size_t alphaSize;

	HUFFTREE *tree = buildHuffTree(msg, msgLen, &alphaSize);
	CCS **symbols = getSymbolsByBitLength(tree, alphaSize);
	
	destroyHuffTree(tree);

	short code = 0;
	for (size_t i = 0; i < alphaSize; ++i)
	{
		printf("(%c, %s)\n", *(symbols + i), code);
		free(*(symbols + i));
	}
	free(symbols);
}

void destroyHuffTree(HUFFTREE *tree)
{
	//this is needed not to burden the callstack
	if(tree->l) destroyHuffTree(tree->l);
	if(tree->r) destroyHuffTree(tree->r);

	free(tree);
}

void printHuffTree(HUFFTREE *tree)
{
	if (tree)
	{
		printf("[");

		if (tree->c) printf("%c", tree->c);
		else 
		{
			printf("(%zu)", tree->st );
			//printing left child
			printf(", ");
			printHuffTree(tree->l);

			//printing right child
			printf(", ");
			printHuffTree(tree->r);
		}
		printf("]");
	}
	else printf("NULL");
}


void test_sortCCSbyO()
{
	size_t len;
	LIST *a = sortCCSByO("this is an example of a huffman tree", 36, &len);
	while (a->next != a)
	{
		HUFFTREE *curr = dequeue(a);
		printf("(%c, %zu) \n", curr->c, curr->st);
		free(curr);
	}
	destroyList(a);

	printf("alphabetSize = %zu \n", len);
}

void test_buildHuffmanTree()
{
	size_t len;
	HUFFTREE *a = buildHuffTree("this is an example of a huffman tree", 36, &len);
	printHuffTree(a);
	printf("\n");
	destroyHuffTree(a);
	printf("alphabetSize = %zu \n", len);
}
void test_getSymbolsByBitLength()
{
	size_t len;
	HUFFTREE *t = buildHuffTree("this is an example of a huffman tree", 36, &len);
	CCS **a = getSymbolsByBitLength(t, len);
	for (size_t i = 0; i < len; ++i)
	{
		CCS *curr = *(a + i);
		printf("(%c, %zu)\n", curr->c, curr->st);
		free(curr);
	}
	free(a);
	destroyHuffTree(t);
}
int main()
{
	test_sortCCSbyO();
	test_buildHuffmanTree();
	test_getSymbolsByBitLength();
}