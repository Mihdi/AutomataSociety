#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "list.h"
#include "misc.h"

#ifndef GRAPH_H
#define GRAPH_H

#ifndef EX_OSERR
#define EX_OSERR 1
#endif

typedef struct graph
{
    LIST *nodes;
} GRAPH;

typedef struct node
{
    int label;
    LIST *edges;
} NODE;

typedef struct edge
{
    NODE *outgoing;
    NODE *ingoing;
    int weight;
} EDGE;

GRAPH *initGraph();

//"lambdas"
BOOL equalsNode(void *x, void *y); // int - NODE
BOOL equalsEdge(void *x, void *y); // int - EDGE
BOOL NodegreaterNode(void *x, void *y); //NODE - NODE
BOOL EdgegreaterEdge(void *x, void *y); //EDGE - EDGE

//create a new node in graph sorted list; 
//if node exists already returns NULL else return NODE pointer
NODE *addNode(GRAPH *g, int value);

//remove node with label from the graph's list and frees it
//TODO also remove all associated edges
BOOL removeNode(GRAPH *g, int label);

void freeNode(NODE *n);
//add weighted edge from out to in; creates the nodes if nonexistant
//add same edge from in to out if directed 
void addEdge(GRAPH *g, int out, int in, int weight, BOOL directed);

//remove and free edge if it exists
BOOL removeEdge(GRAPH *g, int out, int in, int weight);

NODE *getNode(GRAPH *g, int label);
EDGE *getEdge(NODE *out, int in);

BOOL isAdjacent(GRAPH *g, NODE *x, NODE *y);

LIST *neighbors(GRAPH *g, NODE *x);

//setEdgeWeight

//frees and deletes the whole graph
void freeGraph(GRAPH *g);

//printer
void displayGraph(GRAPH *g);
#endif

/* draft
merge addNode and getNode into one function?
removeNode doesn't clear the ingoing edges
need a cleaner way than going through all nodes edges

*/