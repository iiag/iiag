#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct AVLTree_t AVLTree;
typedef struct AVLNode_t AVLNode;

/*
1 >
0 =
-1 <
*/
typedef int (*comp_func)(void *, void *);
typedef void (*map_func)(void *, void *);

AVLTree *avl_new(comp_func);
/*
Note avl_destroy will free data in nodes aswell
*/
void avl_destroy(AVLTree *);
void avl_add(AVLTree *, void *);
void avl_delete(AVLTree *, void *);
void avl_reroot(AVLTree *);
void avl_map(AVLTree *t, void *arg, map_func);
int  avl_height(AVLTree *);
int  avl_size(AVLTree *);
void *avl_find(AVLTree *, void *);

AVLNode *avl_new_node();
void avl_destroy_node(AVLNode *);
void avl_rotate_right(AVLNode *);
void avl_rotate_left(AVLNode *);
void avl_remeasure(AVLNode *);
void avl_rebalance(AVLNode *);
AVLNode *avl_node_find(AVLNode *, void *, comp_func fun);



#endif

