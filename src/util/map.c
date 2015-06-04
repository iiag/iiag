#include "map.h"
#include "AVLTree.h"

struct AVL_Map {
	AVLTree *tree;
};

typedef struct int_pair pair_t;
struct int_pair {
	int key;
	void *data;
};

int pair_comp(void *v1, void *v2) {
	int n1;
	int n2;
	n1 = ((pair_t *)v1)->key;
	n2 = ((pair_t *)v2)->key;

	if(n1 == n2) {
		return 0;
	}
	if(n1 > n2) {
		return 1;
	}
	return -1;
}

map_t *map_new() {
	map_t *ret = malloc(sizeof(map_t));
	ret->tree = avl_new(pair_comp);
	return ret;
}

void map_free(map_t *m) {
	avl_destroy(m->tree);
	free(m);
}

void *map_get(map_t *m, int k) {
	pair_t tmp;
	tmp.key = k;
	return ((pair_t *)avl_find(m->tree, &tmp))->data;
}

void *map_remove(map_t *m, int k) {
	void *ret;
	pair_t tmp;
	tmp.key = k;
	ret = ((pair_t *)avl_find(m->tree, &tmp))->data;

	avl_delete(m->tree, &tmp);
	return ret;
}

void map_add(map_t *m, int k, void *d) {
	pair_t *tmp = malloc(sizeof(pair_t));
	tmp->key = k;
	tmp->data = d;
	avl_add(m->tree, tmp);
}

struct map_map_pair {
	map_map_func fun;
	void *arg;
};

void map_avl_func(void *p, void *arg) {
	struct map_map_pair *pp = arg;
	pair_t *d = p;
	pp->fun(d->key, d->data, pp->arg);
}

void map_map(map_t *m, map_map_func f, void *arg) {
	struct map_map_pair pp;
	pp.fun = f;
	pp.arg = arg;
	avl_map(m->tree, &pp, map_avl_func);
}
