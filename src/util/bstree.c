#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bstree.h"

tree_t *insert(tree_t *tree,void *data,int sizeOfType,compare funccompare){
  tree_t *temp = NULL;
  if(tree==NULL){
    temp = (tree_t *)malloc(sizeof(tree_t));
    temp->left = temp->right = NULL;
    temp->data = malloc(sizeOfType);
    memcpy(temp->data,data,sizeOfType);
    //temp->data = data;
    return temp;
  }
  if(funccompare(data,tree->data)<0){
    tree->left = insert(tree->left,data,sizeOfType,funccompare);
  }else if(funccompare(data,tree->data)>0){
    tree->right = insert(tree->right,data,sizeOfType,funccompare);
  }
  return tree;
}

tree_t *delete(tree_t *tree, void *data,compare funccompare){
  tree_t *temp;
  if(tree==NULL) return tree;
  if(funccompare(data,tree->data)<0){
    tree->left = delete(tree->left, data, funccompare);
  }else if(funccompare(data,tree->data)>0){
    tree->right = delete(tree->right, data, funccompare);
  }else{
   	if(tree->right!=NULL && tree->left!=NULL){
      temp = findMin(tree->right);
      tree -> data = temp->data; 
      tree -> right = delete(tree->right,temp->data, funccompare);
  	}else{
      if(tree->left == NULL){
        temp = tree->right;
        free(tree);
        return temp;
      }else if(tree->right == NULL){
        temp = tree->left;
        free(temp);
        return temp;
      }
    }
  }
  return tree;
}


tree_t *search(tree_t *tree, void *data,compare funccompare){
  if(tree==NULL) return tree;
  if(funccompare(data,tree->data)<0){
    return search(tree->left,data,funccompare);
  }else if(funccompare(data,tree->data)>0){
    return search(tree->right,data,funccompare);
  }else{
    return tree;
  }
  return NULL;
}

//find min element
tree_t *findMin(tree_t *tree){
  if(tree==NULL){
    return NULL;
  }
  if(tree->left!=NULL){
  	return findMin(tree->left);
  }else{
  	return tree;
  }
  return NULL;
}

//find max element
tree_t *findMax(tree_t *tree){
  if(tree==NULL){
    return NULL;
  }
  if(tree->right!=NULL){
  	return findMax(tree->right);
  }else{
  	return tree;
  }
  return NULL;
}

//helper for inserting integers into tree
int compare_int(int *args1,int *args2){
	if(*args1>*args2){
		return 1;
	}else if(*args1<*args2){
		return -1;
	}else{
		return 0;
	}
}

//helper for inserting decimals into tree
int compare_double(double *args1,double *args2){
	if(*args1>*args2){
		return 1;
	}else if(*args1<*args2){
		return -1;
	}else{
		return 0;
	}
}

//helper for inserting floats into tree
int compare_float(float *args1,float *args2){
  if(*args1>*args2){
    return 1;
  }else if(*args1<*args2){
    return -1;
  }else{
    return 0;
  }
}