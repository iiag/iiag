#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util/tree.h"
#include "util/bstree.h"

#define expect(C) \
	if (!(C)) { \
		fprintf(stderr, "test failed: " #C); \
		return EXIT_FAILURE; \
	}

void sumify_int(int * sum, int * x)
{
	*sum += *x;
}

void sumify_double(double * sum, double * x)
{
	*sum += *x;
}

void sumify_float(float * sum, float * x)
{
	*sum += *x;
}

void insertAll(tree_t ** tree,void *attributes, void * data, int size, int count, compare func)
{
	int i;
	for (i = 0; i < count; i++) {
		//fprintf(stderr, "%lu  %d \n",sizeof((char *)data),(size * i));
		*tree = insert(*tree, (char *)attributes +(size * i),(char *)data + (size * i), size, func);
	}
}

int main()
{
	tree_t *tree1 = NULL;
	tree_t *tree2 = NULL;
	tree_t *tree3 = NULL;
	tree_t *tree4 = NULL;

	int sum1=0;
	double sum2=0;

	int stuff[] = {2, 3, 200, 5, 101};
	int stuff2[] = {8, 16, 20, 58, 92};

	double stuff3[] = {1.2329, 101.0282, 200.1092, 30.0982, 3.0929};
	double stuff4[] = {8.1123, 16.4312, 30.982, 0.9321, 9.8271};

	insertAll(&tree1, stuff, stuff2, sizeof(int), 5, (compare)compare_int);
	expect(tree1!=NULL);

	fprintf(stderr, "PREORDER START\n");
	print_preorder(tree1,0,(print)print_int);
	fprintf(stderr, "PREORDER DONE\n\n");
	fprintf(stderr, "INORDER START\n");
	print_inorder(tree1,0,(print)print_int);
	fprintf(stderr, "INORDER DONE\n\n");
	fprintf(stderr, "POSTORDER START\n");
	print_postorder(tree1,0,(print)print_int);
	fprintf(stderr, "POSTORDER DONE\n\n");


	tree3 = search(tree1, stuff+1, (compare)compare_int);
	expect(tree3!=NULL);
	expect(*(int *)(tree3->attribute)==*(int *)(stuff+1));
	expect(*(int *)(tree3->data)==*(int *)(stuff2+1));

	map_func(tree1, &sum1, (func)sumify_int);
	expect(sum1 == 8 + 16 + 20 + 58 + 92);

	insertAll(&tree2, stuff3, stuff4, sizeof(double), 5, (compare)compare_double);
	expect(tree2!=NULL);

	fprintf(stderr, "PREORDER START\n");
	print_preorder(tree2,0,(print)print_double);
	fprintf(stderr, "PREORDER DONE\n\n");
	fprintf(stderr, "INORDER START\n");
	print_inorder(tree2,0,(print)print_double);
	fprintf(stderr, "INORDER DONE\n\n");
	fprintf(stderr, "POSTORDER START\n");
	print_postorder(tree2,0,(print)print_double);
	fprintf(stderr, "POSTORDER DONE\n\n");

	tree4 = search(tree2, stuff3+3, (compare)compare_double);
	expect(tree4!=NULL);
	expect(*(double *)(tree4->attribute)==*(double *)(stuff3+3));
	expect(*(double *)(tree4->data)=*(double *)(stuff4+3));

	map_func(tree2, &sum2, (func)sumify_double);
	expect(sum2 == 8.1123 + 16.4312 + 30.982 + 0.9321 + 9.8271);
	
	tree1=delete(tree1,stuff+1,(compare)compare_int);

	tree3 = search(tree1, stuff+1, (compare)compare_int);
	expect(tree3==NULL);

	fprintf(stderr, "PREORDER DELETE ELEM START\n");
	print_preorder(tree1,0,(print)print_int);
	fprintf(stderr, "PREORDER DELETE ELEM DONE\n\n");
	fprintf(stderr, "INORDER DELETE ELEM START\n");
	print_inorder(tree1,0,(print)print_int);
	fprintf(stderr, "INORDER DELETE ELEM DONE\n\n");
	fprintf(stderr, "POSTORDER START\n");
	print_postorder(tree1,0,(print)print_int);
	fprintf(stderr, "POSTORDER DELETE ELEM DONE\n\n");

	tree2=delete(tree2,stuff3+1,(compare)compare_double);

	tree4 = search(tree2, stuff3+1, (compare)compare_double);
	expect(tree4==NULL);

	fprintf(stderr, "PREORDER DELETE ELEM START\n");
	print_preorder(tree2,0,(print)print_double);
	fprintf(stderr, "PREORDER DELETE ELEM DONE\n\n");
	fprintf(stderr, "INORDER DELETE ELEM START\n");
	print_inorder(tree2,0,(print)print_double);
	fprintf(stderr, "INORDER DELETE ELEM DONE\n\n");
	fprintf(stderr, "POSTORDER DELETE ELEM START\n");
	print_postorder(tree2,0,(print)print_double);
	fprintf(stderr, "POSTORDER DELETE ELEM DONE\n\n");

	destroy_tree(tree1);
	destroy_tree(tree2);
	destroy_tree(tree3);
	destroy_tree(tree4);

	return 0;
}