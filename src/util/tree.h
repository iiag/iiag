#ifndef TREE_H
#define TREE_H

typedef struct tree_s {
  void *data; 		//data contained in tree
  void *attribute; 	//use for depth or balance of trees
  struct tree_s *left;
  struct tree_s *right;
}tree_t;

typedef void (* func)(void *);
typedef int (* compare)(void *,void *);
typedef void (* print)(void *);

// Instantiates a new tree with a given key 
tree_t * make_tree(void);

// destroy tree
int destroy_tree(tree_t *);

// prints inorder transversal of tree
void print_inorder(tree_t *, print);

// prints preorder transversal of tree
void print_preorder(tree_t *, print);

// prints postorder transversal of tree
void print_postorder(tree_t *, print);

// Applies a function to each element in the tree
void map_func(tree_t *, func);

// helper to print integers
void print_int(void *data);

// helper to print decimals
void print_double(void *data);

#endif
