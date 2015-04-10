#ifndef TREE_H
#define TREE_H

typedef struct tree_s{
  void *data;     //data contained in tree
  void *attribute;  //use for depth or balance of trees
  struct tree_s *left;
  struct tree_s *right;
}tree_t;

typedef void (* func)(void *,void *);
typedef void (* print)(void *,int );

// Instantiates a new tree with a given key 
tree_t * make_tree(void);

// destroy tree
void destroy_tree(tree_t *);

// prints inorder transversal of tree
void print_inorder(tree_t *,int spaces,print);

// prints preorder transversal of tree
void print_preorder(tree_t *,int spaces,print);

// prints postorder transversal of tree
void print_postorder(tree_t *,int spaces,print);

// Applies a function to each element with an argument in the tree
void map_func(tree_t *,void *arg, func);

// helper to print integers
void print_int(int *data,int spaces);

// helper to print decimals
void print_double(double *data,int spaces);

// helper to print floats
void print_float(float *data,int spaces);

#endif
