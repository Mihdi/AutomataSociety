#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "misc.h"
#include "list.h"

typedef enum {RED, BLACK} COLOR;

/*
	Here are the 4 properties of redBlackTrees:
		+	root is always black
		+	reds always have black children
		+	external leaves are always black
		+	all external leaves have equal black depth
	Of course, on top of those properties, RBTREEs are Binary Search Trees (BSTs)
 */
typedef struct redBlackTree
{
	void *v; //value
	COLOR c; //colour

	struct redBlackTree *p; //parent

	struct redBlackTree *l; //left
	struct redBlackTree *r; //right
} RBTREE;

//clean code? I guess I could do that, but.. I wanna have some fun writing a spaghetti macro for once
//a cleaner version is available at the bottom of this file, though. I'm not a monster.
#define uncleOf(t) ((t)->p && (t)->p->p ? ((t)->p->p->l == (t)->p ? (t)->p->p->r : (t)->p->p->l) : NULL);

//returns a new empty tree
RBTREE *initRBTree();

//pretty straight forward name: inserts elem in tree given a compare function to determine the order relation
RBTREE *insertInRBTree(void *elem, RBTREE **tree, BOOL (*compare)(void *, void *));

//if successful, returns the node containing value; returns null if not found
RBTREE *searchInRBTree(void *value, RBTREE *tree, BOOL (*compare)(void *, void *), BOOL (*isEq)(void *, void *));

//returns the size of the RBTree
size_t sizeOfRBTree(RBTREE *tree);

//DFS traversal of the tree in preorder, that is <node, left, right>
void preorderDFSRBTree(RBTREE *tree, void (*lambda)(void *, void *), void *args);

//DFS traversal of the tree in inorder, that is <left, node, right>
void inorderDFSRBTree(RBTREE *tree, void (*lambda)(void *, void *), void *args);

//DFS traversal of the tree in postorder, that is <left, right, node>
void postorderDFSRBTree(RBTREE *tree, void (*lambda)(void *, void *), void *args);

//returns a sorted array of the elements contained by the tree
void **RBTreeToArray(RBTREE *tree, size_t *len);

//prints the RBTree, the printer has to be for the datatype stored
void printRBTree(RBTREE *tree, void (*printer)(void *));

//free the tree without freeing its values
void freeRBTree(RBTREE *tree);

//free everything in the tree, including its contained values
void destroyRBTree(RBTREE *tree);

/*
 * here's a cleaner, albeit boring, version of uncleOf:

	RBTREE *uncleOf(RBTREE *t)
	{
		if (t->p && t->p->p)
		{
			if (t->p->p->l == t->p)
			{
				return t->p->p->r;
			}
			else
			{
				return t->p->p->l;
			}
		}
		else
		{
			return NULL;
		}
	}
 I'm still keeping the macro for fun
*/
#endif