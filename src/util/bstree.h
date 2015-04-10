#ifndef BSTREE_H
#define BSTREE_H
#include "tree.h"

typedef int(* compare)(void *,void *);

//Insert node into tree
tree_t *insert(tree_t *,void *,int,compare);

//Delete node from tree
tree_t *delete(tree_t *,void *,compare);

//Search tree for a element
tree_t *search(tree_t *,void *,compare);

//find min element
tree_t *findMin(tree_t *tree);

//find max element
tree_t *findMax(tree_t *tree);

//helper for inserting integers into tree
int compare_int(int *args1,int *args2);

//helper for inserting decimals into tree
int compare_double(double *args1,double *args2);

//helper for inserting float into tree
int compare_float(float *args1,float *args2);

#endif