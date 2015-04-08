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
    free(tree->attribute);
    free(tree);
  }
  return 0;
}

// prints inorder transversal of tree
void print_inorder(tree_t *tree, print func){
  if(tree==NULL) return;
  print_inorder(tree->left,func);
  func(tree->data);
  print_inorder(tree->right,func);
}

// prints preorder transversal of tree
void print_preorder(tree_t *tree, print func){
  if(tree==NULL) return;
  func(tree->data);
  print_preorder(tree->left,func);
  print_preorder(tree->right,func);
}

// prints postorder transversal of tree
void print_postorder(tree_t *tree, print func){
  if(tree==NULL) return;
  print_inorder(tree->left,func);
  print_inorder(tree->right,func);
  func(tree->data);
}

// Applies a function to each element in the tree
void map_func(tree_t *tree, func funcmap){
  tree_t *temp=tree;
  if(tree==NULL) return;
  map_func(temp->left,funcmap);
  map_func(temp->right,funcmap);
  funcmap(tree->data);
}

// helper to print integers
void print_int(void *data){
  printf("%d ",*(int*)data); 
}

// helper to print decimals
void print_double(void *data){
  printf("%f ",*(double*)data); 
}

