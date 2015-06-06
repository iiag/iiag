#include <stdlib.h>
#include "AVLTree.h"

struct AVLNode_t {
	void *data;
	int height;
	AVLNode *right;
	AVLNode *left;
	AVLNode *parent;
};

struct AVLTree_t {
	int size;
	AVLNode *root;
	comp_func comp;
};


AVLTree *avl_new(comp_func fun) {
	AVLTree *t = malloc(sizeof(AVLTree));
	t->size = 0;
	t->root = NULL;
	t->comp = fun;
	return t;
}

void avl_destroy(AVLTree *t) {
	if(t->root) {
		avl_destroy_node(t->root);
	}
	free(t);
}

AVLNode *avl_new_node() {
	AVLNode *n = malloc(sizeof(AVLNode));
	n->height = 0;
	n->data = NULL;
	n->right = n->left = n->parent = NULL;
	return n;
}

void avl_destroy_node(AVLNode *n) {
	if(n->right) {
		avl_destroy_node(n->right);
	}
	if(n->left) {
		avl_destroy_node(n->left);
	}
	if(n->data) {
		free(n->data);
	}
	free(n);
}

void avl_free_node(AVLNode *n) {
	if(n->data) {
		free(n->data);
	}
	free(n);
}

int avl_size(AVLTree *t) {
	return t->size;
}

int avl_height(AVLTree *t) {
	if(t->root) {
		return t->root->height;
	}
	return 0;
}

void avl_add(AVLTree *t, void *data) {
	AVLNode *n = avl_new_node();
	n->data = data;
	n->height = 1;
	t->size++;
	if(t->root) {
		AVLNode *c = t->root;
		//add node, or destroy either way
		int cmp;
		while(13) {
			cmp = t->comp(c->data, data);
			if(cmp < 0) {
				if(c->right) {
					c = c->right;
				} else {
					c->right = n;
					n->parent = c;
					avl_remeasure(n);
					avl_rebalance(n);
					avl_reroot(t);
					return;
				}
			}
			if(cmp > 0) {
				if(c->left) {
					c = c->left;
				} else {
					c->left = n;
					n->parent = c;
					avl_remeasure(n);
					avl_rebalance(n);
					avl_reroot(t);
					return;
				}
			}
			if(cmp == 0) {
				avl_destroy_node(n);
				t->size--;
				return;
			}
		}
	} else {
		t->root = n;
	}
}

AVLNode *avl_node_find(AVLNode *n, void *f, comp_func fun) {

	if(n == NULL) {
		return NULL;
	}
	if(fun(n->data, f) == 0) {
		return n;
	}
	if(fun(n->data, f) < 0) {
		return avl_node_find(n->right, f, fun);
	}

	// fun(n->data, f) > 0 is true
	return avl_node_find(n->left, f, fun);
}

AVLNode *avl_greatest(AVLNode *n) {
	while(n->right) {
		n = n->right;
	}
	return n;
}

AVLNode *avl_least(AVLNode *n) {
	while(n->left) {
		n = n->left;
	}
	return n;
}

void avl_unlink_parent(AVLNode *n, AVLNode *s) {
	if(n->parent) {
		if(n->parent->left == n) {
			n->parent->left = s;
		}
		if(n->parent->right == n) {
			n->parent->right = s;
		}
	}
}

void avl_delete_node(AVLNode *n) {

	if(n->left == NULL && n->right == NULL) {
		avl_unlink_parent(n, NULL);
		if(n->parent) {
			avl_remeasure(n->parent);
			avl_rebalance(n->parent);
		}
		avl_free_node(n);
		return;
	}

	if(n->left == NULL) {
		avl_unlink_parent(n, n->right);
		n->right->parent = n->parent;
		if(n->parent) {
			avl_remeasure(n->parent);
			avl_rebalance(n->parent);
		}
		avl_free_node(n);
		return;
	}

	if(n->right == NULL) {
		avl_unlink_parent(n, n->left);
		n->left->parent = n->parent;
		if(n->parent) {
			avl_remeasure(n->parent);
			avl_rebalance(n->parent);
		}
		avl_free_node(n);
		return;
	}

	AVLNode *c = avl_greatest(n->left);
	AVLNode *p;

	avl_unlink_parent(c, c->left);
	if(c->left) {
		c->left->parent = n->parent;
	}

	avl_unlink_parent(n, c);
	p = c->parent;
	c->parent = n->parent;
	c->right = n->right;
	c->left = n->left;
	if(c->right) {
		c->right->parent = c;
	}
	if(c->left) {
		c->left->parent = c;
	}

	if(p == n) {
		p = c;
	}

	avl_remeasure(p);
	avl_rebalance(p);
	avl_free_node(n);
}

void *avl_find(AVLTree *t, void *d) {
	AVLNode *n = avl_node_find(t->root, d, t->comp);
	return n ? n->data : NULL;
}

void avl_delete(AVLTree *t, void *d) {
	AVLNode *toDie = avl_node_find(t->root, d, t->comp);

	if(!toDie) {
		return;
	}

	t->size--;

	if(toDie == t->root) {
		if(t->root->right) {
			t->root = t->root->right;
		} else if(t->root->left) {
			t->root = t->root->left;
		} else {
			t->root = NULL;
		}
	}

	avl_delete_node(toDie);
	avl_reroot(t);

}

void avl_reroot(AVLTree *t) {
	while(t->root->parent) {
		t->root = t->root->parent;
	}
}

#define max(a,b) (a>b?a:b)

void avl_remeasure(AVLNode *n) {
	int h = 1;
	if(n->right) {
		h = max(h, (n->right->height) + 1);
	}
	if(n->left) {
		h = max(h, (n->left->height) + 1);
	}
	n->height = h;
	if(n->parent) {
		avl_remeasure(n->parent);
	}
}

int avl_factor(AVLNode *n) {
	int l = 0, r = 0;
	if(n->right) {
		r = n->right->height;
	}
	if(n->left) {
		l = n->left->height;
	}
	return l - r;
}

void avl_rotate_right(AVLNode *n) {
	AVLNode *c = n->left;

	n->left = c->right;
	if(n->left) {
		n->left->parent = n;
	}

	if(n->parent) {
		if(n->parent->right == n) {
			n->parent->right = c;
		}
		if(n->parent->left == n) {
			n->parent->left = c;
		}
	}
	c->parent = n->parent;
	n->parent = c;

	c->right = n;
	avl_remeasure(n);
}

void avl_rotate_left(AVLNode *n) {
	AVLNode *c = n->right;

	n->right = c->left;
	if(n->right) {
		n->right->parent = n;
	}

	if(n->parent) {
		if(n->parent->right == n) {
			n->parent->right = c;
		}
		if(n->parent->left == n) {
			n->parent->left = c;
		}
	}
	c->parent = n->parent;
	n->parent = c;

	c->left = n;
	avl_remeasure(n);
}

void avl_rebalance(AVLNode *n) {
	int t = avl_factor(n);

	if(t > 1) {
		if(avl_factor(n->left) < 0) {
			avl_rotate_left(n->left);
		}
		avl_rotate_right(n);
	}
	if(t < -1) {
		if(avl_factor(n->right) > 0) {
			avl_rotate_right(n->right);
		}
		avl_rotate_left(n);
	}

	if(n->parent) {
		avl_rebalance(n->parent);
	}
}

void avl_map_node(AVLNode *n, void *arg, map_func fun) {
	if(n->left) {
		avl_map_node(n->left, arg, fun);
	}

	fun(n->data, arg);

	if(n->right) {
		avl_map_node(n->right, arg, fun);
	}
}

void avl_map(AVLTree *t, void *arg, map_func fun) {
	if(t->root) {
		avl_map_node(t->root, arg, fun);
	}
}
