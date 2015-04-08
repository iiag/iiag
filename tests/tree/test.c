#include <stdio.h>
#include <stdlib.h>
#include "util/tree.h"
#include "util/bstree.h"

#define expect(C) \
	if (!(C)) { \
		fprintf(stderr, "test failed: " #C); \
		return EXIT_FAILURE; \
	}

int sum = 0;

void sumify(int * x)
{
	sum += *x;
}

void insertAll(tree_t * tree, void * ints, int count, compare func)
{
	int i;
	for (i = 0; i < count; i++) {
		insert(tree, ints + i, sizeof(int), func);
	}
}

int main()
{
	tree_t *tree1 = NULL;
	tree_t *tree2 = NULL;


	int stuff[] = {1, 2, 4, 100, 101};
	//int stuff2[] = {8, 16};

	double stuff3[] = {1.2329, 2.0982, 4.8387, 100.9831, 101.0282};
	//double stuff4[] = {8.1123, 16.4312};

	insertAll(tree1, stuff, 5, (compare)compare_int);

	print_preorder(tree1,(print)print_int);
	print_inorder(tree1,(print)print_int);
	print_postorder(tree1,(print)print_int);

	insertAll(tree2, stuff3, 5, (compare)compare_double);

	print_preorder(tree2,(print)print_double);
	print_inorder(tree2,(print)print_double);
	print_postorder(tree2,(print)print_double);


	//expect(*(int *)findMax(tree1) == 101);
	//expect(*(int *)findMin(tree1) == 1);

	//expect(*(int *)findMax(tree2) == 101.0282);
	//expect(*(int *)findMin(tree2) == 1.2329);

	//map_func(tree, (func)sumify);

	//expect(sum == 1 + 2 + 4 + 8 + 16);

	destroy_tree(tree1);
	destroy_tree(tree2);

	return 0;
}