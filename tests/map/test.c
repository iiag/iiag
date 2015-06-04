#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "util/AVLTree.h"
#include "util/map.h"

#define NODES_TO_ADD 10000

#define countof(A) (sizeof(A) / sizeof((A)[0]))

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


// Calculates the height of an AVL tree
int max_height(int height) {
   // Roughly 1.44 * log2(height) - 0.328
   // Add 1 to assure we round up
   return (int) (1.44 * log2((double)(height + 2)) + 0.672);
}

void test_avl_tree(void) {
   // Right now this only tests that the tree height is sound
   // Might want to do other tests, but the map tests will handle some

   AVLTree* tree = avl_new(int_comp);
   int* tmp;
   int tmp2;
   int x=0;

   srand(time(NULL));

   for(;x<NODES_TO_ADD;x++){
      tmp=malloc(sizeof(int));
      *tmp=rand()%NODES_TO_ADD;
      avl_add(tree,tmp);
   }

   assert(avl_height(tree) < max_height(avl_size(tree)));

   for(x=0;x<NODES_TO_ADD;x++){
      tmp2=rand();
      avl_delete(tree,&tmp2);
   }

   assert(avl_height(tree) < max_height(avl_size(tree)));

   avl_destroy(tree);
}

static void sum_keys(int key, void *valp0, void *sump0) {
   int *sump = sump0;
   *sump += key;
}

static void sum_vals(int key, void *valp0, void *sump0) {
   int *sump = sump0;
   int *valp = valp0;
   *sump += *valp;
}

static int sum_arr(int arr[], int size) {
   int i, sum;
   for (i = sum = 0; i < size; i++) {
      sum += arr[i];
   }
   return sum;
}

void test_map(void) {
   int i;
   int keys[] = { 1, 0, 4, 3, 123, 43, 1112 };
   int vals[] = { 3, 6, 7, 2,  92,  3,  123 };

   assert(countof(keys) == countof(vals));

   // check add & get
   map_t* maw = map_new();

   for (i = 0; i < countof(keys); i++) {
      map_add(maw, keys[i], vals + i);
   }

   for (i = 0; i < countof(keys); i++) {
      assert(vals[i] == *(int *)map_get(maw, keys[i]));
   }

   // check map
   int key_sum = 0;
   int val_sum = 0;

   map_map(maw, sum_keys, &key_sum);
   map_map(maw, sum_vals, &val_sum);

   assert(key_sum == sum_arr(keys, countof(keys)));
   assert(val_sum == sum_arr(vals, countof(vals)));

   // check remove
   map_remove(maw, 0);
   map_remove(maw, 3);

   for (i = 0; i < countof(keys); i++) {
      if (keys[i] == 0 || keys[i] == 3) {
         assert(NULL == map_get(maw, keys[i]));
      } else {
         assert(vals[i] == *(int *)map_get(maw, keys[i]));
      }
   }

   // check map set
   int a = 12;
   int b = 13;

   assert(NULL == map_set(maw, -1, &a));
   assert(a == *(int *)map_get(maw, -1));
   assert(a == *(int *)map_set(maw, -1, &b));
   assert(b == *(int *)map_get(maw, -1));

   map_free(maw);
}


int main(){
   test_avl_tree();
   test_map();
   return 0;
}
