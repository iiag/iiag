#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bstree.h"

tree_t *insert(tree_t *tree,void *data,int sizeOfType,compare func){
  tree_t *temp = NULL;
  if(tree==NULL){
    temp = (tree_t *)malloc(sizeof(tree_t));
    temp->left = temp->right = NULL;
    temp->data = malloc(sizeOfType);
    memcpy(temp->data,data,sizeOfType);
    temp->data = data;
    return temp;
  }
  if(func(data,tree->data)<0){
    insert(tree->left,data,sizeOfType,func);
  }else if(func(data,tree->data)>0){
    insert(tree->right,data,sizeOfType,func);
  }else{
  	return NULL;
  }
  return tree;
}

tree_t *delete(tree_t *tree, void *data,compare func){
  tree_t *temp;
  if(tree==NULL){
    return NULL;
  }else if(func(data,tree->data)<0){
    tree->left = delete(tree->left, data, func);
  }else if(func(data,tree->data)>0){
    tree->right = delete(tree->right, data, func);
  }else{
   	if(tree->right!=NULL && tree->left!=NULL){
      temp = findMin(tree->right);
      tree -> data = temp->data; 
      tree -> right = delete(tree->right,temp->data, func);
  	}else{
      temp = tree;
      if(tree->left == NULL){
        tree = tree->right;
      }else if(tree->right == NULL){
        tree = tree->left;
      }
      free(temp);
    }
  }
  return tree;
}


tree_t *search(tree_t *tree, void *data,compare func){
  if(tree==NULL) return NULL;

  if(func(data,tree->data)<0){
    search(tree->left,data,func);
  }else if(func(data,tree->data)>0){
    search(tree->right,data,func);
  }else if(func(data,tree->data)==0){
    return tree;
  }
  return NULL;
}

//find min element
tree_t *findMin(tree_t *tree){
	tree_t *temp=tree;
  if(temp==NULL){
    return NULL;
  }
  if(temp->left){
  	findMin(temp->left);
  }else{
  	return temp;
  }
  return NULL;
}

//find max element
tree_t *findMax(tree_t *tree){
	tree_t *temp=tree;
  if(temp==NULL){
    return NULL;
  }
  if(temp->right){
  	findMin(temp->right);
  }else{
  	return temp;
  }
  return NULL;
}

//helper for inserting integers into tree
int compare_int(void *args1,void *args2){
	if(*(int *)args1>*(int *)args2){
		return 1;
	}else if(*(int *)args1<*(int *)args2){
		return -1;
	}else{
		return 0;
	}
}

//helper for inserting decimals into tree
int compare_double(void *args1,void *args2){
	if(*(double *)args1>*(double *)args2){
		return 1;
	}else if(*(int *)args1<*(double *)args2){
		return -1;
	}else{
		return 0;
	}
}