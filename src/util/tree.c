#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

 // Instantiates a new tree
tree_t *make_tree(void){
  tree_t *tree=malloc(sizeof(tree_t));
  tree->data=NULL;
  tree->attribute=NULL;
  tree->left = tree->right = NULL;
  return tree; 
}

// destroy tree
int destroy_tree(tree_t *tree){
  if(tree==NULL){
    return -1;
  }else{
    destroy_tree(tree->left);
    destroy_tree(tree->right);
    free(tree->data);
    free(tree);
  }
  return 0;
}

// prints inorder transversal of tree
void print_inorder(tree_t *tree,int spaces,print funcprint){
  if(tree==NULL) return;
  print_inorder(tree->left,spaces+4,funcprint);
  funcprint(tree->data,spaces);
  print_inorder(tree->right,spaces+4,funcprint);
}

// prints preorder transversal of tree
void print_preorder(tree_t *tree,int spaces,print funcprint){
  if(tree==NULL) return;
  funcprint(tree->data,spaces);
  print_preorder(tree->left,spaces+4,funcprint);
  print_preorder(tree->right,spaces+4,funcprint);
}

// prints postorder transversal of tree
void print_postorder(tree_t *tree,int spaces,print funcprint){
  if(tree==NULL) return;
  print_inorder(tree->left,spaces+4,funcprint);
  print_inorder(tree->right,spaces+4,funcprint);
  funcprint(tree->data,spaces);

}

// Applies a function to each element in the tree
void map_func(tree_t *tree,void *arg, func funcmap){
  if(tree==NULL) return;
  map_func(tree->left,arg,funcmap);
  map_func(tree->right,arg,funcmap);
  fprintf(stderr, "map sum: %f tree data: %f\n",*(double *)arg,*(double *)tree->data);
  funcmap(arg,tree->data);
}

// helper to print integers
void print_int(void *data,int spaces){
  int i;
  for(i=0;i<spaces;i++){
    fprintf(stderr, " ");
  }
  fprintf(stderr,"%d \n",*(int*)data); 
}

// helper to print decimals
void print_double(void *data,int spaces){
  int i;
  for(i=0;i<spaces;i++){
    fprintf(stderr, " ");
  }
  fprintf(stderr,"%f \n",*(double*)data); 
}

// helper to print float
void print_float(void *data,int spaces){
  int i;
  for(i=0;i<spaces;i++){
    fprintf(stderr, " ");
  }
  fprintf(stderr,"%f \n",*(float*)data); 
}

