#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util/AVLTree.h"
#include "util/map.h"

int int_comp(void* v1, void* v2){
   int n1;int n2;
   n1=*((int*)v1);
   n2=*((int*)v2);
   if(n1==n2)
      return 0;
   if(n1>n2)
      return 1;
   return -1;
}

void print_tree(void* i, void* u){
   printf("%i\n",*(int*)i);
}

void print_par(int i, void* d, void* u){
   printf("(%i,%i)\n",i,*(int*)d);
}

int main(){

   AVLTree* tree = avl_new(int_comp);
   int* tmp;
   int tmp2;
   int x=0;
   srand(time(NULL));
   for(;x<1000000;x++){
      tmp=malloc(sizeof(int));
      *tmp=rand()%1000000;
      avl_add(tree,tmp);
   }

   for(x=0;x<1000000;x++){
      tmp2=rand();
      avl_delete(tree,&tmp2);
   }

   printf("height %i\nsize %i\n",avl_height(tree),avl_size(tree));
   //avl_map(tree,0,print_tree);
   avl_destroy(tree);

   int a=3;
   int b=6;
   int c=7;
   int d=2;

   map_t* maw = map_new();
   map_add(maw,0,&a);
   map_add(maw,1,&b);
   map_add(maw,a,&c);
   map_add(maw,1112,&d);

   map_map(maw,print_par,NULL);
   printf("%i\n",*(int*)map_get(maw,3));

   map_remove(maw,0);
   map_remove(maw,3);
  
   map_map(maw,print_par,NULL);

   map_free(maw);



return 0;
}
