#include <stdio.h>
#include <stdlib.h>
#include "util/list.h"

#define expect(C) \
	if (!(C)) { \
		fprintf(stderr, "test failed on line %d: " #C "\n", __LINE__); \
		return EXIT_FAILURE; \
	}

void sumify(int * x, int * sum)
{
	*sum += *x;
}

void push_all(list_t * ls, int * ints, int cnt)
{
	int i;

	for (i = 0; i < cnt; i++) {
		list_push(ls, ints + i);
	}
}

void push_all_back(list_t * ls, int * ints, int cnt)
{
	int i;

	for (i = 0; i < cnt; i++) {
		list_push_back(ls, ints + i);
	}
}

int main()
{
	int sum = 0;
	list_t * l = list_new();

	int stuff[] = {1, 2, 4, 100, 101};
	int stuff2[] = {8, 16};

	// l = []
	expect(list_pop(l) == NULL);
	expect(list_pop_back(l) == NULL);

	list_push(l, stuff);
	// l = [1]

	expect(*(int *)list_pop(l) == 1);
	expect(list_pop(l) == NULL);
	// l = []

	list_push_back(l, stuff);
	// l = [1]

	expect(*(int *)list_pop_back(l) == 1);
	expect(list_pop_back(l) == NULL);
	// l = []

	push_all(l, stuff, 5);
	// l = [101, 100, 4, 2, 1]

	expect(list_size(l) == 5);

	expect(*(int *)list_pop(l) == 101);
	expect(*(int *)list_pop(l) == 100);
	// l = [4, 2, 1]

	expect(list_size(l) == 3);

	push_all_back(l, stuff2, 2);
	// l = 4, 2, 1, 8, 16

	list_foreach_reverse(l, &sum, (list_foreach_func)sumify);
	expect(sum == 1 + 2 + 4 + 8 + 16);

	sum = 0;
	list_foreach(l, &sum, (list_foreach_func)sumify);
	expect(sum == 1 + 2 + 4 + 8 + 16);

	list_destroy(l);

	return 0;
}
