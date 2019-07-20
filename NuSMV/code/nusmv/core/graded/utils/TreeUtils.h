#ifndef _TREEDUTILS_

#define _TREEDUTILS_

#include "nusmv/core/fsm/bdd/bdd.h"

typedef struct adjList *adjList_ptr;
typedef struct treeNode *treeNode_ptr;

typedef struct adjList{
       adjList_ptr next;
       treeNode_ptr nodo;
} adjList;

typedef struct treeNode{
       BddStates stato;
       BddInputs input;
       adjList_ptr lista;
} treeNode;


#endif


treeNode_ptr TreeUtils_treeNodeCreate(BddStates, BddInputs);
void TreeUtils_concat(treeNode_ptr, treeNode_ptr);

BddStates TreeUtils_getStato(treeNode_ptr);
BddInputs TreeUtils_getInput(treeNode_ptr);
adjList_ptr TreeUtils_getListaFigli(treeNode_ptr);
adjList_ptr TreeUtils_getNext(adjList_ptr);
treeNode_ptr TreeUtils_getNodo(adjList_ptr);
void TreeUtils_freeNodo(treeNode_ptr);