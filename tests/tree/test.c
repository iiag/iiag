#include <stdio.h>
#include <stdlib.h>
#include "util/tree.h"
#include "util/bstree.h"

#define expect(C) \
	if (!(C)) { \
		fprintf(stderr, "test failed: " #C); \
		return EXIT_FAILURE; \
	}

void sumify_int(int * x, int * sum)
{
	*sum += *x;
}

void sumify_double(double * x, double * sum)
{
	*sum += *x;
}

void insertAll(tree_t ** tree, void * ints, int size, int count, compare func)
{
	int i;
	for (i = 0; i < count; i++) {
		*tree = insert(*tree, (char *)ints + (size * i), size, func);
	}
}

int main()
{
	tree_t *tree1 = NULL;
	tree_t *tree2 = NULL;
	tree_t *tree3 = NULL;
	tree_t *tree4 = NULL;
	tree_t *tree5 = NULL;
	tree_t *tree6 = NULL;

	int sum1=0;
	double sum2=0;

	int stuff[] = {2, 3, 200, 5, 101};
	//int stuff2[] = {8, 16};

	double stuff3[] = {1.2329, 2.0982, 4.8387, 100.9831, 101.0282};
	//double stuff4[] = {8.1123, 16.4312};

	insertAll(&tree1, stuff, sizeof(int), 5, (compare)compare_int);

	fprintf(stderr, "PREORDER START\n");
	print_preorder(tree1,0,(print)print_int);
	fprintf(stderr, "PREORDER DONE\n\n");
	fprintf(stderr, "INORDER START\n");
	print_inorder(tree1,0,(print)print_int);
	fprintf(stderr, "INORDER DONE\n\n");
	fprintf(stderr, "POSTORDER START\n");
	print_postorder(tree1,0,(print)print_int);
	fprintf(stderr, "POSTORDER DONE\n");

	tree3 = search(tree1, stuff+3, (compare)compare_int);
	expect(tree3!=NULL);

	insertAll(&tree2, stuff3, sizeof(double), 5, (compare)compare_double);

	print_preorder(tree2,0,(print)print_double);
	print_inorder(tree2,0,(print)print_double);
	print_postorder(tree2,0,(print)print_double);

	tree4 = search(tree2, stuff3+3, (compare)compare_double);
	expect(tree4!=NULL);

	tree5=findMax(tree1);
	expect(tree5!=NULL);
	expect(*(int *)tree5->data == 200);
	tree5=findMin(tree1);
	expect(tree5!=NULL);
	expect(*(int *)tree5->data == 2);

	tree6=findMax(tree2);
	expect(tree6!=NULL);
	expect(*(double *)tree6->data == 101.0282);
	expect(tree6!=NULL);
	tree6=findMin(tree2);
	expect(*(double *)tree6->data == 1.2329);

	map_func(tree1, &sum1, (func)sumify_int);
	expect(sum1 == 2 + 3 + 200 + 5 + 101);

	map_func(tree2, &sum2, (func)sumify_double);
	expect(sum2 == 1.2329 + 2.0982 + 4.8387 + 100.9831 + 101.0282);

	destroy_tree(tree1);
	destroy_tree(tree2);

	return 0;
}