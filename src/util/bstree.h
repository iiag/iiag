#ifndef BSTREE_H
#define BSTREE_H
#include "tree.h"

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
int compare_int(void *args1,void *args2);

//helper for inserting decimals into tree
int compare_double(void *args1,void *args2);

#endif