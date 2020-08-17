#include "graph.h"
#include "list.h"

#include <assert.h>

GRAPH *initGraph()
{
    GRAPH *res = calloc(1, sizeof(GRAPH));
    res->nodes = initList();

    return res;
}

//compare macros to use as lambda
#pragma region macros
BOOL equalsNode(void *x, void *y)
{
    if (*(int *)x == ((NODE *)y)->label) {
        return TRUE;
    } else if (*(int *)x < ((NODE *)y)->label) {
        return UNKNOWN;
    } else {
        return FALSE;
    }
}

BOOL equalsEdge(void *x, void *y)
{
    if (*(int *)x == ((EDGE *)y)->ingoing->label) {
        return TRUE;
    } else if (*(int *)x < ((EDGE *)y)->ingoing->label) {
        return UNKNOWN;
    } else {
        return FALSE;
    }
}

BOOL NodegreaterNode(void *x, void *y)
{
    return ((NODE *)x)->label < ((NODE *)y)->label;
}

BOOL EdgegreaterEdge(void *x, void *y)
{
    return ((EDGE *)x)->ingoing->label < ((EDGE *)y)->ingoing->label;
}
#pragma endregion

//add - remove
NODE *addNode(GRAPH *g, int value)
{
    if(find(g->nodes, &value, equalsNode)){
        return NULL;
    }
    NODE *new = calloc(1, sizeof(NODE));
    new->label = value;
    new->edges = initList();

    insertSorted(new, g->nodes, NodegreaterNode);

    return new;
}

BOOL removeNode(GRAPH *g, int label)
{
    LIST *p = find(g->nodes, &label, equalsNode);
    if(!p || p == g->nodes) {
        return FALSE;
    }
    freeNode((NODE *) removeElt(p));
    return TRUE;
}
//TODO -> remove ingoing edges too

void addEdge(GRAPH *g, int out, int in, int weight, BOOL reversed)
{
    EDGE *new = calloc(1, sizeof(EDGE));
    new->weight = weight;
    NODE *outg = getNode(g, out);
    outg = outg ? outg : addNode(g, out);
    NODE *ing = getNode(g, in);
    ing = ing ? ing : addNode(g, in);

    new->outgoing = outg;
    new->ingoing = ing;

    insertSorted(new, outg->edges, EdgegreaterEdge);
    if(reversed)
        addEdge(g, in, out, weight, FALSE);
}

BOOL removeEdge(GRAPH *g, int out, int in, int weight)
{
    NODE *outg = getNode(g, out);
    if (!outg) {
        return FALSE;
    }
    LIST *p = find(outg->edges, &in, equalsEdge);
    if (!p || p == outg->edges) {
        return FALSE;
    }
    free(removeElt(p));
    return TRUE;
}

//getters
NODE *getNode(GRAPH *g, int label)
{
    LIST *tmp = find(g->nodes, &label, equalsNode);
    return !tmp ? NULL :(NODE *)tmp->value;
}

EDGE *getEdge(NODE *out, int in)
{
    LIST *tmp = find(out->edges, &in, equalsEdge);
    return !tmp ? NULL :(EDGE *)tmp->value;
}

//misc
void freeNode(NODE *n)
{
    destroyList(n->edges);
    free(n);
}

void freeGraph(GRAPH *g)
{
	LIST *temp;
    for (LIST *p = g->nodes->next; p != g->nodes; p = temp){
        temp = p->next;
        freeNode((NODE *) p->value);
        free(p);
    }
    free(g->nodes);
    free(g);
}

void displayGraph(GRAPH *g)
{
    printf("Graph :\n");
    for (LIST *p = g->nodes->next; p != g->nodes; p = p->next)
    {
        NODE *n = p->value;
        printf(" - Node nb %d. Edges :\n", n->label);
        for (LIST *q = n->edges->next; q != n->edges; q = q->next)
        {
            EDGE *e = q->value;
            printf("    - Edge from %d to %d. Weight %d.\n", 
            e->outgoing->label, e->ingoing->label, e->weight);
        }
    }

    printf("End of graph.\n");
}

//applications
BOOL isAdjacent(GRAPH *g, NODE *x, NODE *y)
{
    return getEdge(x, y->label) ? TRUE : FALSE;
}

int main()
{
    printf("Beginning graph tests...\n");
    GRAPH *g = initGraph();
    
    assert(addNode(g, 1));
    assert(addNode(g, -11));
    assert(addNode(g, 62));
    assert(addNode(g, 14));
    assert(!addNode(g, 1));
    assert(!addNode(g, 14));
    assert(getNode(g, 1)->label == 1);

    addEdge(g, 1, 2, 0, FALSE);
    addEdge(g, 1, 4, 1, FALSE);
    addEdge(g, 4, 6, 3, TRUE);

    displayGraph(g);

    assert(removeEdge(g, 1, 4, 0));
    assert(removeEdge(g, 4, 6, 3));
    assert(!removeEdge(g, 4, 6, 3));
    assert(!removeEdge(g, 2, 2, 3));

    //displayGraph(g);

    assert(removeNode(g, 14));
    assert(!removeNode(g, 14));
    assert(!removeNode(g, 15));
    assert(removeNode(g, 1));

    displayGraph(g);

    printf("clearing up...\n");
    freeGraph(g);
    printf("all passed !\n");
    return 0;
}